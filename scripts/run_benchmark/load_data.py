from pathlib import Path
import pandas as pd
import json
import numpy as nd

def load_experiment_data(root_dir: Path) -> pd.DataFrame:
    experiments: list[dict[str, Any]] = [] # Stores experiment data

    # Iterate through experiment directories
    for expr_dir in root_dir.iterdir():
        if exp_dir.is_dir(): # Ignore non-directories
            json_file: Path = exp_dir / "experiment.json"
            csv_file: Path = exp_dir / "results.csv"
        
        # Load experiment data from experiment.json
        with open(json_file) as f:
            metadata: Dict[str, Any] = json.load(f)
        
        # Load fitness values from csv
        data: np.ndarray = np.loadtxt(csv_file, delimiter=",")
        
        metadata['fitness_values'] = data # Add CSV data to metadata
        experiments.append(metadata) # Store experiment

    return pd.DataFrame(experiments) # Return as dataframe