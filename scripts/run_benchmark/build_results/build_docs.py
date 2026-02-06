import pandas as pd
from pathlib import Path

def build_summary_table(
    df: pd.DataFrame,
    output_dir: Path,
    *,
    filename: str = "best_fitness_summary.tex",
    label: str = "tab:best-fitness-summary",
    precision: int = 3,
) -> Path:
    """
    Write a single LaTeX table summarizing best-fitness statistics per experiment.

    Rows: one per experiment
    Columns: mean, median, std, min-max (best fitness over seeds)

    Returns
    -------
    Path
        Path to the written .tex file
    """

    output_dir.mkdir(parents=True, exist_ok=True)
    out_path = output_dir / filename

    def esc(s: str) -> str:
        return (
            s.replace("_", r"\_")
             .replace("&", r"\&")
             .replace("%", r"\%")
        )

    def fmt(x: float) -> str:
        return "--" if pd.isna(x) else f"{x:.{precision}g}"

    lines: list[str] = []

    lines.extend([
        r"\begin{table}[htbp]",
        r"\centering",
        r"\small",
        r"\begin{tabular}{lllrcccc}",
        r"\toprule",
        r"Problem & Dim & Optimizer & Mean & Median & Std & Min--Max \\",
        r"\midrule",
    ])

    df_sorted = df.sort_values(
        by=["problem_name", "dimensions", "optimizer_type"],
        kind="stable",
    )

    for _, row in df_sorted.iterrows():
        lines.append(
            " {} & {} & {} & {} & {} & {} & [{} , {}] \\\\".format(
                esc(row["problem_name"]),
                int(row["dimensions"]),
                esc(row["optimizer_type"]),
                fmt(row["best_mean"]),
                fmt(row["best_median"]),
                fmt(row["best_std"]),
                fmt(row["best_min"]),
                fmt(row["best_max"]),
            )
        )

    lines.extend([
        r"\bottomrule",
        r"\end{tabular}",
        r"\caption{Summary statistics of best fitness values over 30 independent runs per experiment.}",
        rf"\label{{{label}}}",
        r"\end{table}",
        "",
    ])

    out_path.write_text("\n".join(lines))
    return out_path

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
