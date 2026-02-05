import subprocess
from pathlib import Path
import importlib.resources as res

def run_benchmark(config_path: Path, output_dir: Path) -> bool:
    with res.path("run_benchmark", "benchmark") as exe:
        # Ensure output directory exists
        output_dir.parent.mkdir(parents=True, exist_ok=True)

        cmd = [str(exe), str(config_path), str(output_dir)]
        subprocess.run(cmd, check=True)
    
