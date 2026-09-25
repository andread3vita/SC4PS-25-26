#!/usr/bin/env bash

# Build a private GMP + MPFR toolchain without root privileges.
# Everything created by this script stays below this directory.

set -euo pipefail

GMP_VERSION="6.3.0"
MPFR_VERSION="4.2.2"
GMP_SHA256="a3c2b80201b89e68616f4ad30bc66aee4927c3ce50e33929ca819d5c43538898"
MPFR_SHA256="b67ba0383ef7e8a8563734e2e889ef5ec3c3b898a01d00fa0a6869ad81c6ce01"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PREFIX="${MPFR_PREFIX:-${SCRIPT_DIR}/.mpfr-env}"
WORK_DIR="${MPFR_BUILD_DIR:-${SCRIPT_DIR}/.mpfr-build}"
DOWNLOAD_DIR="${WORK_DIR}/downloads"
SOURCE_DIR="${WORK_DIR}/sources"
BUILD_DIR="${WORK_DIR}/build"
JOBS="${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || printf '2')}"

die() {
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

for command_name in gcc make tar sha256sum; do
    command -v "${command_name}" >/dev/null 2>&1 || \
        die "required command not found: ${command_name}"
done

if command -v curl >/dev/null 2>&1; then
    download() {
        curl --fail --location --retry 3 --output "$2" "$1"
    }
elif command -v wget >/dev/null 2>&1; then
    download() {
        wget --tries=3 --output-document="$2" "$1"
    }
else
    die "curl or wget is required to download the source archives"
fi

mkdir -p "${PREFIX}" "${DOWNLOAD_DIR}" "${SOURCE_DIR}" "${BUILD_DIR}"

fetch_and_verify() {
    local url="$1"
    local archive="$2"
    local expected_sha256="$3"

    if [[ ! -f "${archive}" ]]; then
        printf 'Downloading %s\n' "${url}"
        download "${url}" "${archive}.part"
        mv -- "${archive}.part" "${archive}"
    fi

    printf '%s  %s\n' "${expected_sha256}" "${archive}" | sha256sum --check --status || {
        rm -f -- "${archive}"
        die "checksum verification failed; the bad archive was removed"
    }
}

extract_source() {
    local archive="$1"
    local source_path="$2"

    if [[ ! -x "${source_path}/configure" ]]; then
        rm -rf -- "${source_path}"
        tar -xf "${archive}" -C "${SOURCE_DIR}"
    fi
}

GMP_ARCHIVE="${DOWNLOAD_DIR}/gmp-${GMP_VERSION}.tar.xz"
MPFR_ARCHIVE="${DOWNLOAD_DIR}/mpfr-${MPFR_VERSION}.tar.xz"
GMP_SOURCE="${SOURCE_DIR}/gmp-${GMP_VERSION}"
MPFR_SOURCE="${SOURCE_DIR}/mpfr-${MPFR_VERSION}"

fetch_and_verify \
    "https://ftp.gnu.org/gnu/gmp/gmp-${GMP_VERSION}.tar.xz" \
    "${GMP_ARCHIVE}" "${GMP_SHA256}"
fetch_and_verify \
    "https://ftp.gnu.org/gnu/mpfr/mpfr-${MPFR_VERSION}.tar.xz" \
    "${MPFR_ARCHIVE}" "${MPFR_SHA256}"

extract_source "${GMP_ARCHIVE}" "${GMP_SOURCE}"
extract_source "${MPFR_ARCHIVE}" "${MPFR_SOURCE}"

if [[ ! -f "${PREFIX}/include/gmp.h" ]]; then
    printf '\nBuilding GMP %s...\n' "${GMP_VERSION}"
    mkdir -p "${BUILD_DIR}/gmp-${GMP_VERSION}"
    cd "${BUILD_DIR}/gmp-${GMP_VERSION}"
    "${GMP_SOURCE}/configure" \
        --prefix="${PREFIX}" \
        --enable-shared \
        --enable-static
    make -j"${JOBS}"
    make -j"${JOBS}" check
    make install
else
    printf 'Using existing local GMP installation in %s\n' "${PREFIX}"
fi

LOCAL_LIB_DIR="${PREFIX}/lib"
[[ -d "${PREFIX}/lib64" ]] && LOCAL_LIB_DIR="${PREFIX}/lib64"
export LD_LIBRARY_PATH="${LOCAL_LIB_DIR}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

if [[ ! -f "${PREFIX}/include/mpfr.h" ]]; then
    printf '\nBuilding MPFR %s...\n' "${MPFR_VERSION}"
    mkdir -p "${BUILD_DIR}/mpfr-${MPFR_VERSION}"
    cd "${BUILD_DIR}/mpfr-${MPFR_VERSION}"
    "${MPFR_SOURCE}/configure" \
        --prefix="${PREFIX}" \
        --with-gmp="${PREFIX}" \
        --enable-shared \
        --enable-static
    make -j"${JOBS}"
    make -j"${JOBS}" check
    make install
else
    printf 'Using existing local MPFR installation in %s\n' "${PREFIX}"
fi

SMOKE_TEST="${BUILD_DIR}/mpfr-smoke-test"
printf '%s\n' \
    '#include <mpfr.h>' \
    'int main(void) {' \
    '  mpfr_t x;' \
    '  mpfr_init_set_ui(x, 2, MPFR_RNDN);' \
    '  mpfr_sqrt(x, x, MPFR_RNDN);' \
    '  mpfr_printf("sqrt(2) = %.20Rf\n", x);' \
    '  mpfr_clear(x);' \
    '  return 0;' \
    '}' | gcc -x c - \
        -I"${PREFIX}/include" \
        -L"${LOCAL_LIB_DIR}" \
        -Wl,-rpath,"${LOCAL_LIB_DIR}" \
        -lmpfr -lgmp -o "${SMOKE_TEST}"

printf '\nInstallation test: '
"${SMOKE_TEST}"
printf '\nMPFR is ready. Activate it in your current shell with:\n\n'
printf '    source "%s/activate_mpfr.sh"\n\n' "${SCRIPT_DIR}"
