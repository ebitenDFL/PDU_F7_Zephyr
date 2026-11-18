# VIIALA_PDU1_V0 - standalone Zephyr CANopen project

This repository is an out-of-tree Zephyr application with its own custom board
definition. It does not modify the Zephyr source tree or any existing project.

The functional firmware keeps a fail-safe startup boundary:

- all 15 power-output enables initialize inactive;
- current-sense enable and AD7490 sampling are initialized before output control;
- ADC chip select initializes inactive;
- CAN1 is enabled on PB8/PB9 at 500 kbit/s with CANopen node ID 10;
- the CANopen dictionary, EDS/XML sources and 10 Hz current/state TPDO map are included;
- output requests require CANopen Operational state and valid current telemetry;
- configurable continuous and inrush current protection latches a tripped output OFF;
- node-ID and bitrate changes are available through volatile SDO settings;
- there are no automatic runner, flash, or debug commands in the project.

The normal standalone `A001` image was first programmed and byte-verified on
2026-11-11 using an external STLINK-V3SET after the target was identified as an
STM32F72x/73x with 256 KiB flash. The hash-only MCUboot update artifacts remain
explicitly unsafe to flash. See
`hardware/BUILD_VERIFICATION.md`.

Two variants are available. SDO upload `0x100A:00` returns `A001` for image A
or `B001` for image B, allowing the running image to be identified with one
standard CANopen request. See `docs/CANOPEN.md` and
`docs/OBJECT_DICTIONARY.md`.

An additional bench-tested MCUboot/CANopen download configuration is included.
It uses a 64 KiB bootloader, 64 KiB active slot, and 128 KiB staging slot in
overwrite-only mode. It does not provide rollback, and its hash-only build must
not be flashed. A separate RSA-signed bench build uses MCUboot's public
development key and successfully updated `A001` to `B001` over CAN. See
`docs/CANOPEN.md` for the safety boundary.

## Build with pinned Zephyr 4.5.0

Run these commands in Bash from the repository root. Use a checkout path
without spaces because Zephyr 4.5.0 splits it during Kconfig configuration.
The environment helper defaults to `"$HOME/zephyrproject-4.5.0"`.
Set `PDU_WEST_WORKSPACE` to use
another west workspace. Set `ZEPHYR_BASE` if its Zephyr checkout is elsewhere.
If you set only `ZEPHYR_BASE`, the helper uses its parent as the workspace.
The helper uses an active virtual environment or the workspace's `.venv`.
Set `PDU_VENV` to select a different one. If your toolchain needs an explicit
SDK location, export `ZEPHYR_SDK_INSTALL_DIR`, for example
`export ZEPHYR_SDK_INSTALL_DIR="$HOME/zephyr-sdk-1.0.1"`.

### One-time flash-layout setup

Select the Zephyr workspace you intend to build against. This procedure checks
the selected checkout and asks before applying the STM32F722RC 256 KiB
flash-layout patch. Run it again after changing Zephyr revisions.

```bash
(
  source scripts/setup-env.sh || exit 1
  patch_file="$pdu_root/patches/zephyr-stm32f722rc-256k-flash-layout.patch"
  git -C "$ZEPHYR_BASE" log -1 --oneline || exit 1
  if git -C "$ZEPHYR_BASE" apply --reverse --check "$patch_file" 2>/dev/null; then
    echo "The flash-layout patch is already applied."
  elif git -C "$ZEPHYR_BASE" apply --check "$patch_file"; then
    read -r -p "Apply the flash-layout patch to $ZEPHYR_BASE? [y/N] " answer
    if [[ "$answer" == [yY] ]]; then
      git -C "$ZEPHYR_BASE" apply "$patch_file"
    fi
  else
    echo "The patch is incompatible or partially applied. Use a clean Zephyr 4.5.0 checkout." >&2
    exit 1
  fi
)
```

### Build signed CANopen updates

Normal builds check the flash-layout patch without altering the Zephyr checkout.
If CANopenNodeZephyr lives outside your west manifest, export
`ZEPHYR_EXTRA_MODULES` with its absolute path before building. For example,
`export ZEPHYR_EXTRA_MODULES="$HOME/zephyrproject/modules/CANopenNodeZephyr"`.
Build both signed CAN-update variants with:

```sh
source scripts/setup-env.sh
./scripts/build_canopen_updates.sh
```

Zephyr sysbuild selects `root-rsa-2048.pem` from the discovered MCUboot module
(`bootloader/mcuboot/root-rsa-2048.pem` in a standard west workspace).
This is the same published RSA-2048 development key used in the bench-tested
MCUboot setup. Production firmware requires a private signing key with
controlled access and a bootloader configured to trust its public key.

These commands compile only. They do not invoke the connected ST-LINK. Do not
connect or activate loads during compile-only verification.

### Temporary A001 SWO diagnostic build

This separate variant keeps the normal A/B configurations unchanged. It sends
boot and five-second safe-state messages through ITM port 0 on PB3 at 2 MHz.

```sh
(
  cd "$PDU_WEST_WORKSPACE" || exit
  west build -p always -b speeder_pdu_f7/stm32f722xx \
    "$pdu_root" -d "$pdu_root/build_swo_a" -- \
    -DEXTRA_CONF_FILE=variants/swo_a.conf \
    -DDTC_OVERLAY_FILE=variants/swo.overlay \
    "-DZEPHYR_EXTRA_MODULES=${ZEPHYR_EXTRA_MODULES:-}"
)

STM32_Programmer_CLI -c port=SWD freq=1000 mode=UR reset=HWrst \
  -swv freq=216 portnumber=0 "${TMPDIR:-/tmp}/pdu_swo_live.log" -RA
```

The viewer must connect under hardware reset on this target. Hot attach did not
identify the running core. The diagnostic image still reports `A001` through
CANopen SDO `0x100A:00` and does not enable any output-control path.

## Expected verification

After a successful build, verify from the repository root. Sysbuild names the
application image directory after the checkout directory:

```sh
image_dir="$(basename "$pdu_root")"
for variant in a b; do
  output_dir="build_update_$variant/$image_dir/zephyr"
  grep -E 'CONFIG_(CANOPEN_NODE_ID|CAN_DEFAULT_BITRATE|PDU_IMAGE_VARIANT)' \
    "$output_dir/.config"
  strings "$output_dir/zephyr.signed.bin" | grep 'VIIALA_PDU1_V0'
done
```

Expected: both builds succeed, CAN defaults to 500 kbit/s and node ID 10, the
respective A/B identity is embedded, and flash remains limited to 256 KiB.

The first verified build is recorded in `hardware/BUILD_VERIFICATION.md`.

## Linux CANopen GUI

The `gui/` directory contains the VIIALA PDU CANopen Workbench for the PEAK
PCAN-USB adapter on Linux. It shows live identity, heartbeat, diagnostics and
all fifteen telemetry channels, includes an SDO inspector and CAN traffic view,
and supports per-row ON/OFF/Reset output controls, current protection configuration,
confirmed raw SDO writes, volatile communication changes and signed MCUboot
updates. Its launcher automatically prepares `can0` at 500 kbit/s when needed.

Launch it from the repository root:

```sh
./gui/run_pdu_gui.sh
```

See `gui/README.md` for interface setup, testing and the update safety boundary.

## Source of truth

See `hardware/VERIFIED_PINOUT.md`. The board definition was derived from the
updated `SPEEDER_PDU_v2.pdf`, not copied from conflicting historical VIIALA
Zephyr board files.
