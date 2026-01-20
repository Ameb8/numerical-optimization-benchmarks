from pathlib import Path

def build_latex_report(
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
