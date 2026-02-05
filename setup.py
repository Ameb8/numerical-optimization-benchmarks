from setuptools import setup
from setuptools.command.build_py import build_py
import subprocess
import pathlib
import shutil

class BuildCppBinary(build_py):
    """Custom build step: compile C++ binary with Makefile in release mode."""

    def run(self):
        # Run make in release mode
        cpp_dir = pathlib.Path(__file__).parent / "cpp"  # Adjust if your Makefile is elsewhere
        subprocess.check_call(["make", "MODE=release"], cwd=cpp_dir)

        # Copy the binary into the Python package
        bin_src = cpp_dir / "bin/release/benchmark"  # Must match your Makefile TARGET
        if not bin_src.exists():
            raise FileNotFoundError(f"C++ binary not found: {bin_src}")

        # inside the build package
        pkg_dir = pathlib.Path(self.build_lib) / "run_benchmark"
        pkg_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(bin_src, pkg_dir / "benchmark")

        # Continue the normal Python build
        super().run()

setup(
    cmdclass={"build_py": BuildCppBinary},
)
