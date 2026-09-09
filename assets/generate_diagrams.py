"""Generates the reference diagrams used in README.md.

Run with: python assets/generate_diagrams.py
Requires matplotlib. Output PNGs are checked into assets/ so the README
renders on GitHub without anyone having to run this script.
"""

import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({
    "figure.facecolor": "#0d1117",
    "axes.facecolor": "#0d1117",
    "axes.edgecolor": "#8b949e",
    "axes.labelcolor": "#c9d1d9",
    "xtick.color": "#c9d1d9",
    "ytick.color": "#c9d1d9",
    "text.color": "#c9d1d9",
    "grid.color": "#30363d",
    "font.size": 11,
})

FUNCTIONS = [
    ("sin", np.sin, "#58a6ff", (-3, 3)),
    ("cos", np.cos, "#3fb950", (-3, 3)),
    ("tan", np.tan, "#f778ba", (-5, 5)),
    ("csc (1/sin)", lambda x: 1 / np.sin(x), "#d29922", (-5, 5)),
    ("sec (1/cos)", lambda x: 1 / np.cos(x), "#f85149", (-5, 5)),
    ("cot (1/tan)", lambda x: 1 / np.tan(x), "#a371f7", (-5, 5)),
]


def plot_overview():
    fig, axes = plt.subplots(2, 3, figsize=(13, 7))
    x = np.linspace(-np.pi, np.pi, 2000)
    for ax, (name, fn, color, ylim) in zip(axes.flat, FUNCTIONS):
        y = fn(x)
        # Break the line at asymptotes so matplotlib doesn't draw verticals.
        y_masked = np.where(np.abs(y) > ylim[1] * 1.5, np.nan, y)
        ax.axhline(0, color="#30363d", linewidth=1)
        ax.axvline(0, color="#30363d", linewidth=1)
        ax.plot(x, y_masked, color=color, linewidth=2)
        ax.set_title(name, color=color, fontweight="bold")
        ax.set_xlim(-np.pi, np.pi)
        ax.set_ylim(*ylim)
        ax.set_xticks([-np.pi, -np.pi / 2, 0, np.pi / 2, np.pi])
        ax.set_xticklabels(["-π", "-π/2", "0", "π/2", "π"])
        ax.grid(alpha=0.3)
    fig.suptitle("The Six Trigonometric Functions", fontsize=16, color="#f0f6fc", fontweight="bold")
    fig.tight_layout(rect=[0, 0, 1, 0.95])
    fig.savefig("assets/functions_overview.png", dpi=150)
    plt.close(fig)


def plot_unit_circle():
    fig, ax = plt.subplots(figsize=(6, 6))
    theta = np.linspace(0, 2 * np.pi, 400)
    ax.plot(np.cos(theta), np.sin(theta), color="#58a6ff", linewidth=2)
    ax.axhline(0, color="#30363d", linewidth=1)
    ax.axvline(0, color="#30363d", linewidth=1)

    angle = np.pi / 4
    px, py = np.cos(angle), np.sin(angle)
    ax.plot([0, px], [0, py], color="#f778ba", linewidth=2)
    ax.plot([px, px], [0, py], color="#3fb950", linewidth=1.5, linestyle="--")
    ax.plot([0, px], [0, 0], color="#d29922", linewidth=1.5, linestyle="--")
    ax.scatter([px], [py], color="#f778ba", zorder=5)
    ax.annotate("(cos θ, sin θ)", (px, py), textcoords="offset points",
                xytext=(10, 8), color="#f0f6fc")
    ax.annotate("θ", (0.18, 0.08), color="#f0f6fc", fontsize=14)

    ax.set_aspect("equal")
    ax.set_xlim(-1.3, 1.3)
    ax.set_ylim(-1.3, 1.3)
    ax.set_title("Unit Circle Definition", color="#f0f6fc", fontweight="bold")
    ax.grid(alpha=0.3)
    fig.tight_layout()
    fig.savefig("assets/unit_circle.png", dpi=150)
    plt.close(fig)


if __name__ == "__main__":
    plot_overview()
    plot_unit_circle()
    print("Wrote assets/functions_overview.png and assets/unit_circle.png")
