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



def build_latex_main(result_dir: Path) -> None:
    tex_dir = result_dir / "docs"
    fig_dir = tex_dir / "figures"
    tab_dir = tex_dir / "tables"

    figure_files = sorted(
        p for p in fig_dir.glob("*.tex") if p.name != "main.tex"
    )

    table_files = sorted(tab_dir.glob("*.tex"))

    main_tex = tex_dir / "main.tex"

    lines = []

    # Preamble
    lines += [
        r"\documentclass{article}",
        r"\usepackage{graphicx}",
        r"\usepackage{booktabs}",
        r"\usepackage{placeins}",
        r"\usepackage{caption}",
        "",
        r"\begin{document}",
        "",
        r"\section{Results}",
        "",
    ]

    # Figures
    if figure_files:
        lines += [
            r"\subsection{Figures}",
            "",
        ]
        for fig in figure_files:
            rel = fig.relative_to(tex_dir).as_posix()
            lines.append(rf"\input{{{rel}}}")
        lines += ["", r"\FloatBarrier", ""]

    # Tables
    if table_files:
        lines += [
            r"\subsection{Tables}",
            "",
        ]
        for tab in table_files:
            rel = tab.relative_to(tex_dir).as_posix()
            lines.append(rf"\input{{{rel}}}")
        lines += ["", r"\FloatBarrier", ""]

    # End
    lines += [
        r"\end{document}",
        "",
    ]

    main_tex.write_text("\n".join(lines))



def write_latex_figure(
    tex_path: Path,
    image_path: Path,
    caption: str,
    label: str,
    width: str = "0.85\\linewidth",
) -> None:
    tex = rf"""
\begin{{figure}}[htbp]
    \centering
    \includegraphics[width={width}]{{{image_path.as_posix()}}}
    \caption{{{caption}}}
    \label{{{label}}}
\end{{figure}}
""".lstrip()

    tex_path.parent.mkdir(parents=True, exist_ok=True)
    tex_path.write_text(tex)
