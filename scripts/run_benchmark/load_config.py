import tomllib
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path


@dataclass
class Experiment:
    experiment_name: str = "Unnamed"
    upper_bound: float = 100.0
    lower_bound: float = -100.0
    problem_type: int = 1
    dimension: int = 30
    seed: int = 108664

    def __post_init__(self):
        # Initialize Experiments
        self.upper_bound = float(self.upper_bound)
        self.lower_bound = float(self.lower_bound)
        self.problem_type = int(self.problem_type)
        self.dimension = int(self.dimension)
        self.seed = int(self.seed)

        # Ensure valid population bounds
        if self.upper_bound <= self.lower_bound:
            self.upper_bound = 100.0
            self.lower_bound = -100.0

        # Ensure valid dimensions
        if self.dimension <= 0:
            self.dimension = 30

        # Ensure valid problem ID
        if self.problem_type < 0 or self.problem_type > 10:
            self.problem_type = 1

        # Ensure valid seed
        if self.seed < 0: 
            self.seed = 102486


@dataclass
class Benchmark:
    benchmark_name: str = f'Benchmark_{datetime.now()}'
    population_size: int = 30
    experiments: list[Experiment] = field(default_factory=list)

    @classmethod
    def from_toml(cls, config_path: Path) -> "Benchmark":
        """Construct a Benchmark instance from a TOML config file."""
        with config_path.open("rb") as f:
            config_data = tomllib.load(f)

        # Read benchmark-level info
        benchmark_data = config_data.get("benchmark", {})
        benchmark_name = benchmark_data.get("benchmark_name", f'Benchmark_{datetime.now()}')
        population_size = int(benchmark_data.get("population_size", 30))

        # Read experiments
        experiments = [Experiment(**exp_dict) for exp_dict in config_data.get("experiment", [])]

        return cls(
            benchmark_name=benchmark_name,
            population_size=population_size,
            experiments=experiments
        )