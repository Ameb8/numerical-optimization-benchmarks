import subprocess
from pathlib import Path

from .load_config import Benchmark, Experiment

PROJECT_ROOT: Path = Path(__file__).resolve().parent.parent.parent
EXEC_PATH: Path = PROJECT_ROOT / 'bin' / 'benchmark'
RESULTS_ROOT: Path = PROJECT_ROOT / 'results'

def compile_benchmark() -> bool:
    result = subprocess.run(
        ['make'],
        cwd=PROJECT_ROOT,
        capture_output=True,
        text=True,
        check=False
    )

    # Print compilation results
    print(f'\n\nCompiling Benchmark Program:\n{result.stdout}')

    if(result.stderr): # Print errors
        print(f'Compilation Errors:\n{result.stderr}')

    return result.returncode == 0

def execute_benchmark(benchmark: Benchmark) -> bool:
    benchmark_path: Path = RESULTS_ROOT / benchmark.benchmark_name / 'benchmark.json'
    exec_command: list[str] = ['./bin/benchmark', benchmark_path, benchmark.benchmark_name]

    # DEBUG ****
    print(f'\nExecute Command: {exec_command}')

    result = subprocess.run(
        exec_command,
        cwd=PROJECT_ROOT,
        capture_output=True,
        text=True,
        check=False,
    )

    print(f'\n\nBenchmark Executed:\n{result.stdout}')

    if(result.stderr):
        print(f'\n\nError executing benchmark:\n{result.stderr}')
    
    return result.returncode == 0

def run_benchmark(benchmark: Benchmark) -> bool:
    if not compile_benchmark():
        return False
    
    return execute_benchmark(benchmark)



