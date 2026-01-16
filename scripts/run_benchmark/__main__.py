import pandas as pd

from pathlib import Path
import sys

from .load_data import load_experiment_data, add_stat_cols
from .plot_builder import fitness_box_plot_sb, fitness_histogram_sb, mean_std_plot

# Paths to project directories
PROJECT_ROOT: Path = Path(__file__).resolve().parent.parent.parent
DATA_DIR: Path = PROJECT_ROOT / "results"

# File names for experiment data
EXP_RESULT: Path = Path("results.csv")
EXP_INFO: Path = Path("experiment.json")


def plot_results(result_dir: Path):
    # Load experiment data into dataframe
    experiment_results: pd.DataFrame = load_experiment_data(result_dir)
    experiment_results: pd.DataFrame = add_stat_cols(experiment_results)

    combined_fitness_vals = [] # Store combined fitness values

    # Iterate through experiments
    for _, row in experiment_results.iterrows():
        combined_fitness_vals.extend(row["fitness_values"])

        # Create fitness values box plot
        experiment_name: str = str(row['experiment'])
        save_dir: Path = result_dir / experiment_name 
        title: str = f'{str(row["experiment"])} fitness values'
        fitness_box_plot_sb(row['fitness_values'], save_dir, title=title)

        # Create fitness histogram plot
        fitness_histogram_sb(
            row["fitness_values"],
            save_dir,
            title=f'{experiment_name} Fitness Histogram'
        )

        mean_std_plot(
            mean=row["mean"],
            std=row["std"],
            save_dir=save_dir,
            title=f"{experiment_name} – Mean ± Std"
        )

        
        
    # Create box plot for all functions
    fitness_box_plot_sb(combined_fitness_vals, 
        result_dir,
        'combined_fitness_boxplot.png',
        'Combined fitness boxplot'
    )


def main():
    if len(sys.argv) < 1: # Ensure command line arg provided
        print("No path to result directory provided")
        sys.exit(1)

    experiment_root = DATA_DIR / sys.argv[1] # Get experiment directory

    plot_results(experiment_root) # Plot data


if __name__ == "__main__":
    main()