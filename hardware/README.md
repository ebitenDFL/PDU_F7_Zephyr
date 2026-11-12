# Speeder PDU - standalone Zephyr bring-up

This repository is an out-of-tree Zephyr application with its own custom board
definition. It does not modify the Zephyr source tree or any existing project.

Milestone 1 is deliberately compile-only:

- all 15 power-output enables initialize inactive;
- current-sense enable initializes inactive;
- ADC chip select initializes inactive;
- CAN, SPI, UART, flash writes, and storage are disabled;
- there are no runner, flash, or debug commands in the project.

## Build in the existing Linux Zephyr workspace

```sh
source /home/dfl/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=/home/dfl/zephyrproject/zephyr
export ZEPHYR_SDK_INSTALL_DIR=/home/dfl/zephyr-sdk-1.0.1
cd /home/dfl/Projects/PDU_F7_Zephyr
cmake -S . -B build -GNinja -DBOARD=speeder_pdu_f7
cmake --build build
```

Compile only. Do not run `west flash` or connect/activate loads for this
milestone.

## Expected verification

After a successful build, verify:

```sh
grep -E 'CONFIG_(CAN|SPI|FLASH|FLASH_MAP|NVS)' build/zephyr/.config
grep -n 'flash@8000000\|safe_controls\|clock-frequency' build/zephyr/zephyr.dts
```

Expected: CAN, SPI, flash, and NVS are disabled; flash is limited to 256 KiB;
the 16 MHz HSE and all safe-control GPIOs appear in the generated devicetree.

The first verified build is recorded in `hardware/BUILD_VERIFICATION.md`.

## Source of truth

See `hardware/VERIFIED_PINOUT.md`. The board definition was derived from the
updated `SPEEDER_PDU_v2.pdf`, not copied from conflicting historical VIIALA
Zephyr board files.
