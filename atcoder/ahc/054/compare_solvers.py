#!/usr/bin/env python3
"""Run two solvers on the same AHC054 instances and visualise score differences."""

from __future__ import annotations

import argparse
import csv
import math
import sys
from pathlib import Path

from score_utils import (
    DEFAULT_INPUT_DIR,
    DEFAULT_TESTER,
    collect_inputs,
    parse_command,
    run_case,
)


def import_matplotlib():
    try:
        import matplotlib.pyplot as plt  # type: ignore

        return plt
    except ImportError:
        return None


def sanitise_label(label: str) -> str:
    return label.strip() or "solver"


def make_output_dirs(base: Path | None, label_a: str, label_b: str) -> tuple[Path | None, Path | None]:
    if base is None:
        return None, None
    a_dir = base / label_a.replace(" ", "_")
    b_dir = base / label_b.replace(" ", "_")
    return a_dir, b_dir


def write_csv(path: Path, rows: list[dict[str, object]]) -> None:
    fieldnames = ["index", "input", "score_a", "score_b", "diff"]
    with path.open("w", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def plot_results(
    rows: list[dict[str, object]],
    label_a: str,
    label_b: str,
    plot_path: Path,
) -> None:
    plt = import_matplotlib()
    if plt is None:
        print("[info] matplotlib not available; skipping plot generation", file=sys.stderr)
        return

    indices = [row["index"] for row in rows]
    score_a = [row["score_a"] for row in rows]
    score_b = [row["score_b"] for row in rows]
    diffs = [row["diff"] for row in rows]

    fig, (ax_scores, ax_diff) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    ax_scores.plot(indices, score_a, label=label_a, marker="o")
    ax_scores.plot(indices, score_b, label=label_b, marker="o")
    ax_scores.set_ylabel("Score")
    ax_scores.legend()
    ax_scores.grid(True, alpha=0.3)

    ax_diff.bar(indices, diffs, color=["#2ca02c" if d >= 0 else "#d62728" for d in diffs])
    ax_diff.axhline(0.0, color="black", linewidth=0.8)
    ax_diff.set_ylabel(f"{label_b} - {label_a}")
    ax_diff.set_xlabel("Case index")
    ax_diff.grid(True, axis="y", alpha=0.3)

    xtick_labels = [row["input"] for row in rows]
    if len(xtick_labels) <= 30:
        ax_diff.set_xticks(indices, xtick_labels, rotation=45, ha="right")
    else:
        step = math.ceil(len(indices) / 30)
        tick_positions = indices[::step]
        tick_labels = xtick_labels[::step]
        ax_diff.set_xticks(tick_positions, tick_labels, rotation=45, ha="right")

    fig.tight_layout()
    plot_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(plot_path)
    plt.close(fig)
    print(f"[info] Saved plot to {plot_path}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Compare two solvers on the same set of AHC054 instances",
    )
    parser.add_argument(
        "--tester",
        default=str(DEFAULT_TESTER),
        help="Tester command (binary path or 'cargo run -r --bin tester --')",
    )
    parser.add_argument("--solver-a", required=True, help="Command for solver A")
    parser.add_argument("--solver-b", required=True, help="Command for solver B")
    parser.add_argument("--label-a", default="Solver A", help="Label for solver A")
    parser.add_argument("--label-b", default="Solver B", help="Label for solver B")
    parser.add_argument(
        "--inputs",
        default=str(DEFAULT_INPUT_DIR),
        help="Directory containing input cases",
    )
    parser.add_argument(
        "--count",
        type=int,
        default=50,
        help="Number of cases to evaluate (<= available, 0 for all)",
    )
    parser.add_argument("--shuffle", action="store_true", help="Shuffle input order")
    parser.add_argument(
        "--timeout",
        type=float,
        default=None,
        help="Optional timeout per tester execution (seconds)",
    )
    parser.add_argument(
        "--out-dir",
        type=str,
        default=None,
        help="If set, store solver outputs under this directory (per solver)",
    )
    parser.add_argument(
        "--csv",
        type=str,
        default="solver_comparison.csv",
        help="Path to write per-case scores (CSV)",
    )
    parser.add_argument(
        "--plot",
        type=str,
        default="solver_comparison.png",
        help="Path to save the comparison plot",
    )
    parser.add_argument("--no-plot", action="store_true", help="Skip plot generation")
    parser.add_argument("--verbose", action="store_true", help="Show tester stderr")

    args = parser.parse_args()

    label_a = sanitise_label(args.label_a)
    label_b = sanitise_label(args.label_b)

    try:
        tester_cmd = parse_command(args.tester)
        solver_a_cmd = parse_command(args.solver_a)
        solver_b_cmd = parse_command(args.solver_b)
    except ValueError as exc:
        parser.error(str(exc))

    input_dir = Path(args.inputs)
    count = max(args.count, 0)
    out_dir = Path(args.out_dir) if args.out_dir is not None else None
    out_a_dir, out_b_dir = make_output_dirs(out_dir, label_a, label_b)

    try:
        inputs = collect_inputs(input_dir, count if count else sys.maxsize, args.shuffle)
    except (FileNotFoundError, ValueError) as exc:
        parser.error(str(exc))

    if not inputs:
        parser.error("No inputs selected for evaluation")

    rows: list[dict[str, object]] = []
    failed: list[str] = []

    for idx, inp in enumerate(inputs, 1):
        score_a, _stderr_a = run_case(
            tester_cmd=tester_cmd,
            solver_cmd=solver_a_cmd,
            inp=inp,
            output_dir=out_a_dir,
            timeout=args.timeout,
            verbose=args.verbose,
        )

        score_b, _stderr_b = run_case(
            tester_cmd=tester_cmd,
            solver_cmd=solver_b_cmd,
            inp=inp,
            output_dir=out_b_dir,
            timeout=args.timeout,
            verbose=args.verbose,
        )

        if score_a is None or score_b is None:
            failed.append(inp.name)
            print(f"{idx:3d}. {inp.name:<12} -> failed", flush=True)
            continue

        diff = float(score_b) - float(score_a)
        rows.append(
            {
                "index": idx,
                "input": inp.name,
                "score_a": float(score_a),
                "score_b": float(score_b),
                "diff": diff,
            }
        )
        print(
            f"{idx:3d}. {inp.name:<12} -> {label_a}: {score_a:.6f}, {label_b}: {score_b:.6f}, diff: {diff:+.6f}",
            flush=True,
        )

    if not rows:
        print("\nNo successful paired scores recorded.")
        if failed:
            print(f"Failed cases ({len(failed)}): {', '.join(failed)}")
        return

    csv_path = Path(args.csv)
    csv_path.parent.mkdir(parents=True, exist_ok=True)
    write_csv(csv_path, rows)
    print(f"\nSaved per-case scores to {csv_path}")

    avg_a = sum(row["score_a"] for row in rows) / len(rows)
    avg_b = sum(row["score_b"] for row in rows) / len(rows)
    avg_diff = avg_b - avg_a

    print(f"Cases compared : {len(rows)}")
    print(f"Average {label_a}: {avg_a:.6f}")
    print(f"Average {label_b}: {avg_b:.6f}")
    print(f"Average diff   : {avg_diff:+.6f} ({label_b} - {label_a})")
    print(f"Best diff      : {max(row['diff'] for row in rows):+.6f}")
    print(f"Worst diff     : {min(row['diff'] for row in rows):+.6f}")

    if failed:
        print(f"Failed cases ({len(failed)}): {', '.join(failed)}")

    if not args.no_plot:
        plot_results(rows, label_a, label_b, Path(args.plot))


if __name__ == "__main__":
    main()

