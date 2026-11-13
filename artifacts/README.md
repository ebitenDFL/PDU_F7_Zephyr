# Firmware artifacts

The functional 2026-11-13 Zephyr 4.5.0 A/B images are:

- `VIIALA_PDU1_V0_A001_DEV_KEY_BENCH_ONLY.bin`
- `VIIALA_PDU1_V0_B001_DEV_KEY_BENCH_ONLY.bin`

They include AD7490 current monitoring, IN15 input-voltage monitoring,
factory-calibrated STM32 die temperature and VDDA telemetry, configurable
continuous/inrush protection, guarded output requests and volatile
node-ID/bitrate configuration. Both compile inside the 64 KiB active slot.
The A and B images were transferred and booted over CAN on the powered bench
unit; B001 was left active with all output/request/fault bitmaps zero.

**Bench use only:** these images use MCUboot's public development signing key.
Do not use them for field or production deployment.

## Historical compile-only artifacts

These are MCUboot-format CANopen update images from the compile-feasibility
build. They use hash-only verification and have not been tested on hardware.

**Do not flash or transfer these images to a PDU.** Production use requires a
protected signing key and bench validation of update, interruption, power-loss,
activation, and recovery behavior.

- `CANOPEN_UPDATE_A001_HASH_ONLY_DO_NOT_FLASH.bin` reports `A001` at SDO
  `0x100A:00`.
- `CANOPEN_UPDATE_B001_HASH_ONLY_DO_NOT_FLASH.bin` reports `B001` at SDO
  `0x100A:00`.
