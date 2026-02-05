import pandas as pd
import tomllib

from pathlib import Path
import argparse
import sys

from .load_data import load_benchmark_data
from .models import Benchmark, Experiment
from .run_experiments import run_benchmark
from .build_results import build_result


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Standard Benchmark Function Optimizer Experiments"
    )

    # Positional argument for experiment config file (defaults to cwd/config.toml)
    parser.add_argument(
        "config",
        nargs="?",
        type=Path,
        default=Path("config.toml"),
        help="Path to experiment configuration file (relative to CWD)"
    )

    # Argument to prevent running of experiment, only produce analysis of already-existent results
    group = parser.add_mutually_exclusive_group()
    group.add_argument(
        "-a", "--analyze",
        type=Path,
        metavar="PATH",
        help="Analyze pre-existing data at PATH (do not run experiment)"
    )


    # Determine path to output directory for metadata, raw results, figures, and plots
    parser.add_argument(
        "-o", "--output",
        type=Path,
        default=Path("results"),
        help="Output directory (relative to CWD)"
    )

    # Do not produce plots/documents, only raw data and full experiment configuration file
    parser.add_argument(
        "--run-only",
        action="store_true",
        help="Run experiments up to raw data generation (do not build plots)"
    )

    return parser.parse_args()



def load_config(config_file: Path) -> Benchmark:
    if not config_file.exists():
        sys.exit(f"Error: Config file not found: {config_file}")

    try:
        # Load benchmark config
        benchmark: Benchmark = Benchmark.from_toml(config_file)

    except tomllib.TOMLDecodeError as e:
        print(f"Error: Failed to parse Config file {config_file}:\n{e}")
        sys.exit(1)

    except Exception as e:
        print(f"Unexpected error loading benchmark config: {e}")
        sys.exit(1)

    print(f'\n\nSuccessfully loaded {len(benchmark.experiments)} experiments from {config_file}  \n')
    return benchmark


def load_result_data(benchmark_dir: Path) -> pd.DataFrame:
    try: # Attempt to parse and load benchmark results
        data: pd.DataFrame = load_benchmark_data(
            benchmark_dir / "best_fitnesses.csv",
            benchmark_dir / "times.csv",
            benchmark_dir / "benchmark.json"
        )

        print(f'\n\nData Loaded:\n\n{data.info()}\n\n{data.describe()}')
    except (FileNotFoundError, pd.errors.ParserError, ValueError) as e:
        print(f"Error loading benchmark data: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error loading benchmark data: {e}")
        sys.exit(1)

    return data

def analyze_run(path_arg: Path) -> None:
    # Attempt to load results directory
    print(f"\nLoading benchmark result data from {path_arg}")
    path_arg = path_arg.resolve(strict=False)

    # Ensure directory exists
    if not path_arg.is_dir():
        sys.exit(f"Error: Analyze path is not a directory or does not exist: {path_arg}")

    # Ensure experiment result files exists
    required_files = ["benchmark.json", "best_fitnesses.csv", "times.csv"]
    missing_files = [f for f in required_files if not (path_arg / f).exists()]

    # Display missing files
    if missing_files:
        sys.exit(f"Error: Missing required file(s) in {path_arg}: {', '.join(missing_files)}")

    # Load results int dataframe
    data = load_result_data(path_arg)

    # Build and save experiment results
    build_result(data, path_arg)
    print(f"Analysis complete for {path_arg}")



def main():
    args: argparse.Namespace = parse_args()

    if(args.analyze):
        analyze_run(args.analyze)
        sys.exit(0)

    # Load file paths from command line args
    config_file = args.config
    output_dir = args.output

    # Load config file
    benchmark: Benchmark = load_config(config_file)

    # Define directory for benchmark results and metadata
    benchmark_dir = output_dir / benchmark.benchmark_name
    benchmark_dir.mkdir(parents=True, exist_ok=True)

    # Write validated config to json
    benchmark_path = benchmark_dir / "benchmark.json"
    benchmark_path.write_text(benchmark.to_json(), encoding="utf-8")

    # Display path to validated experiment config
    print(f'\nFinal experiment configuration for {benchmark.benchmark_name} written to {benchmark_path}')

    # Execute benchmark program
    run_benchmark(benchmark_path, benchmark_dir)
    

    print( # Display paths to benchmark results
        f"Raw fitness and execution time values written to "
        f"{benchmark_dir / 'best_fitness.csv'} and {benchmark_dir / 'times.csv'}"
    )

    if args.run_only:
        print("Run-only mode enabled; skipping plot generation.")
        sys.exit(0)

    # Load results into dataframe
    data: pd.DataFrame = load_result_data(benchmark_dir)

    # Create and save plots
    build_result(data, benchmark_dir)

    

if __name__ == "__main__":
    main()