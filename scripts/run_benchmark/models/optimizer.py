from pydantic import BaseModel, Field, ConfigDict, field_validator

from typing import Literal
import tomllib 


class Optimizer(BaseModel):
    name: str
    type: Literal['blind', 'repeated local']
    iterations: int = Field(..., gt=0)
    neighbor_delta: int | None = Field(default=None, gt=0, alias="delta")
    num_neighbors: int | None = Field(default=None, gt=0)
    apply: str

    model_config = ConfigDict(
        frozen=True,
        populate_by_name=True
    )

    @field_validator("neighbor_delta", "num_neighbors", mode="before")
    @classmethod
    def validate_optimizer_fields(cls, v, info):
        opt_type = info.data.get("type")

        if opt_type == "blind":
            if v is not None:
                raise ValueError(
                    f"{info.name} must be omitted for blind optimizer"
                )
            return None

        if opt_type == "repeated local":
            if v is None or v <= 0:
                raise ValueError(
                    f"{info.name} must be set and > 0 for repeated local optimizer"
                )

        return v
