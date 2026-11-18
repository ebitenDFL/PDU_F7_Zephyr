#!/usr/bin/env bash
set -euo pipefail

source "$(dirname -- "${BASH_SOURCE[0]}")/setup-env.sh"
layout_patch="$pdu_root/patches/zephyr-stm32f722rc-256k-flash-layout.patch"

if ! git -C "$ZEPHYR_BASE" apply --reverse --check "$layout_patch"; then
	echo "The STM32F722RC flash layout patch is required. See README.md." >&2
	exit 2
fi

cd -- "$PDU_WEST_WORKSPACE"
module_args=()
if [[ -n "${ZEPHYR_EXTRA_MODULES:-}" ]]; then
	module_args+=("-DZEPHYR_EXTRA_MODULES=$ZEPHYR_EXTRA_MODULES")
fi

west build --sysbuild -p always -b speeder_pdu_f7/stm32f722xx \
	"$pdu_root" -d "$pdu_root/build_update_a" -- \
	-DBOARD_ROOT="$pdu_root" -DDTS_ROOT="$pdu_root" \
	-DEXTRA_CONF_FILE=variants/update_a.conf "${module_args[@]}"

west build --sysbuild -p always -b speeder_pdu_f7/stm32f722xx \
	"$pdu_root" -d "$pdu_root/build_update_b" -- \
	-DBOARD_ROOT="$pdu_root" -DDTS_ROOT="$pdu_root" \
	-DEXTRA_CONF_FILE=variants/update_b.conf "${module_args[@]}"

echo "Bench-signed A and B CANopen update images built successfully."
