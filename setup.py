from setuptools import setup
from setuptools.command.build_py import build_py
import subprocess
import pathlib
import shutil

class BuildCppBinary(build_py):
    """Custom build step: compile C++ binary with Makefile in release mode."""

    def run(self):
        # Path to project root (where Makefile lives)
        project_root = pathlib.Path(__file__).parent

        # Ensure Makefile exists
        if not (project_root / "Makefile").exists():
            raise FileNotFoundError(f"No Makefile found in {project_root}")

        # Run make in release mode
        subprocess.check_call(["make", "MODE=release"], cwd=project_root)

        # Copy the resulting binary into the Python package
        bin_src = project_root / "bin/release/benchmark"  # Must match your Makefile TARGET
        if not bin_src.exists():
            raise FileNotFoundError(f"C++ binary not found: {bin_src}")

        # Target location inside the Python package
        pkg_dir = pathlib.Path(self.build_lib) / "run_benchmark"
        pkg_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(bin_src, pkg_dir / "benchmark")

        # Continue the normal Python build
        super().run()


setup(
    cmdclass={"build_py": BuildCppBinary},
)
