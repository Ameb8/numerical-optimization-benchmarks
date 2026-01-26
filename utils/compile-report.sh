#!/usr/bin/env bash

# Exit on error
set -euo pipefail

REPORT_NAME="benchmark-report"

# Compile latex report to PDF
cd latex
latexmk -pdf --outdir=build benchmark.tex


# Move compiled report to docs directory
mv build/benchmark.pdf ../docs/${REPORT_NAME}.pdf

