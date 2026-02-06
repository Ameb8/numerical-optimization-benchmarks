import pandas as pd
import numpy as np

from pathlib import Path

from ..models import Benchmark, Experiment

from . import plot_builder, build_docs


from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def plot_execution_times(df: pd.DataFrame, plot_dir: Path, tex_dir: Path) -> None:
    plot_dir.mkdir(parents=True, exist_ok=True)
    
    # Make the plots for each dimension
    for dim in sorted(df['dimensions'].unique()):
        # Get dimension vlue
        df_dim = df[df['dimensions'] == dim]
        
        # Create plot
        plt.figure(figsize=(12, 6))
        sns.barplot(
            data=df_dim,
            x='problem_name',
            y='execution_time',
            hue='optimizer_type',
            palette='Set2'
        )
        
        # Label plot
        plt.title(f'Execution Time for {dim} Dimensions')
        plt.xlabel('Problem Name')
        plt.ylabel('Execution Time (s)')
        plt.legend(title='Optimizer')
        plt.xticks(rotation=45)
        plt.tight_layout()
        
        # define path name
        filename = f'execution_time_{dim}d'
        save_path = plot_dir / filename

        # Save plot
        plt.savefig(f'{save_path}.png')
        plt.close()

        caption = (
            f"Execution time for {dim}-dimensions: "
            "Execution speed comparison between Blind Search and "
            "Repeated Local Search across different functions."
        )

        # Create label
        label = f"fig:{filename}"

        # Create LaTeX figure
        build_docs.write_latex_figure(
            tex_path=tex_dir / f'{filename}.tex',
            image_path=save_path.relative_to(tex_dir.parent),
            caption=caption,
            label=label,
        )



def plot_fitness_histograms(df: pd.DataFrame, plot_dir: Path, bins: int = 10) -> None:
    for _, row in df.iterrows():
        plot_builder.build_histogram(
            values=row['fitness_values'],
            mean=row['mean'],
            std=row['std'],
            median=row['median'],
            save_dir=plot_dir,
            filename=f"{row['experiment_name']}_histogram.png",
            title=f"{row['experiment_name']} Fitness Histogram",
            xlabel='Fitness Value',
            bins=bins,
        )

def plot_all_fitness_histogram(df: pd.DataFrame, plot_dir: Path, bins: int = 20) -> None:
    all_values = np.concatenate(df['fitness_values'].values)

    plot_builder.build_histogram(
        values=all_values,
        mean = np.mean(all_values),
        std = np.std(all_values),
        median = np.median(all_values),
        save_dir=plot_dir,
        filename='all_experiments_fitness_histogram.png',
        title='All Experiments Fitness Histogram',
        xlabel='Fitness Value',
        color='lightgreen',
        bins=bins
    )

def plot_time_bar(df: pd.DataFrame, plot_dir: Path) -> None:
    function_names = df['experiment_name'].tolist()
    times = df['wall_time'].values

    plot_builder.build_stat_bar(
        function_names=function_names,
        values=times,
        save_dir=plot_dir,
        filename='all_experiments_time.png',
        title='Execution Time per Benchmark Function',
        ylabel='Execution Time (s)'
    )

def plot_fitness_violins(df: pd.DataFrame, plot_dir: Path) -> None:
    for _, row in df.iterrows():
        experiment_name = row["experiment_name"]
        fitness_values = row["fitness_values"]

        plot_builder.build_violin_plot(
            values=fitness_values,
            save_dir=plot_dir,
            filename=f"{experiment_name}_violin.png",
            title=f"{experiment_name} Fitness Distribution"
        )


def resample_to_x(y, x):
    y = np.asarray(y)
    old_x = np.linspace(0, 1, len(y))
    new_x = np.linspace(0, 1, len(x))
    return np.interp(new_x, old_x, y)


def plot_fitness_curves(
    df: pd.DataFrame,
    plot_dir: Path,
    tex_dir: Path,
) -> None:
    # Group experiments by problem type and dimension
    grouped = df.groupby(["problem_name", "dimensions"])

    for (problem_name, dimensions), group in grouped:
        blind = group[group["optimizer_type"] == "blind"]
        repeated = group[group["optimizer_type"] == "repeated local"]

        if blind.empty or repeated.empty:
            continue
        
        blind = blind.iloc[0]
        repeated = repeated.iloc[0]

        x = np.arange(len(blind["fitness_curve_mean"]))

        filename = f"{problem_name}_dim{dimensions}_convergence"
        image_path = plot_dir / f"{filename}.png"


        # Build plot
        plot_builder.build_line_plot(
            x,
            [
                blind["fitness_curve_mean"],
                resample_to_x(repeated["fitness_curve_mean"], x)
            ],
            [
                "Blind Search",
                "Repeated Local Search",
            ],
            plot_dir,
            filename,
            f'{blind["problem_name"]} {blind["dimensions"]}-Dimensioned Convergence Speed',
            "Iterations",
            "Best Fitness Found",
        )

        caption = (
            f"{problem_name} "
            "convergence speed comparison between Blind Search and "
            f"Repeated Local Search with {dimensions}-Dimensions."
        )

        # Create label
        label = f"fig:{filename}"

        # Create LaTeX figure
        build_docs.write_latex_figure(
            tex_path=tex_dir / f"{filename}.tex",
            image_path=image_path.relative_to(tex_dir.parent),
            caption=caption,
            label=label,
        )





def build_result(df: pd.DataFrame, result_dir: Path):
    # Define and create output directories
    tex_dir = result_dir / 'docs'
    fig_dir = tex_dir / 'figures'
    plots_dir = fig_dir / 'plots'
    plots_dir.mkdir(parents=True, exist_ok=True)
    tab_dir = tex_dir / 'tables'
    tab_dir.mkdir(parents=True, exist_ok=True)

    # Generate figures
    plot_fitness_curves(df, plots_dir, fig_dir)
    plot_execution_times(df, plots_dir, fig_dir)
    build_docs.build_summary_table(
        df,
        tab_dir,
        filename="convergence_summary.tex",
    )

    # Generate main.tex
    build_docs.build_latex_main(result_dir)

    # Inform user of pots/document creation
    print(f'\nPlots and LaTeX document written to {plots_dir} and {result_dir / "docs"}')
