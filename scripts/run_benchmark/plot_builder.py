import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

from pathlib import Path

from typing import Optional, Sequence


def fitness_box_plot(
    values: Sequence[float],
    save_dir: Path,
    filename: str = "fitness_boxplot.png",
    title: Optional[str] = "Fitness Distribution",
    figsize: tuple[int, int] = (6, 8),
) -> None:
    
    # Ensure directory exists
    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    # Create figure
    fig, ax = plt.subplots(figsize=figsize)
    ax.boxplot(values, showmeans=True)
    ax.set_ylabel("Fitness Value")
    ax.set_title(title)

    # Save and close
    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    plt.close(fig)

def fitness_histogram(
    values: Sequence[float],
    save_dir: Path,
    filename: str = "fitness_histogram.png",
    title: Optional[str] = "Fitness Histogram",
    bins: int = 10,
    figsize: tuple[int, int] = (6, 5),
) -> None:

    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    fig, ax = plt.subplots(figsize=figsize)
    ax.hist(values, bins=bins, edgecolor="black", alpha=0.7)
    ax.set_xlabel("Fitness Value")
    ax.set_ylabel("Frequency")
    ax.set_title(title)

    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    plt.close(fig)


def mean_std_plot(
    mean: float,
    std: float,
    save_dir: Path,
    filename: str = "mean_std.png",
    title: Optional[str] = "Mean ± Std",
    figsize: tuple[int, int] = (4, 6),
) -> None:

    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    fig, ax = plt.subplots(figsize=figsize)
    ax.bar([0], [mean], yerr=[std], capsize=10)
    ax.set_xticks([0])
    ax.set_xticklabels(["Fitness"])
    ax.set_ylabel("Fitness Value")
    ax.set_title(title)

    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    plt.close(fig)


def fitness_box_plot_sb(
    values: Sequence[float],
    save_dir: Path,
    filename: str = "fitness_boxplot_sb.png",
    title: Optional[str] = "Fitness Distribution",
    figsize: tuple[int, int] = (6, 8),
) -> None:

    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    sns.set_theme(style="whitegrid", palette="pastel")
    sns.set_context("talk")

    fig, ax = plt.subplots(figsize=figsize)
    sns.boxplot(
        y=values, 
        ax=ax, 
        showmeans=True, 
        meanprops={"marker":"o", "markerfacecolor":"red", "markeredgecolor":"black"},
        color="skyblue",
        width=0.5
    )
    
    # Optional: annotate median
    median_val = np.median(values)
    ax.text(0, median_val, f"{median_val:.2f}", ha='center', va='bottom', fontsize=12, color='black')

    ax.set_ylabel("Fitness Value", fontsize=14)
    ax.set_title(title, fontsize=16, weight='bold')
    sns.despine(trim=True)

    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    plt.close(fig)


def fitness_histogram_sb(
    values: Sequence[float],
    save_dir: Path,
    filename: str = "fitness_histogram_sb.png",
    title: Optional[str] = "Fitness Histogram",
    bins: int = 10,
    figsize: tuple[int, int] = (6, 5),
) -> None:

    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    sns.set_theme(style="whitegrid", palette="pastel")
    sns.set_context("talk")

    fig, ax = plt.subplots(figsize=figsize)
    sns.histplot(
        values, 
        bins=bins, 
        kde=True,          # add density curve
        color="lightgreen", 
        edgecolor="black", 
        ax=ax
    )

    # Annotate mean and median
    mean_val = np.mean(values)
    median_val = np.median(values)
    ax.axvline(mean_val, color='red', linestyle='--', linewidth=2, label=f'Mean: {mean_val:.2f}')
    ax.axvline(median_val, color='blue', linestyle='-.', linewidth=2, label=f'Median: {median_val:.2f}')
    ax.legend()

    ax.set_xlabel("Fitness Value", fontsize=14)
    ax.set_ylabel("Frequency", fontsize=14)
    ax.set_title(title, fontsize=16, weight='bold')
    sns.despine(trim=True)

    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    plt.close(fig)
