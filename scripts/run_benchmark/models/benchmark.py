from pydantic import BaseModel, Field, model_serializer

from datetime import datetime
from pathlib import Path
import tomllib
from typing import Any

from .experiment import Experiment

class Benchmark(BaseModel):
    benchmark_name: str = Field(
        default_factory=lambda: datetime.now().strftime("Benchmark_%Y%m%d_%H%M%S")
    )
    population_size: int = Field(30, gt=0)
    experiments: list[Experiment] = Field(default_factory=list)

    @model_serializer(mode="wrap")
    def serialize_with_population(self, serializer) -> dict[str, Any]:
        data = serializer(self)

        # Inject population_size into each experiment JSON
        data["experiments"] = [
            {
                **exp.model_dump(),
                "population_size": self.population_size
            }
            for exp in self.experiments
        ]

        return data

    @classmethod
    def from_toml(cls, config_path: Path) -> "Benchmark":
        # Open config file
        with config_path.open("rb") as f:
            config = tomllib.load(f)

        return cls(
            benchmark_name=config.get("benchmark", {}).get(
                "benchmark_name",
                f"Benchmark_{datetime.now()}"
            ),
            population_size=config.get("benchmark", {}).get(
                "population_size",
                30
            ),
            experiments=config.get("experiment", [])
        )
