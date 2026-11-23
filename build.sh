#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

if [[ "${1:-}" == --help || "${1:-}" == -h ]]; then
    cat <<'EOF'
Usage: ./build.sh [--skip-deps] [CMake configure options...]

Installs missing Linux dependencies, then configures and builds Chess GUI.
Only package installation uses sudo; do not run this whole script with sudo.

  --skip-deps   Use dependencies you have already installed (e.g. custom Qt).
  BUILD_DIR    Build directory (default: build, relative to the repository).
  JOBS         Number of parallel build jobs (default: 2).

Example: ./build.sh -DCMAKE_BUILD_TYPE=Release
EOF
    exit 0
fi

if [[ "${1:-}" == --skip-deps ]]; then
    shift
else
    "$project_dir/scripts/install-dependencies.sh"
fi

cd -- "$project_dir"
build_dir="${BUILD_DIR:-build}"
cmake -S "$project_dir" -B "$build_dir" "$@"
cmake --build "$build_dir" --parallel "${JOBS:-2}"
printf '\nBuild complete. Run: %q\n' "$build_dir/appchessGui_2"
