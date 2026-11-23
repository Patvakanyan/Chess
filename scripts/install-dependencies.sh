#!/usr/bin/env bash
set -euo pipefail

dry_run=false
case "${1:-}" in
    --dry-run) dry_run=true; shift ;;
    --help|-h)
        echo 'Usage: ./scripts/install-dependencies.sh [--dry-run]'
        echo 'Install missing build and Qt runtime packages on Fedora, Debian/Ubuntu, or Arch Linux.'
        echo '--dry-run prints the commands without installing anything or using sudo.'
        exit 0
        ;;
esac
if (( $# )); then
    echo "Unknown argument: $1" >&2
    exit 2
fi

if [[ ! -r /etc/os-release ]]; then
    echo 'Automatic dependency installation requires a supported Linux distribution.' >&2
    exit 1
fi
# shellcheck source=/dev/null
source /etc/os-release

case " ${ID:-} ${ID_LIKE:-} " in
    *' debian '*|*' ubuntu '*)
        manager=apt-get
        packages=(build-essential cmake make ca-certificates pkg-config
                  qt6-base-dev qt6-multimedia-dev qt6-qpa-plugins qt6-wayland)
        ;;
    *' fedora '*)
        manager=dnf
        packages=(gcc-c++ cmake make ca-certificates pkgconf-pkg-config
                  qt6-qtbase-devel qt6-qtmultimedia-devel qt6-qtwayland)
        ;;
    *' arch '*)
        manager=pacman
        packages=(gcc cmake make ca-certificates pkgconf
                  qt6-base qt6-multimedia qt6-multimedia-ffmpeg qt6-wayland)
        ;;
    *)
        echo "Automatic dependency installation is not supported on ${PRETTY_NAME:-this OS}." >&2
        echo 'Install CMake >= 3.16, a C++17 compiler, GNU Make, and Qt >= 6.4 (Widgets and Multimedia).' >&2
        echo 'Then run ./build.sh --skip-deps.' >&2
        exit 1
        ;;
esac

if ! command -v "$manager" >/dev/null 2>&1; then
    echo "Required package manager not found: $manager" >&2
    exit 1
fi

is_installed() {
    case "$manager" in
        apt-get) [[ "$(dpkg-query -W -f='${Status}' "$1" 2>/dev/null)" == 'install ok installed' ]] ;;
        dnf) rpm -q "$1" >/dev/null 2>&1 ;;
        pacman) pacman -Q "$1" >/dev/null 2>&1 ;;
    esac
}

missing=()
for package in "${packages[@]}"; do
    if ! is_installed "$package"; then
        missing+=("$package")
    fi
done

if (( ${#missing[@]} == 0 )); then
    echo 'All dependency packages are already installed.'
    exit 0
fi

printf 'Missing packages:'
printf ' %s' "${missing[@]}"
printf '\n'

privilege=()
if (( EUID != 0 )); then
    if ! "$dry_run" && ! command -v sudo >/dev/null 2>&1; then
        echo 'sudo is required to install missing packages. Ask an administrator to run this installer.' >&2
        exit 1
    fi
    privilege=(sudo)
fi

run() {
    printf '+'
    printf ' %q' "${privilege[@]}" "$@"
    printf '\n'
    if ! "$dry_run"; then
        "${privilege[@]}" "$@"
    fi
}

case "$manager" in
    apt-get)
        run apt-get update
        run apt-get install -y "${missing[@]}"
        ;;
    dnf) run dnf install -y "${missing[@]}" ;;
    pacman) run pacman -S --needed --noconfirm "${missing[@]}" ;;
esac
