#!/usr/bin/env bash
set -euo pipefail

project_dir=/home/dfl/Projects/PDU_F7_Zephyr
west_dir=/home/dfl/zephyrproject
zephyr_dir=/home/dfl/zephyrproject/zephyr
layout_patch="$project_dir/patches/zephyr-stm32f722rc-256k-flash-layout.patch"
driver_file=drivers/flash/flash_stm32f7x.c

if [[ -n "$(git -C "$zephyr_dir" status --short -- "$driver_file")" ]]; then
	echo "Refusing to build: $driver_file already has local changes." >&2
	exit 2
fi

git -C "$zephyr_dir" apply "$layout_patch"
cleanup() {
	git -C "$zephyr_dir" apply -R "$layout_patch"
}
trap cleanup EXIT HUP INT TERM

cd "$west_dir"
source .venv/bin/activate

west build --sysbuild -p always -b speeder_pdu_f7/stm32f722xx \
	"$project_dir" -d "$project_dir/build_update_a" -- \
	-DBOARD_ROOT="$project_dir" -DEXTRA_CONF_FILE=variants/update_a.conf

west build --sysbuild -p always -b speeder_pdu_f7/stm32f722xx \
	"$project_dir" -d "$project_dir/build_update_b" -- \
	-DBOARD_ROOT="$project_dir" -DEXTRA_CONF_FILE=variants/update_b.conf

echo "Bench-signed A and B CANopen update images built successfully."
