#!/usr/bin/env bash

# Source this file to use the private GMP + MPFR installation:
#   source ./activate_mpfr.sh
# Run `deactivate_mpfr_env` to restore the previous shell environment.

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
    printf 'This script must be sourced:\n  source "%s"\n' "$0" >&2
    exit 1
fi

if [[ "${MPFR_ENV_ACTIVE:-}" == "1" ]]; then
    printf 'The local MPFR environment is already active: %s\n' "${MPFR_ROOT}"
    return 0
fi

_MPFR_ACTIVATE_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
MPFR_ROOT="${MPFR_PREFIX:-${_MPFR_ACTIVATE_DIR}/.mpfr-env}"

if [[ ! -f "${MPFR_ROOT}/include/mpfr.h" ]]; then
    printf 'MPFR is not installed in %s\nRun "%s/install_mpfr.sh" first.\n' \
        "${MPFR_ROOT}" "${_MPFR_ACTIVATE_DIR}" >&2
    unset MPFR_ROOT _MPFR_ACTIVATE_DIR
    return 1
fi

_MPFR_OLD_PATH="${PATH-}"
_MPFR_OLD_CPATH="${CPATH-}"
_MPFR_OLD_LIBRARY_PATH="${LIBRARY_PATH-}"
_MPFR_OLD_LD_LIBRARY_PATH="${LD_LIBRARY_PATH-}"
_MPFR_OLD_PKG_CONFIG_PATH="${PKG_CONFIG_PATH-}"
_MPFR_OLD_CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH-}"
_MPFR_HAD_CPATH="${CPATH+x}"
_MPFR_HAD_LIBRARY_PATH="${LIBRARY_PATH+x}"
_MPFR_HAD_LD_LIBRARY_PATH="${LD_LIBRARY_PATH+x}"
_MPFR_HAD_PKG_CONFIG_PATH="${PKG_CONFIG_PATH+x}"
_MPFR_HAD_CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH+x}"

_MPFR_LIB_DIR="${MPFR_ROOT}/lib"
[[ -d "${MPFR_ROOT}/lib64" ]] && _MPFR_LIB_DIR="${MPFR_ROOT}/lib64"

export MPFR_ROOT
export PATH="${MPFR_ROOT}/bin${PATH:+:${PATH}}"
export CPATH="${MPFR_ROOT}/include${CPATH:+:${CPATH}}"
export LIBRARY_PATH="${_MPFR_LIB_DIR}${LIBRARY_PATH:+:${LIBRARY_PATH}}"
export LD_LIBRARY_PATH="${_MPFR_LIB_DIR}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
export PKG_CONFIG_PATH="${_MPFR_LIB_DIR}/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}"
export CMAKE_PREFIX_PATH="${MPFR_ROOT}${CMAKE_PREFIX_PATH:+:${CMAKE_PREFIX_PATH}}"
export MPFR_ENV_ACTIVE=1

deactivate_mpfr_env() {
    export PATH="${_MPFR_OLD_PATH}"

    if [[ -n "${_MPFR_HAD_CPATH}" ]]; then export CPATH="${_MPFR_OLD_CPATH}"; else unset CPATH; fi
    if [[ -n "${_MPFR_HAD_LIBRARY_PATH}" ]]; then export LIBRARY_PATH="${_MPFR_OLD_LIBRARY_PATH}"; else unset LIBRARY_PATH; fi
    if [[ -n "${_MPFR_HAD_LD_LIBRARY_PATH}" ]]; then export LD_LIBRARY_PATH="${_MPFR_OLD_LD_LIBRARY_PATH}"; else unset LD_LIBRARY_PATH; fi
    if [[ -n "${_MPFR_HAD_PKG_CONFIG_PATH}" ]]; then export PKG_CONFIG_PATH="${_MPFR_OLD_PKG_CONFIG_PATH}"; else unset PKG_CONFIG_PATH; fi
    if [[ -n "${_MPFR_HAD_CMAKE_PREFIX_PATH}" ]]; then export CMAKE_PREFIX_PATH="${_MPFR_OLD_CMAKE_PREFIX_PATH}"; else unset CMAKE_PREFIX_PATH; fi

    unset MPFR_ROOT MPFR_ENV_ACTIVE _MPFR_ACTIVATE_DIR _MPFR_LIB_DIR
    unset _MPFR_OLD_PATH _MPFR_OLD_CPATH _MPFR_OLD_LIBRARY_PATH
    unset _MPFR_OLD_LD_LIBRARY_PATH _MPFR_OLD_PKG_CONFIG_PATH _MPFR_OLD_CMAKE_PREFIX_PATH
    unset _MPFR_HAD_CPATH _MPFR_HAD_LIBRARY_PATH _MPFR_HAD_LD_LIBRARY_PATH
    unset _MPFR_HAD_PKG_CONFIG_PATH _MPFR_HAD_CMAKE_PREFIX_PATH
    unset -f deactivate_mpfr_env
    printf 'Local MPFR environment deactivated.\n'
}

printf 'Local MPFR environment activated: %s\n' "${MPFR_ROOT}"
printf 'Compile with, for example: gcc program.c -lmpfr -lgmp\n'

