#!/usr/bin/env python3
"""Batch run the AHC054 local tester to collect score statistics."""

import argparse
import statistics
import sys
from pathlib import Path

from score_utils import (
    DEFAULT_INPUT_DIR,
    DEFAULT_TESTER,
    collect_inputs,
    parse_command,
    run_case,
)


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
