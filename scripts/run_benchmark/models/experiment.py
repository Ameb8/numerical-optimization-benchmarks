from pydantic import BaseModel, Field, field_validator, ConfigDict

class Experiment(BaseModel):
    model_config = ConfigDict(frozen=True)

    experiment_name: str = "Unnamed"
    upper_bound: float = 100.0
    lower_bound: float = -100.0
    problem_type: int = Field(1, ge=0, le=10)
    dimensions: int = Field(30, gt=0)
    seed: int = Field(108664, ge=0)

    @field_validator("upper_bound")
    @classmethod
    def validate_bounds(cls, v, info):
        lower = info.data.get("lower_bound", -100.0)
        if v <= lower:
            return 100.0
        return v
