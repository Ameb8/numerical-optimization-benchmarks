import pandas as pd

from pathlib import Path
import sys

from .load_data import load_experiment_data
from .plot_builder import fitness_box_plot

# Paths to project directories
PROJECT_ROOT: Path = Path(__file__).resolve().parent.parent.parent
DATA_DIR: Path = PROJECT_ROOT / "results"

# File names for experiment data
EXP_RESULT: Path = Path("results.csv")
EXP_INFO: Path = Path("experiment.json")


def plot_results(result_dir: Path):
    experiment_results: pd.DataFrame = load_experiment_data(result_dir)

    # Iterate through experiments
    for _, row in experiment_results.iterrows():
        # Create fitness values box plot
        experiment_name: str = str(row['experiment'])
        save_path: Path = result_dir / experiment_name 
        title: str = f'{str(row["experiment"])} fitness values'
        fitness_box_plot(row['fitness_values'], save_path, title=title)
        


def main():
    if len(sys.argv) < 1: # Ensure command line arg provided
        print("No path to result directory provided")
        sys.exit(1)

    experiment_root = DATA_DIR / sys.argv[1] # Get experiment directory

    plot_results(experiment_root) # Plot data


if __name__ == "__main__":
    main()