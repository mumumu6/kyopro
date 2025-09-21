#!/usr/bin/env python3
"""Shared utilities for running the AHC054 tester."""

from __future__ import annotations

import contextlib
import re
import shlex
import subprocess
import sys
from pathlib import Path
from typing import Iterable

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
    tester_cmd: Iterable[str],
    solver_cmd: Iterable[str],
    inp: Path,
    output_dir: Path | None,
    timeout: float | None,
    verbose: bool,
) -> tuple[float | None, str]:
    cmd = list(tester_cmd) + list(solver_cmd)
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
            joined = " ".join(cmd)
            raise FileNotFoundError(f"Failed to launch command: {joined}") from exc

    stderr_text = completed.stderr.decode(errors="replace")
    if verbose and stderr_text.strip():
        print(stderr_text.strip(), file=sys.stderr)

    if completed.returncode != 0:
        print(
            f"[warn] tester exited with code {completed.returncode} for {inp.name}",
            file=sys.stderr,
        )
        return None, stderr_text

    try:
        score = parse_score(stderr_text)
    except ValueError as exc:
        print(f"[warn] {exc}", file=sys.stderr)
        return None, stderr_text

    return score, stderr_text

