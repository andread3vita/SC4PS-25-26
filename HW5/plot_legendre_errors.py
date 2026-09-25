#!/usr/bin/env python3

import sys

import matplotlib.pyplot as plt
import numpy as np


if len(sys.argv) != 2:
    raise SystemExit(f"Usage: python {sys.argv[0]} input.txt")

data = np.loadtxt(sys.argv[1])
x_values = np.unique(data[:, 0])

for name, forward_column, backward_column in (
    ("absolute", 2, 4),
    ("relative", 3, 5),
):
    figure, axes = plt.subplots(2, 2, figsize=(10, 7), sharex=True, sharey=True)

    for axis, x in zip(axes.flat, x_values):
        rows = data[data[:, 0] == x]
        axis.plot(rows[:, 1], rows[:, forward_column], label="Forward")
        axis.plot(rows[:, 1], rows[:, backward_column], label="Backward")
        axis.set(title=f"x = {x:g}", xlabel="Degree l", ylabel=f"{name.title()} error")
        axis.set_yscale("symlog", linthresh=1e-18)
        axis.grid(True, which="both", linestyle=":")

    figure.legend(*axes.flat[0].get_legend_handles_labels(), loc="upper center", ncol=2)
    figure.tight_layout(rect=(0, 0, 1, 0.95))
    figure.savefig(f"legendre_{name}_error.png", dpi=180)
    plt.close(figure)
