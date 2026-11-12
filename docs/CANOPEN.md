# CANopen milestone

This compile-only milestone enables CAN1 on PB8/PB9 at 500 kbit/s and uses
CANopen node ID 10. All 15 power-output controls and the current-sense enable
are configured inactive before CAN is placed in normal mode.

Four TPDOs broadcast all fifteen output-current channels in mA plus the active
output-state bitmap at 10 Hz. Until the SPI/ADC driver is implemented, current
values are the explicit unavailable value `0xFFFF`; they are never presented as
false zero-current measurements. See `docs/OBJECT_DICTIONARY.md` for the byte
layout, SDO indexes, units and validity bits.

## Image identity

Two builds are provided. A single upload from standard SDO object `0x100A:00`
returns the active image identity:

- variant A: ASCII `A001`
- variant B: ASCII `B001`

The fixed device name at `0x1008:00` is `SPEEDER_PDU_F7`, and hardware version
`0x1009:00` is `HW20`.

Matching commissioning files are `objdict/speeder_pdu_A001.eds` and
`objdict/speeder_pdu_B001.eds`. `objdict/objdict.xml` is the editable dictionary
source and `CO_OD.c`/`CO_OD.h` are the firmware representation with a small
compile-time A/B identity selection.

## Firmware download path

The dictionary contains the CiA 302-3 draft program-download objects at
`0x1F50`, `0x1F51`, `0x1F56`, and `0x1F57`. The normal A/B smoke variants keep
their handler disabled. The `update_a.conf` and `update_b.conf` variants enable
the handler, MCUboot integration, flash image buffering, CRC, and reboot.

The fitted STM32F722RCT6 has only 256 KiB internal flash with uneven erase
sectors. The compile-tested layout is:

- `0x00000..0x0FFFF`: 64 KiB MCUboot (four 16 KiB sectors)
- `0x10000..0x1FFFF`: 64 KiB active image/slot 0 (one 64 KiB sector)
- `0x20000..0x3FFFF`: 128 KiB download staging/slot 1 (one 128 KiB sector)

This uses MCUboot **overwrite-only** mode. Slot 1 stages the new image and
MCUboot copies it into slot 0. It is not a true rollback-capable A/B scheme;
the previous firmware is not retained after activation. The internal erase
geometry does not provide two practical, symmetric application slots plus a
bootloader. External flash or a larger MCU is recommended if automatic rollback
is a requirement.

Zephyr 4.4.99 lacks the STM32F722xC 256 KiB page layout. The project therefore
contains `patches/zephyr-stm32f722rc-256k-flash-layout.patch` and a guarded build
script which applies it only while compiling and reverses it on exit. The shared
Zephyr checkout remains unchanged after the build.

The update path was bench-tested on 2026-11-11 using MCUboot RSA verification
with MCUboot's published development key. It successfully transferred a signed
53,384-byte B image over CANopen, rebooted through MCUboot, changed SDO
`0x100A:00` from `A001` to `B001`, and confirmed the new image. The development
key is public and is **not approved for production or field deployment**.
Production images still require a protected project signing key, interrupted
transfer and power-loss testing, and a defined ST-LINK recovery procedure.

The MCUboot child build uses `sysbuild/mcuboot.overlay` so it links at
`0x08000000`; the application remains at `0x08010000`. Always audit both HEX
addresses before programming.

The STM32 128 KiB staging-sector erase can exceed the CANopen client's default
one-second SDO timeout. Use at least a five-second SDO timeout. If a client
times out while clearing, read `0x1F51:01` and `0x1F57:01` before retrying; a
successful completed erase reports program control `0x03` and flash status 0.

## Build variants

Build A with `-DEXTRA_CONF_FILE=variants/a.conf` and B with
`-DEXTRA_CONF_FILE=variants/b.conf`, using separate build directories.

For the bench-signed MCUboot/update builds, run
`scripts/build_canopen_updates.sh`. The configured RSA key is for development
testing only and must be replaced before any production use.
