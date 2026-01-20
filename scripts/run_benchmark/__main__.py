import pandas as pd

from pathlib import Path
import sys

from .load_data import load_benchmark_data
from .models import Benchmark, Experiment
from .run_experiments import run_benchmark
from .build_results.build_plots import build_plots


# Paths to project directories
PROJECT_ROOT: Path = Path(__file__).resolve().parent.parent.parent
DEFAULT_CONFIG: Path = PROJECT_ROOT / 'config.toml'
DATA_DIR: Path = PROJECT_ROOT / 'results'




def main():
    config_file: Path = DEFAULT_CONFIG
    
    if len(sys.argv) > 1:
        config_file = sys.argv[1]

    # Load benchmark config
    benchmark: Benchmark = Benchmark.from_toml(config_file)

    # Path to benchmark config
    benchmark_path = DATA_DIR / benchmark.benchmark_name / 'benchmark.json'
    benchmark_path.parent.mkdir(parents=True, exist_ok=True)

    # Write benchmark config to JSON
    benchmark_path.write_text(
        benchmark.model_dump_json(indent=2),
        encoding='utf-8'
    )

    run_benchmark(benchmark) # Compile and run benchmark program
    
    # Parse and load benchmark results
    data: pd.DataFrame = load_benchmark_data(DATA_DIR / benchmark.benchmark_name)

    # Create and save plots
    build_plots(data, benchmark , DATA_DIR / benchmark.benchmark_name)


if __name__ == "__main__":
    main()