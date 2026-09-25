#!/usr/bin/env python3

"""
Plot the sampling-demonstration data with matplotlib.

This script is intentionally simple and beginner-friendly:

- it reads plain CSV files produced by the C programs
- it creates the same figures as the gnuplot workflow
- it demonstrates a minimal reproducible plotting script in Python

Because the figures are generated from files rather than from in-memory objects,
the plotting step is fully decoupled from the numerical simulation step. That is
often a good habit in research workflows.
"""

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUTPUT_DIR = ROOT / "output"
PLOT_DIR = ROOT / "plots"

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np


def read_csv_columns(path: Path) -> tuple[list[float], list[float]]:
    """Read a two-column CSV file after skipping its header row."""
    x_values: list[float] = []
    y_values: list[float] = []

    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.reader(handle)
        next(reader)
        for row in reader:
            x_values.append(float(row[0]))
            y_values.append(float(row[1]))

    return x_values, y_values


def plot_sampling_signals() -> None:
    """Plot the time-domain signals for the three sampling experiments."""
    cases = [
        (
            "good_sampling_signal.csv",
            "Good sampling: 50 Hz and 120 Hz resolved correctly",
        ),
        (
            "undersampled_signal.csv",
            "Undersampled: 120 Hz aliases to about 8 Hz",
        ),
        (
            "short_record_signal.csv",
            "Short record: nearby frequencies cannot be separated well",
        ),
    ]

    fig, axes = plt.subplots(3, 1, figsize=(14, 9), constrained_layout=True)
    fig.suptitle("Sampling demo: time-domain signals")

    for axis, (filename, title) in zip(axes, cases):
        time_s, signal = read_csv_columns(OUTPUT_DIR / filename)
        axis.plot(time_s, signal, linewidth=1.8)
        axis.set_title(title)
        axis.set_xlabel("Time (s)")
        axis.set_ylabel("Signal")
        axis.grid(True, alpha=0.3)

    fig.savefig(PLOT_DIR / "sampling_signals.png", dpi=150)
    plt.close(fig)


def plot_sampling_spectra() -> None:
    """Plot the corresponding one-sided FFT amplitude spectra."""
    cases = [
        (
            "good_sampling_spectrum.csv",
            "Good sampling spectrum",
            (0.0, 200.0),
        ),
        (
            "undersampled_spectrum.csv",
            "Undersampled spectrum",
            (0.0, 80.0),
        ),
        (
            "short_record_spectrum.csv",
            "Short-record spectrum",
            (0.0, 120.0),
        ),
    ]

    fig, axes = plt.subplots(3, 1, figsize=(14, 9), constrained_layout=True)
    fig.suptitle("Sampling demo: FFT amplitude spectra")

    for axis, (filename, title, x_limits) in zip(axes, cases):
        frequency_hz, amplitude = read_csv_columns(OUTPUT_DIR / filename)
        axis.vlines(frequency_hz, 0.0, amplitude, linewidth=1.4)
        axis.set_title(title)
        axis.set_xlim(*x_limits)
        axis.set_xlabel("Frequency (Hz)")
        axis.set_ylabel("Amplitude")
        axis.grid(True, alpha=0.3)

    fig.savefig(PLOT_DIR / "sampling_spectra.png", dpi=150)
    plt.close(fig)


def plot_frequency_resolution_comparison() -> None:
    """Compare the original short record with the longer Part 7 record."""
    sample_rate_hz = 512.0
    cases = [
        (64, "Original: N = 64, T = 0.125 s, df = 8 Hz"),
        (512, "Part 7: N = 512, T = 1 s, df = 1 Hz"),
    ]

    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=True, constrained_layout=True)
    fig.suptitle("Part 7: a longer observation resolves 50 Hz and 55 Hz")

    for axis, (sample_count, title) in zip(axes, cases):
        times = np.arange(sample_count) / sample_rate_hz
        samples = np.sin(2.0 * np.pi * 50.0 * times)
        samples += 0.85 * np.sin(2.0 * np.pi * 55.0 * times)
        samples -= np.mean(samples)

        frequencies = np.fft.rfftfreq(sample_count, d=1.0 / sample_rate_hz)
        amplitudes = np.abs(np.fft.rfft(samples)) / sample_count
        amplitudes[1:-1] *= 2.0

        axis.vlines(frequencies, 0.0, amplitudes, linewidth=1.8, label="FFT bins")
        axis.axvline(50.0, color="tab:red", linestyle="--", label="True: 50 Hz")
        axis.axvline(55.0, color="tab:green", linestyle="--", label="True: 55 Hz")
        axis.set_title(title)
        axis.set_xlim(35.0, 70.0)
        axis.set_xlabel("Frequency (Hz)")
        axis.set_ylabel("Amplitude")
        axis.grid(True, alpha=0.3)
        axis.legend()

    fig.savefig(PLOT_DIR / "part7_frequency_resolution.png", dpi=150)
    plt.close(fig)


def main() -> None:
    """Create the output directory and generate both sampling figures."""
    PLOT_DIR.mkdir(exist_ok=True)
    plot_sampling_signals()
    plot_sampling_spectra()
    plot_frequency_resolution_comparison()


if __name__ == "__main__":
    main()
