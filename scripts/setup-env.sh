#!/usr/bin/env bash
# Source this file from Bash before building the firmware.
pdu_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd -P)" || return 1

export ZEPHYR_BASE="${ZEPHYR_BASE:-${PDU_WEST_WORKSPACE:-"$HOME/zephyrproject-4.5.0"}/zephyr}"
export PDU_WEST_WORKSPACE="${PDU_WEST_WORKSPACE:-"${ZEPHYR_BASE%/}/.."}"
if [[ -n "${PDU_VENV:-}" ]]; then
	source "$PDU_VENV/bin/activate" || return 1
elif [[ -z "${VIRTUAL_ENV:-}" && -f "$PDU_WEST_WORKSPACE/.venv/bin/activate" ]]; then
	source "$PDU_WEST_WORKSPACE/.venv/bin/activate" || return 1
fi
