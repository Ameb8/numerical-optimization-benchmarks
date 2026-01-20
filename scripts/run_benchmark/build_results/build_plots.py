import pandas as pd
import numpy as np

from pathlib import Path

from ..models import Benchmark, Experiment

from . import plot_builder


def plot_fitness_histograms(df: pd.DataFrame, plot_dir: Path, bins: int = 10) -> None:
    for _, row in df.iterrows():
        experiment_name = row['experiment_name']
        fitness_values = row['fitness_values']

        plot_builder.build_histogram(
            values=fitness_values,
            save_dir=plot_dir,
            filename=f'{experiment_name}_histogram.png',
            title=f'{experiment_name} Fitness Histogram',
            xlabel='Fitness Value',
            color='lightgreen',
            bins=bins
        )

def plot_all_fitness_histogram(df: pd.DataFrame, plot_dir: Path, bins: int = 20) -> None:
    all_values = np.concatenate(df['fitness_values'].values)

    plot_builder.build_histogram(
        values=all_values,
        save_dir=plot_dir,
        filename='all_experiments_fitness_histogram.png',
        title='All Experiments Fitness Histogram',
        xlabel='Fitness Value',
        color='lightgreen',
        bins=bins
    )

def plot_time_bar(df: pd.DataFrame, plot_dir: Path) -> None:
    function_names = df['experiment_name'].tolist()
    times = df['wall_time'].values

    plot_builder.stat_bar_across_functions_sb(
        function_names=function_names,
        values=times,
        save_dir=plot_dir,
        filename='all_experiments_time.png',
        title='Execution Time per Benchmark Function',
        ylabel='Execution Time (s)'
    )


def build_plots(df: pd.DataFrame, benchmark: Benchmark, result_dir: Path):
    plots_dir = result_dir / 'plots'
    plots_dir.mkdir(parents=True, exist_ok=True)
    
    # Fitness
    plot_fitness_histograms(df, plots_dir)
    plot_all_fitness_histogram(df, plots_dir)

    # Time
    plot_time_bar(df, plots_dir)
