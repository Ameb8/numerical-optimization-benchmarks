import pandas as pd
import tomllib

from pathlib import Path
import argparse
import sys

from .load_data import load_benchmark_data
from .models import Benchmark, Experiment
from .run_experiments import compile_benchmark, exec_benchmark
from .build_results import build_result


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Standard Benchmark Function Optimizer Experiments"
    )

    parser.add_argument(
        "config",
        type=Path,
        nargs="?",
        default=Path("config.toml"),
        help="Path to experiment configuration file (relative to CWD)"
    )

    parser.add_argument(
        "-o", "--output",
        type=Path,
        default=Path("results"),
        help="Output directory (relative to CWD)"
    )

    return parser.parse_args()



def main():
    args: argparse.Namespace = parse_args()

    project_root: Path = Path.cwd()

    config_file: Path = args.config.resolve()
    output_dir: Path = (project_root / args.output).resolve()

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

    print(f'\n\nSuccessfully loaded {len(benchmark.experiments)} from {config_file}  \n')


    # Path to benchmark config
    benchmark_dir = output_dir / benchmark.benchmark_name
    benchmark_dir.mkdir(parents=True, exist_ok=True)
    benchmark_path = benchmark_dir / 'benchmark.json'
    

    # Write benchmark config to JSON
    benchmark_path.write_text(
        benchmark.to_json(),
        encoding='utf-8'
    )

    print(f'Experiment configurations written to {benchmark_path}')


    print(f'\nFinal experiment configuration for {benchmark.benchmark_name} written to {benchmark_path}')

    if not compile_benchmark(): # Compile benchmark program
        sys.exit(1)

    if not exec_benchmark(benchmark): # Execute benchmark program
        sys.exit(1)

    print(f'\nRaw fitness and execution time values written to {benchmark_dir / "fitness.csv"} and {benchmark_dir / "time.csv"}')
'''   
    try: # Attempt to parse and load benchmark results
        data: pd.DataFrame = load_benchmark_data(DATA_DIR / benchmark.benchmark_name)
    except (FileNotFoundError, pd.errors.ParserError, ValueError) as e:
        print(f"Error loading benchmark data: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error loading benchmark data: {e}")
        sys.exit(1)

    # Create and save plots
    build_result(data, benchmark , DATA_DIR / benchmark.benchmark_name)
'''
    

if __name__ == "__main__":
    main()