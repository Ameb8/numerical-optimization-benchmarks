import pandas as pd

from pathlib import Path
import sys

from .load_data import load_benchmark_data
from .models import Benchmark, Experiment
from .run_experiments import compile_benchmark, exec_benchmark
from .build_results import build_result


# Paths to project directories
PROJECT_ROOT: Path = Path(__file__).resolve().parent.parent.parent
DEFAULT_CONFIG: Path = PROJECT_ROOT / 'config.toml'
DATA_DIR: Path = PROJECT_ROOT / 'results'




def main():
    config_file: Path = DEFAULT_CONFIG
    
    if len(sys.argv) > 1:
        config_file = (PROJECT_ROOT / sys.argv[1]).resolve()

    try:
        # Load benchmark config
        benchmark: Benchmark = Benchmark.from_toml(config_file)
    except FileNotFoundError:
        print(f"Error: Config file not found: {config_file}")
        sys.exit(1)

    except tomllib.TOMLDecodeError as e:
        print(f"Error: Failed to parse TOML file {config_file}:\n{e}")
        sys.exit(1)

    except Exception as e:
        print(f"Unexpected error loading benchmark config: {e}")
        sys.exit(1)


    # Path to benchmark config
    benchmark_path = DATA_DIR / benchmark.benchmark_name / 'benchmark.json'
    benchmark_path.parent.mkdir(parents=True, exist_ok=True)

    # Write benchmark config to JSON
    benchmark_path.write_text(
        benchmark.model_dump_json(indent=2),
        encoding='utf-8'
    )

    print(f'\nFinal experiment configuration for {benchmark.benchmark_name} written to {benchmark_path}')

    if not compile_benchmark(): # Compile benchmark program
        sys.exit(1)

    if not exec_benchmark(benchmark): # Execute benchmark program
        sys.exit(1)
        
    try: # Attempt toarse and load benchmark results
        data: pd.DataFrame = load_benchmark_data(DATA_DIR / benchmark.benchmark_name)
    except (FileNotFoundError, pd.errors.ParserError, ValueError) as e:
        print(f"Error loading benchmark data: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error loading benchmark data: {e}")
        sys.exit(1)

    # Create and save plots
    build_result(data, benchmark , DATA_DIR / benchmark.benchmark_name)


if __name__ == "__main__":
    main()