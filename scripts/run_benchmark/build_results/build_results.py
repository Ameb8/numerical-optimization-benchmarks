import pandas as pd
import numpy as np

from pathlib import Path

from ..models import Benchmark, Experiment

from . import plot_builder, build_docs


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

    plot_builder.build_stat_bar(
        function_names=function_names,
        values=times,
        save_dir=plot_dir,
        filename='all_experiments_time.png',
        title='Execution Time per Benchmark Function',
        ylabel='Execution Time (s)'
    )

def plot_fitness_violins(df: pd.DataFrame, plot_dir: Path) -> None:
    for _, row in df.iterrows():
        experiment_name = row["experiment_name"]
        fitness_values = row["fitness_values"]

        plot_builder.build_violin_plot(
            values=fitness_values,
            save_dir=plot_dir,
            filename=f"{experiment_name}_violin.png",
            title=f"{experiment_name} Fitness Distribution"
        )



def build_plots(df: pd.DataFrame, benchmark: Benchmark, result_dir: Path):
    plots_dir = result_dir / 'plots'
    plots_dir.mkdir(parents=True, exist_ok=True)
    
    # Fitness plots
    plot_fitness_histograms(df, plots_dir)
    plot_all_fitness_histogram(df, plots_dir)
    plot_fitness_violins(df, plots_dir)

    # Time plots
    plot_time_bar(df, plots_dir)

    # Create document
    build_docs.build_latex_report(result_dir)
