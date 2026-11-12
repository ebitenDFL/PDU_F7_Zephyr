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

## Build in the existing Linux Zephyr workspace

```sh
source /home/dfl/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=/home/dfl/zephyrproject/zephyr
export ZEPHYR_SDK_INSTALL_DIR=/home/dfl/zephyr-sdk-1.0.1
cd /home/dfl/zephyrproject
west build -p always -b speeder_pdu_f7/stm32f722xx \
  /home/dfl/Projects/PDU_F7_Zephyr \
  -d /home/dfl/Projects/PDU_F7_Zephyr/build_canopen_a -- \
  -DEXTRA_CONF_FILE=variants/a.conf

west build -p always -b speeder_pdu_f7/stm32f722xx \
  /home/dfl/Projects/PDU_F7_Zephyr \
  -d /home/dfl/Projects/PDU_F7_Zephyr/build_canopen_b -- \
  -DEXTRA_CONF_FILE=variants/b.conf
```

These commands compile only. They do not invoke the connected ST-LINK. Do not
connect or activate loads during compile-only verification.

### Temporary A001 SWO diagnostic build

This separate variant keeps the normal A/B configurations unchanged. It sends
boot and five-second safe-state messages through ITM port 0 on PB3 at 2 MHz.

```sh
west build -p always -b speeder_pdu_f7/stm32f722xx \
  /home/dfl/Projects/PDU_F7_Zephyr \
  -d /home/dfl/Projects/PDU_F7_Zephyr/build_swo_a -- \
  -DEXTRA_CONF_FILE=variants/swo_a.conf \
  -DDTC_OVERLAY_FILE=variants/swo.overlay

STM32_Programmer_CLI -c port=SWD freq=1000 mode=UR reset=HWrst \
  -swv freq=216 portnumber=0 /tmp/pdu_swo_live.log -RA
```

The viewer must connect under hardware reset on this target. Hot attach did not
identify the running core. The diagnostic image still reports `A001` through
CANopen SDO `0x100A:00` and does not enable any output-control path.

## Expected verification

After a successful build, verify:

```sh
grep -E 'CONFIG_(CANOPEN_NODE_ID|CAN_DEFAULT_BITRATE|PDU_IMAGE_VARIANT)' \
  build_canopen_a/zephyr/.config build_canopen_b/zephyr/.config
strings build_canopen_a/zephyr/zephyr.bin | grep 'VIIALA_PDU1_V0'
strings build_canopen_b/zephyr/zephyr.bin | grep 'VIIALA_PDU1_V0'
```

Expected: both builds succeed, CAN defaults to 500 kbit/s and node ID 10, the
respective A/B identity is embedded, and flash remains limited to 256 KiB.

The first verified build is recorded in `hardware/BUILD_VERIFICATION.md`.

## Linux CANopen GUI

The `gui/` directory contains the VIIALA PDU CANopen Workbench for the PEAK
PCAN-USB adapter on Linux. It shows live identity, heartbeat, diagnostics and
all fifteen telemetry channels, includes an SDO inspector and CAN traffic view,
and supports guarded output requests, current protection configuration,
confirmed raw SDO writes, volatile communication changes and signed MCUboot
updates.

Launch it with:

```sh
/home/dfl/Projects/PDU_F7_Zephyr/gui/run_pdu_gui.sh
```

See `gui/README.md` for interface setup, testing and the update safety boundary.

## Source of truth

See `hardware/VERIFIED_PINOUT.md`. The board definition was derived from the
updated `SPEEDER_PDU_v2.pdf`, not copied from conflicting historical VIIALA
Zephyr board files.
