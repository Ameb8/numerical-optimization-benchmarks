from pathlib import Path
import pandas as pd
import numpy as np


def load_benchmark_data(result_dir: Path) -> pd.DataFrame:
    fitness_file: Path = result_dir / 'fitness.csv'
    time_file: Path = result_dir / 'time.csv'

        # Check that files exist
    if not fitness_file.exists():
        raise FileNotFoundError(f"Missing fitness CSV: {fitness_file}")
    if not time_file.exists():
        raise FileNotFoundError(f"Missing time CSV: {time_file}")

    fitness_df: pd.DataFrame = pd.read_csv(result_dir / 'fitness.csv')
    time_df: pd.DataFrame = pd.read_csv(result_dir / 'time.csv')

    # Check that CSVs are not empty
    if fitness_df.empty:
        raise ValueError(f"fitness.csv is empty: {fitness_file}")
    if time_df.empty:
        raise ValueError(f"time.csv is empty: {time_file}")

    # Transpose fitness_df so experiments are rows
    fitness_transposed = fitness_df.T
    fitness_transposed.columns = [f"run_{i+1}" for i in range(fitness_transposed.shape[1])]
    fitness_transposed.index.name = "experiment_name"
    fitness_transposed.reset_index(inplace=True)

    # Combine all run columns into a single numpy array column
    fitness_transposed["fitness_values"] = fitness_transposed.iloc[:, 1:].apply(lambda row: row.to_numpy(dtype=np.float64), axis=1)

    # Keep only experiment_name and fitness_values
    fitness_transposed = fitness_transposed[["experiment_name", "fitness_values"]]

    # Merge Dataframes
    processed_df = pd.merge(fitness_transposed, time_df, on="experiment_name")

    # Add summary statistics
    processed_df["mean"] = processed_df["fitness_values"].apply(np.mean)
    processed_df["std"] = processed_df["fitness_values"].apply(np.std)
    processed_df["median"] = processed_df["fitness_values"].apply(np.median)
    processed_df["min"] = processed_df["fitness_values"].apply(np.min)
    processed_df["max"] = processed_df["fitness_values"].apply(np.max)
    processed_df["range"] = processed_df["max"] - processed_df["min"]

    return processed_df