from enum import IntEnum

class ProblemType(IntEnum):
    SCHWEFEL = (1, "Schwefel")
    DEJONG_ONE = (2, "DeJong One")
    ROSENBROCK = (3, "Rosenbrock")
    RASTRIGIN = (4, "Rastrigin")
    GRIEWANGK = (5, "Griewangk")
    SINE_ENVELOPE_SINE_WAVE = (6, "Sine Envelope Sine Wave")
    STRETCHED_V_SINE_WAVE = (7, "Stretched V Sine Wave")
    ACKLEY_ONE = (8, "Ackley One")
    ACKLEY_TWO = (9, "Ackley Two")
    EGG_HOLDER = (10, "Egg Holder")

    def __new__(cls, value: int, label: str):
        obj = int.__new__(cls, value)
        obj._value_ = value
        obj.label = label
        return obj
