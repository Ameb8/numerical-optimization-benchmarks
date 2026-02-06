import pandas as pd
import numpy as np

from pathlib import Path
import json
from typing import Union

from .problems import ProblemType


def load_experiment_metadata(json_path: Union[str, Path]) -> pd.DataFrame:
    """
    Load experiment metadata from a JSON file and return a DataFrame with
    columns: experiment, dimensions, problem_type, optimizer_type
    """
    with open(json_path, "r") as f:
        data = json.load(f)

    rows = []
    for exp in data["experiments"]:
        experiment_full = exp["experiment_name"]
        experiment = experiment_full.rsplit("_seed", 1)[0]  # remove seed
        rows.append({
            "experiment": experiment,
            "dimensions": exp.get("dimensions"),
            "problem_type": exp.get("problem_type"),
            "optimizer_type": exp.get("optimizer", {}).get("type")
        })

    return pd.DataFrame(rows).drop_duplicates(subset="experiment")  # one row per experiment


def resample_curve(
    curve: np.ndarray,
    target_iters: int = 30,
) -> np.ndarray:
    if len(curve) == target_iters + 1:
        return curve

    old_x = np.linspace(0, target_iters, num=len(curve))
    new_x = np.linspace(0, target_iters, num=target_iters + 1)

    return np.interp(new_x, old_x, curve)


def add_normalized_curves(
    df: pd.DataFrame,
    target_iters: int = 30,
) -> pd.DataFrame:
    df = df.copy()

    df["fitness_curve_norm"] = df["fitness_curve_mean"].apply(
        lambda c: resample_curve(c, target_iters)
    )

    return df


def load_benchmark_data(
    fitness_csv: Union[str, Path],
    time_csv: Union[str, Path],
    metadata_json: Union[str, Path],
) -> pd.DataFrame:
    """
    Load experiment fitness trajectories and execution times, average results
    across seeds, and return a single DataFrame.

    Output columns:
        experiment
        fitness_curve_mean : np.ndarray
        best_mean, best_std
        final_mean, final_std
        n_seeds
        execution_time
    """

    # Load fitness curves (one array per seed)
    rows: list[dict[str, object]] = []

    with open(fitness_csv, "r") as f:
        for line in f:
            parts = line.strip().split(",")
            experiment_full = parts[0]
            curve = np.asarray(parts[1:], dtype=float)

            experiment = experiment_full.rsplit("_seed", 1)[0]

            rows.append(
                {
                    "experiment": experiment,
                    "fitness_curve": curve,
                }
            )

    fitness_df = pd.DataFrame(rows)

    # Calculate per-seed scalar statistics 
    fitness_df["best"] = fitness_df["fitness_curve"].apply(np.min)
    fitness_df["final"] = fitness_df["fitness_curve"].apply(lambda x: x[-1])

    # Curve averaging helper 
    def mean_curve(curves: pd.Series) -> np.ndarray:
        max_len = max(len(c) for c in curves)

        padded = np.vstack(
            [
                np.pad(c, (0, max_len - len(c)), constant_values=np.nan)
                for c in curves
            ]
        )

        return np.nanmean(padded, axis=0)

    # Aggregate per experiment 
    fitness_agg = (
        fitness_df
        .groupby("experiment")
        .agg(
            fitness_curve_mean=("fitness_curve", mean_curve),
            best_mean=("best", "mean"),
            best_std=("best", "std"),
            final_mean=("final", "mean"),
            final_std=("final", "std"),
            n_seeds=("fitness_curve", "count"),
        )
        .reset_index()
    )

    # Load execution times
    time_df = pd.read_csv(time_csv)
    time_df.columns = ["experiment_full", "execution_time"]

    time_df["experiment"] = (
        time_df["experiment_full"]
        .str.replace(r"_seed\d+$", "", regex=True)
    )

    time_avg = (
        time_df
        .groupby("experiment", as_index=False)["execution_time"]
        .mean()
    )

    # Merge dataframes 
    result = pd.merge(
        fitness_agg,
        time_avg,
        on="experiment",
        how="left",
    )

    # Merge experiment metadata
    metadata_df = load_experiment_metadata(metadata_json)
    result = pd.merge(result, metadata_df, on="experiment", how="left")

    # Add problem name column
    result["problem_name"] = result["problem_type"].map(
        lambda v: ProblemType(v).label if pd.notna(v) else None
    )

    # Add normalized fitness curve
    result = add_normalized_curves(result, 30)

    return result
