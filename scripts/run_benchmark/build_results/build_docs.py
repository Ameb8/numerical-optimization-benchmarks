import pandas as pd
from pathlib import Path

def latex_escape(text: str) -> str:
    """
    Escape LaTeX special characters.
    """
    return (
        text.replace("\\", r"\textbackslash{}")
            .replace("_", r"\_")
            .replace("%", r"\%")
            .replace("&", r"\&")
            .replace("#", r"\#")
            .replace("{", r"\{")
            .replace("}", r"\}")
    )


def write_fitness_summary_table(f, df: pd.DataFrame) -> None:
    f.write(r"""
\section{Fitness Summary Statistics}

\begin{table}[H]
\centering
\caption{Summary statistics of fitness values across experiments}
\label{tab:fitness-summary}
\resizebox{\textwidth}{!}{
\begin{tabular}{lrrrrrrr}
\hline
Experiment & Mean & Std & Median & Min & Max & Range & Time (ms) \\
\hline
""")

    for _, row in df.iterrows():
        exp_name = latex_escape(row["experiment_name"])
        time_ms = row["wall_time"] * 1000.0

        f.write(
            f"{exp_name} & "
            f"{row['mean']:.4e} & "
            f"{row['std']:.4e} & "
            f"{row['median']:.4e} & "
            f"{row['min']:.4e} & "
            f"{row['max']:.4e} & "
            f"{row['range']:.4e} & "
            f"{time_ms:.2f} \\\\\n"
        )

    f.write(r"""\hline
\end{tabular}}
\end{table}
""")


def build_latex_report(
    df: pd.DataFrame,
    benchmark_dir: Path,
    title: str = "Benchmark Results",
    author: str = "Auto-generated",
) -> None:
    # Get paths
    plots_dir = benchmark_dir / "plots"
    docs_dir = benchmark_dir / "docs"
    docs_dir.mkdir(parents=True, exist_ok=True)

    tex_path = docs_dir / "benchmark.tex" # Path to generated .tex file

    # Collect all generated plots
    image_files = sorted(plots_dir.glob("*.png"))

    # Write document header
    with tex_path.open("w") as f:
        f.write(r"""\documentclass{article}
\usepackage{graphicx}
\usepackage{float}
\usepackage{caption}
\usepackage{subcaption}
\usepackage{geometry}
\geometry{margin=1in}

\title{%s}
\author{%s}
\date{\today}

\begin{document}
\maketitle
""" % (title, author))

        # Write fitness tables
        write_fitness_summary_table(f, df)
        #write_per_experiment_tables(f, df)

        # Write figures header
        f.write("\n\\section{Benchmark Plots}\n")

        # Iterate through plots
        for img in image_files:
            label = img.stem.replace("_", "-")
            caption = img.stem.replace("_", " ").title()

            # Write plots to file
            f.write(r"""
\begin{figure}[H]
    \centering
    \includegraphics[width=0.9\linewidth]{../plots/%s}
    \caption{%s}
    \label{fig:%s}
\end{figure}
""" % (img.name, caption, label))

        # End document
        f.write("\n\\end{document}\n")
