import numpy as np
import matplotlib.pyplot as plt

from pathlib import Path

from typing import Sequence


def fitness_box_plot(
    values: Sequence[float],
    save_dir: Path,
    filename: str = "fitness_boxplot.png",
    title: Optional[str] = "Fitness Distribution",
    figsize: tuple[int, int] = (6, 8),
) -> None:

    if not values:
        raise ValueError("`values` must not be empty.")

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