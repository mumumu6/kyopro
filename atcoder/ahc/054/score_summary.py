#!/usr/bin/env python3
"""Batch run the AHC054 local tester to collect score statistics."""

import argparse
import contextlib
import re
import shlex
import statistics
import subprocess
import sys
from pathlib import Path

SCORE_PATTERN = re.compile(r"Score\s*=\s*([-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?)")

BASE_DIR = Path(__file__).resolve().parent
_TOOL_CANDIDATES = [
    BASE_DIR / "tools",
    BASE_DIR / "YDAxDRZr_v2_windows" / "tools_x86_64-pc-windows-gnu",
]

for _candidate in _TOOL_CANDIDATES:
    if _candidate.exists():
        DEFAULT_TOOLS_DIR = _candidate
        break
else:
    DEFAULT_TOOLS_DIR = _TOOL_CANDIDATES[0]

_LINUX_TESTER = DEFAULT_TOOLS_DIR / "target" / "release" / "tester"
_WINDOWS_TESTER = DEFAULT_TOOLS_DIR / "tester.exe"

if _LINUX_TESTER.exists():
    DEFAULT_TESTER = str(_LINUX_TESTER)
elif _WINDOWS_TESTER.exists():
    DEFAULT_TESTER = str(_WINDOWS_TESTER)
else:
    DEFAULT_TESTER = "cargo run -q -r --bin tester --"

DEFAULT_INPUT_DIR = DEFAULT_TOOLS_DIR / "in"


def parse_command(command: str) -> list[str]:
    parts = shlex.split(command)
    if not parts:
        raise ValueError("Empty command string")
    return parts


def parse_score(stderr: str) -> float:
    matches = SCORE_PATTERN.findall(stderr)
    if not matches:
        raise ValueError(f"Could not find score in tester output:\n{stderr.strip()}")
    return float(matches[-1])


def collect_inputs(input_dir: Path, limit: int, shuffle: bool) -> list[Path]:
    inputs = sorted(input_dir.glob("*.txt"))
    if not inputs:
        raise FileNotFoundError(f"No input files found under {input_dir}")
    if shuffle:
        import random

        random.shuffle(inputs)
    if limit > 0:
        inputs = inputs[:limit]
    return inputs


def run_case(
    tester_cmd: list[str],
    solver_cmd: list[str],
    inp: Path,
    output_dir: Path | None,
    timeout: float | None,
    verbose: bool,
) -> tuple[float | None, str]:
    cmd = tester_cmd + solver_cmd
    with inp.open("rb") as stdin, contextlib.ExitStack() as stack:
        if output_dir is not None:
            output_dir.mkdir(parents=True, exist_ok=True)
            out_path = output_dir / f"{inp.stem}.txt"
            stdout_handle = stack.enter_context(out_path.open("wb"))
        else:
            stdout_handle = subprocess.DEVNULL

        try:
            completed = subprocess.run(
                cmd,
                stdin=stdin,
                stdout=stdout_handle,
                stderr=subprocess.PIPE,
                timeout=timeout,
                check=False,
            )
        except FileNotFoundError as exc:
            raise FileNotFoundError(f"Failed to launch command: {' '.join(cmd)}") from exc

    stderr_text = completed.stderr.decode(errors="replace")
    if verbose and stderr_text.strip():
        print(stderr_text.strip(), file=sys.stderr)

    if completed.returncode != 0:
        print(f"[warn] tester exited with code {completed.returncode} for {inp.name}", file=sys.stderr)
        return None, stderr_text

    try:
        score = parse_score(stderr_text)
    except ValueError as exc:
        print(f"[warn] {exc}", file=sys.stderr)
        return None, stderr_text

    return score, stderr_text


def main() -> None:
    parser = argparse.ArgumentParser(description="Batch score evaluator for AHC054 solutions")
    parser.add_argument(
        "--tester",
        default=str(DEFAULT_TESTER),
        help="Command to launch the tester (binary path or 'cargo run -r --bin tester --')",
    )
    parser.add_argument(
        "--solver",
        default="./a.out",
        help="Command to launch the solver under test",
    )
    parser.add_argument(
        "--inputs",
        default=str(DEFAULT_INPUT_DIR),
        help="Directory that contains input instances (*.txt)",
    )
    parser.add_argument(
        "--count",
        type=int,
        default=50,
        help="Number of instances to evaluate (<= available). Use 0 for all.",
    )
    parser.add_argument(
        "--shuffle",
        action="store_true",
        help="Shuffle the available inputs before selecting them",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=None,
        help="Optional timeout (seconds) per tester execution",
    )
    parser.add_argument(
        "--out-dir",
        type=str,
        default=None,
        help="If set, save tester stdout (solver output) under this directory",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print tester stderr for each case",
    )

    args = parser.parse_args()

    try:
        tester_cmd = parse_command(args.tester)
        solver_cmd = parse_command(args.solver)
    except ValueError as exc:
        parser.error(str(exc))

    input_dir = Path(args.inputs)
    count = max(args.count, 0)
    out_dir = Path(args.out_dir) if args.out_dir is not None else None

    try:
        inputs = collect_inputs(input_dir, count if count else sys.maxsize, args.shuffle)
    except (FileNotFoundError, ValueError) as exc:
        parser.error(str(exc))

    if not inputs:
        parser.error("No inputs selected for evaluation")

    scores: list[float] = []
    failed_cases: list[str] = []

    for idx, inp in enumerate(inputs, 1):
        score, _stderr = run_case(
            tester_cmd=tester_cmd,
            solver_cmd=solver_cmd,
            inp=inp,
            output_dir=out_dir,
            timeout=args.timeout,
            verbose=args.verbose,
        )
        if score is None:
            failed_cases.append(inp.name)
            line = f"{idx:3d}. {inp.name:<12} -> failed"
        else:
            scores.append(score)
            line = f"{idx:3d}. {inp.name:<12} -> {score:.6f}"
        print(line)

    if scores:
        average = statistics.fmean(scores)
        print(f"\nEvaluated {len(scores)} cases successfully.")
        print(f"Average score: {average:.6f}")
        print(f"Best score   : {max(scores):.6f}")
        print(f"Worst score  : {min(scores):.6f}")
    else:
        print("\nNo successful scores recorded.")

    if failed_cases:
        print(f"Failed cases ({len(failed_cases)}): {', '.join(failed_cases)}")


if __name__ == "__main__":
    main()
