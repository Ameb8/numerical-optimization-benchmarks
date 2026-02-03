import subprocess
from pathlib import Path

from .models import Benchmark

PROJECT_ROOT: Path = Path.cwd()
EXEC_PATH: Path = PROJECT_ROOT / 'bin' / 'release' / 'benchmark'
RESULTS_ROOT: Path = PROJECT_ROOT / 'results'

def compile_benchmark() -> bool:
    try:
        result = subprocess.run(
            ['make'],
            cwd=PROJECT_ROOT,
            text=True,
            check=False
        )
    except FileNotFoundError:
        print("Error: 'make' not found. Ensure it is installed and on PATH.")
        return False
    except PermissionError:
        print("Error: Permission denied when running 'make'.")
        return False
    except Exception as e:
        print(f"Unexpected error during compilation: {e}")
        return False

    # Print compilation results
    print('\n\nCompiling Benchmark Program' + (f':\n{result.stdout}\n' if result.stdout else "\n"))

    if(result.stderr): # Print errors
        print(f'Compilation Errors:\n{result.stderr}')

    return result.returncode == 0

def exec_benchmark(benchmark: Benchmark, config_path: Path, output_dir: Path) -> bool:
    exec_command: list[str] = ['./bin/release/benchmark', str(config_path), str(output_dir)]

    try:
        result = subprocess.run(
            exec_command,
            cwd=PROJECT_ROOT,
            text=True,
            check=False,
        )
    except FileNotFoundError:
        print(f"Error: Executable './bin/benchmark' not found. Did you compile it?")
        return False
    except PermissionError:
        print("Error: Permission denied when executing './bin/benchmark'.")
        return False
    except Exception as e:
        print(f"Unexpected error executing benchmark: {e}")
        return False

    print(f'\n\nBenchmark Executed:\n{result.stdout}')

    if(result.stderr):
        print(f'\n\nError executing benchmark:\n{result.stderr}')
    
    return result.returncode == 0

def run_benchmark(benchmark: Benchmark, config_path: Path, output_dir: Path) -> bool:
    if not compile_benchmark():
        return False
    
    return exec_benchmark(benchmark)



