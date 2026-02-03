from pydantic import BaseModel, Field, model_validator

from pathlib import Path
import tomllib
from itertools import product
from typing import List

from .experiment import Experiment
from .optimizer import Optimizer


class Benchmark(BaseModel):
    benchmark_name: str
    runs: int = Field(30, gt=0)
    dimensions: list[int] = Field(default_factory=list)
    seed_type: str
    seed_step: int
    seed_begin: int
    optimizers: list[Optimizer] = Field(default_factory=list)
    experiment_templates: list[dict] = Field(default_factory=list)
    experiments: list[Experiment] = Field(default_factory=list)


    def generate_all_experiments(self):
        """Automatically generate all experiment combinations as a Benchmark field."""
        all_experiments = []

        # Compute seeds
        if self.seed_type == "iterate":
            seeds = [self.seed_begin + i * self.seed_step for i in range(self.runs)]
        else:
            seeds = [self.seed_begin] * self.runs

        # Cartesian product: template × dimension × optimizer × seed
        for template, dim, optimizer, seed in product(
            self.experiment_templates, self.dimensions, self.optimizers, seeds
        ):
            exp = Experiment(
                template_name=template["experiment_name"],
                optimizer=optimizer,
                dimensions=dim,
                seed=seed,
                upper_bound=template.get("upper_bound", 100.0),
                lower_bound=template.get("lower_bound", -100.0),
                problem_type=template.get("problem_type", 1),
            )
            all_experiments.append(exp)

        self.experiments = all_experiments


    @model_validator(mode="after")
    def build_experiments(self):
        self.generate_all_experiments()
        return self


    @classmethod
    def from_toml(cls, path: Path) -> "Benchmark":
        with path.open("rb") as f:
            cfg = tomllib.load(f)

        # Parse optimizers
        optimizers = [Optimizer(**opt) for opt in cfg.get("optimizer", [])]

        # Flatten seed info
        seed_info = cfg.get("seed", {})
        runs = cfg.get("benchmark", {}).get("runs", 30)

        return cls(
            benchmark_name=cfg.get("benchmark", {}).get("benchmark_name", "Unnamed"),
            runs=runs,
            dimensions=cfg.get("benchmark", {}).get("dimensions", [30]),
            seed_type=seed_info.get("type", "iterate"),
            seed_step=seed_info.get("step", 1),
            seed_begin=seed_info.get("begin", 108664),
            optimizers=optimizers,
            experiment_templates=cfg.get("experiment", []),
        )
    
    def to_json_dict(self) -> dict:
        return {
            "benchmark_name": self.benchmark_name,
            "runs": self.runs,
            "experiments": [
            exp.model_dump(
                by_alias=True,
                exclude_none=True,
                exclude={
                    "template_name": True,
                    "optimizer": {"apply"},
                }
            )
            for exp in self.experiments
        ],
        }

    def to_json(self, *, indent: int = 2) -> str:
        import json
        return json.dumps(self.to_json_dict(), indent=indent)
