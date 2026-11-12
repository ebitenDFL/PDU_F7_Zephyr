# Compile-only artifacts

These are MCUboot-format CANopen update images from the compile-feasibility
build. They use hash-only verification and have not been tested on hardware.

**Do not flash or transfer these images to a PDU.** Production use requires a
protected signing key and bench validation of update, interruption, power-loss,
activation, and recovery behavior.

- `CANOPEN_UPDATE_A001_HASH_ONLY_DO_NOT_FLASH.bin` reports `A001` at SDO
  `0x100A:00`.
- `CANOPEN_UPDATE_B001_HASH_ONLY_DO_NOT_FLASH.bin` reports `B001` at SDO
  `0x100A:00`.
