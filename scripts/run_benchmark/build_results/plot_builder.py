import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

from pathlib import Path

from typing import Optional, Sequence

def build_histogram(
    values: Sequence[float],
    save_dir: Path,
    filename: str,
    title: str,
    xlabel: str,
    color: str = 'lightgreen',
    bins: int = 10,
    figsize: tuple[int, int] = (6, 5),
) -> None:
    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    sns.set_theme(style='whitegrid', palette='pastel')
    sns.set_context('talk')

    fig, ax = plt.subplots(figsize=figsize)
    sns.histplot(
        values,
        bins=bins,
        kde=True,
        color=color,
        edgecolor='black',
        ax=ax
    )

    mean_val = np.mean(values)
    median_val = np.median(values)
    ax.axvline(mean_val, color='red', linestyle='--', linewidth=2, label=f'Mean: {mean_val:.2f}')
    ax.axvline(median_val, color='blue', linestyle='-.', linewidth=2, label=f'Median: {median_val:.2f}')
    ax.legend()

    ax.set_xlabel(xlabel, fontsize=14)
    ax.set_ylabel('Frequency', fontsize=14)
    ax.set_title(title, fontsize=16, weight='bold')
    sns.despine(trim=True)

    fig.savefig(save_path, dpi=300, bbox_inches='tight')
    plt.close(fig)





def stat_bar_across_functions_sb(
    function_names: Sequence[str],
    values: Sequence[float],
    save_dir: Path,
    filename: str = 'stat_bar_across_functions.png',
    title: Optional[str] = 'Statistic Across Benchmark Functions',
    ylabel: Optional[str] = 'Value',
    figsize: tuple[int, int] = (12, 6),
    log_scale: bool = False,
) -> None:
    save_dir.mkdir(parents=True, exist_ok=True)
    save_path = save_dir / filename

    sns.set_theme(style='whitegrid', palette='pastel')
    sns.set_context('talk')

    x = np.arange(len(function_names))

    fig, ax = plt.subplots(figsize=figsize)

    ax.bar(
        x,
        values,
        color=sns.color_palette('pastel', len(function_names)),
        edgecolor='black',
    )

    ax.set_xticks(x)
    ax.set_xticklabels(function_names, rotation=45, ha='right')
    ax.set_ylabel(ylabel, fontsize=14)
    ax.set_xlabel('Benchmark Function', fontsize=14)
    ax.set_title(title, fontsize=16, weight='bold')

    # Annotate values on top of bars
    for i, val in enumerate(values):
        ax.text(
            i,
            val,
            f'{val:.2e}',
            ha='center',
            va='bottom',
            fontsize=10,
            rotation=90,
        )

    if log_scale:
        ax.set_yscale('log')

    sns.despine(trim=True)

    fig.savefig(save_path, dpi=300, bbox_inches='tight')
    plt.close(fig)


