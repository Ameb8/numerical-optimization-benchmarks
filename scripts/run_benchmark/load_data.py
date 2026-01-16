from pathlib import Path
import pandas as pd
import json
import numpy as np

def load_experiment_data(root_dir: Path) -> pd.DataFrame:
    # DEBUG *****
    print(f'Loading data from: {root_dir}')

    experiments: list[dict[str, Any]] = [] # Stores experiment data

    # Iterate through experiment directories
    for expr_dir in root_dir.iterdir():
        if expr_dir.is_dir(): # Ignore non-directories
            json_file: Path = expr_dir / "experiment.json"
            csv_file: Path = expr_dir / "results.csv"
        
        # Load experiment data from experiment.json
        with open(json_file) as f:
            metadata: Dict[str, Any] = json.load(f)
        
        # Load fitness values from csv
        data: np.ndarray = np.loadtxt(csv_file, delimiter=",")
        
        metadata['fitness_values'] = data # Add CSV data to metadata
        experiments.append(metadata) # Store experiment

    return pd.DataFrame(experiments) # Return as dataframe


def add_stat_cols(df: pd.DataFrame) -> pd.DataFrame:
    df = df.copy() # Create copy of dataframe

    # Add columns for useful statistics
    df["mean"] = df["fitness_values"].apply(lambda x: np.mean(x))
    df["std"] = df["fitness_values"].apply(lambda x: np.std(x, ddof=1))
    df["median"] = df["fitness_values"].apply(lambda x: np.median(x))
    df["min"] = df["fitness_values"].apply(lambda x: np.min(x))
    df["max"] = df["fitness_values"].apply(lambda x: np.max(x))
    df["range"] = df["max"] - df["min"]

    return df
