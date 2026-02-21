#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import shutil
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("contest_id")
    return parser.parse_args()


def validate_contest_id(contest_id: str) -> str:
    normalized = contest_id.lower()
    if not re.fullmatch(r"abc\d{3}", normalized):
        print("Contest ID must match abc + 3 digits (e.g., abc422).", file=sys.stderr)
        raise SystemExit(1)
    return normalized


def main() -> int:
    args = parse_args()
    contest_id = validate_contest_id(args.contest_id)

    repo_root = Path(__file__).resolve().parent.parent
    template_file = repo_root / "library" / "template.cpp"
    target_dir = repo_root / "atcoder" / "abc" / contest_id

    if not template_file.is_file():
        print(f"Template not found: {template_file}", file=sys.stderr)
        return 1

    target_dir.mkdir(parents=True, exist_ok=True)

    for problem in ("a", "b", "c", "d", "e", "f"):
        dest = target_dir / f"{problem}.cpp"
        if dest.exists():
            print(f"Skip existing file: {dest.relative_to(repo_root)}")
            continue
        shutil.copy2(template_file, dest)
        print(f"Created {dest.relative_to(repo_root)}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
