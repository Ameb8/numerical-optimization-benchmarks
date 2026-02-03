from pydantic import BaseModel, Field, field_validator, ConfigDict, computed_field

from .optimizer import Optimizer

class Experiment(BaseModel):
    model_config = ConfigDict(
        frozen=True,
        populate_by_name=True
    )

    template_name: str
    upper_bound: float = 100.0
    lower_bound: float = -100.0
    problem_type: int = Field(1, ge=0, le=10)
    dimensions: int = Field(30, gt=0)
    seed: int = Field(108664, ge=0)
    optimizer: Optimizer

    @computed_field
    @property
    def experiment_name(self) -> str:
        return (
            f"{self.template_name}_"
            f"{self.optimizer.name}_"
            f"dim{self.dimensions}_"
            f"seed{self.seed}"
        )

    @field_validator("upper_bound")
    @classmethod
    def validate_bounds(cls, v, info):
        lower = info.data.get("lower_bound", -100.0)
        if v <= lower:
            return 100.0
        return v
    
