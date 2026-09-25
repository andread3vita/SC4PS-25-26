#!/usr/bin/env python3
"""Plot Legendre recurrence errors stored in legendre_comparison.csv."""

import argparse
import csv
import math
import sys
from collections import defaultdict
from pathlib import Path


ERROR_COLUMNS = {
    "relative": {
        "forward": "forward_relative_error",
        "backward": "backward_relative_error",
    },
    "absolute": {
        "forward": "forward_absolute_error",
        "backward": "backward_absolute_error",
    },
}

REQUIRED_COLUMNS = {
    "x",
    "l",
    "high_precision_value",
    "forward_value",
    "backward_value",
    *ERROR_COLUMNS["relative"].values(),
    *ERROR_COLUMNS["absolute"].values(),
}


def parse_arguments():
    parser = argparse.ArgumentParser(
        description=(
            "Plot forward- and backward-recurrence errors versus degree l, "
            "with one panel for each x value."
        )
    )
    parser.add_argument(
        "csv_file",
        nargs="?",
        type=Path,
        default=Path("legendre_comparison.csv"),
        help="input CSV file (default: legendre_comparison.csv)",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("plots"),
        help="directory for generated plots (default: plots)",
    )
    parser.add_argument(
        "--format",
        choices=("png", "pdf", "svg"),
        default="png",
        help="output image format (default: png)",
    )
    parser.add_argument(
        "--dpi",
        type=int,
        default=180,
        help="resolution for raster output (default: 180)",
    )
    parser.add_argument(
        "--show",
        action="store_true",
        help="also display the figures interactively",
    )
    return parser.parse_args()


def load_data(csv_file):
    """Return rows grouped by x, ordered by increasing l."""
    grouped = defaultdict(list)

    try:
        stream = csv_file.open(newline="", encoding="utf-8")
    except OSError as error:
        raise RuntimeError(f"cannot open {csv_file}: {error}") from error

    with stream:
        reader = csv.DictReader(stream)
        available = set(reader.fieldnames or ())
        missing = sorted(REQUIRED_COLUMNS - available)
        if missing:
            raise RuntimeError(
                f"{csv_file} is missing columns: {', '.join(missing)}"
            )

        for line_number, row in enumerate(reader, start=2):
            try:
                parsed = {"l": int(row["l"])}
                for name in REQUIRED_COLUMNS - {"l"}:
                    parsed[name] = float(row[name])
            except (TypeError, ValueError) as error:
                raise RuntimeError(
                    f"invalid numeric value in {csv_file}, line {line_number}"
                ) from error
            grouped[parsed["x"]].append(parsed)

    if not grouped:
        raise RuntimeError(f"{csv_file} contains no data rows")

    for rows in grouped.values():
        rows.sort(key=lambda row: row["l"])

    return dict(sorted(grouped.items()))


def finite_or_nan(value):
    """Break plotted lines at CSV nan/inf values."""
    return value if math.isfinite(value) else math.nan


def plot_error(plt, grouped, error_kind, output_file, dpi):
    x_values = list(grouped)
    column_count = min(3, len(x_values))
    row_count = math.ceil(len(x_values) / column_count)

    figure, axes = plt.subplots(
        row_count,
        column_count,
        figsize=(5.0 * column_count, 3.6 * row_count),
        sharex=True,
        sharey=True,
        squeeze=False,
    )

    styles = {
        "forward": {"color": "#1f77b4", "marker": "o"},
        "backward": {"color": "#d62728", "marker": "s"},
    }

    for axis, x in zip(axes.flat, x_values):
        rows = grouped[x]
        degrees = [row["l"] for row in rows]

        for method, column in ERROR_COLUMNS[error_kind].items():
            errors = [finite_or_nan(row[column]) for row in rows]
            axis.plot(
                degrees,
                errors,
                label=method.capitalize(),
                color=styles[method]["color"],
                marker=styles[method]["marker"],
                markersize=2.8,
                linewidth=1.1,
            )

        # symlog keeps exact zero errors visible while retaining logarithmic
        # resolution for the many nonzero error scales.
        axis.set_yscale("symlog", linthresh=1.0e-18, linscale=0.7)
        axis.set_title(f"x = {x:.8g}")
        axis.set_xlabel("Legendre degree l")
        axis.set_ylabel(f"{error_kind.capitalize()} error")
        axis.set_xlim(0, 50)
        axis.grid(True, which="both", linestyle=":", linewidth=0.6, alpha=0.75)

    for axis in list(axes.flat)[len(x_values):]:
        axis.set_visible(False)

    handles, labels = axes.flat[0].get_legend_handles_labels()
    # ``outside upper center`` is only available in newer Matplotlib releases.
    # The explicit anchor keeps the script compatible with older installations.
    figure.legend(
        handles,
        labels,
        loc="upper center",
        bbox_to_anchor=(0.5, 0.945),
        ncol=2,
    )
    figure.suptitle(
        f"Legendre recurrence {error_kind} error versus degree",
        fontsize=14,
        y=0.995,
    )
    figure.tight_layout(rect=(0.0, 0.0, 1.0, 0.90))
    figure.savefig(output_file, dpi=dpi)
    return figure


def main():
    args = parse_arguments()

    try:
        import matplotlib.pyplot as plt
    except ModuleNotFoundError:
        print(
            "Matplotlib is required. Install it in a local environment with:\n"
            "  python3 -m venv .plot-venv\n"
            "  source .plot-venv/bin/activate\n"
            "  python -m pip install matplotlib",
            file=sys.stderr,
        )
        return 2

    try:
        grouped = load_data(args.csv_file)
    except RuntimeError as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1

    args.output_dir.mkdir(parents=True, exist_ok=True)
    figures = []
    for error_kind in ("relative", "absolute"):
        output_file = args.output_dir / (
            f"legendre_{error_kind}_error.{args.format}"
        )
        figures.append(
            plot_error(plt, grouped, error_kind, output_file, args.dpi)
        )
        print(f"Saved {output_file}")

    if args.show:
        plt.show()
    else:
        for figure in figures:
            plt.close(figure)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
