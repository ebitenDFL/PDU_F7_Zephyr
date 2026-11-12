# Milestone 1 build verification

Verified on the Linux laptop on 2026-11-11.

- Project: `/home/dfl/Projects/PDU_F7_Zephyr`
- Zephyr: 4.4.99 (`v4.4.0-2681-g53689933c524`)
- SDK/toolchain: Zephyr SDK 1.0.1, GCC 14.3.0
- Board target: `speeder_pdu_f7/stm32f722xx`
- Result: successful compile and link; no flashing performed

## Linker result

| Region | Used | Available | Utilization |
|---|---:|---:|---:|
| Flash | 23,496 bytes | 256 KiB | 8.96% |
| RAM | 4,480 bytes | 192 KiB | 2.28% |
| DTCM | 0 bytes | 64 KiB | 0% |

The generated configuration selects `SOC_STM32F722XX` and a 216 MHz system
clock derived from the verified 16 MHz HSE. The generated flash node is limited
to `0x40000` bytes (256 KiB).

The generated configuration confirms CAN, SPI, serial console, and flash access
are disabled. NVS and flash-map support are not selected. The generated
devicetree contains all 15 inactive output-enable pins, inactive CS_EN, and the
inactive active-low ADC chip select.

The build emitted a devicetree unit-address warning for the STM32 factory-system
flash/calibration node inherited from Zephyr's STM32F722 include. It is unrelated
to the application flash node at `0x08000000`; the build completed successfully.

# Milestone 2 CANopen build verification

Verified on the Linux laptop on 2026-11-11. Both `A001` and `B001` variants
compiled and linked successfully; no flashing was performed.

| Region | Used | Available | Utilization |
|---|---:|---:|---:|
| Flash | 46,836 bytes | 256 KiB | 17.87% |
| RAM | 10,816 bytes | 192 KiB | 5.50% |

The generated configuration confirms CANopen node ID 10 and CAN bitrate
500,000 bit/s. Binary string inspection confirms image A contains
`SPEEDER_PDU_F7HW20A001`, while image B contains
`SPEEDER_PDU_F7HW20B001`.

The program-download objects are present in the dictionary, but MCUboot,
CANopen program download, flash writes, and persistent storage remain disabled.

# MCUboot/CANopen update feasibility verification

Both `update_a.conf` and `update_b.conf` compiled and linked with MCUboot in
overwrite-only mode. No firmware was flashed and no hardware was activated.

| Image | Flash used | Region | Utilization |
|---|---:|---:|---:|
| MCUboot | 18,860 bytes | 64 KiB | 28.78% |
| CANopen update app A | 52,312 bytes | 64 KiB | 79.82% |
| CANopen update app B | 52,312 bytes | 64 KiB | 79.82% |

The MCUboot-format application binaries are 52,352 bytes including the image
header. Binary inspection confirms A contains `A001` and B contains `B001`.
CANopen program download, flash page layout, MCUboot integration, and
overwrite-only mode are selected in both generated configurations.

The shared Zephyr driver needed the project-owned 256 KiB flash-layout patch
during compilation. A guarded build applied and reversed it automatically;
the original driver file was verified clean afterward.

This establishes compile feasibility only. Hash-only MCUboot images are not
production-safe and no runtime flash, transfer interruption, boot activation,
or recovery behavior has been tested.

# Milestone 3 telemetry object-dictionary verification

Verified on the Linux laptop on 2026-11-11. Both normal variants and both
MCUboot/CANopen update variants compiled and linked successfully. The connected
ST-LINK was not accessed and no firmware was flashed.

## Normal A/B variants

| Region | Used | Available | Utilization |
|---|---:|---:|---:|
| Flash | 47,568 bytes | 256 KiB | 18.15% |
| RAM | approximately 11 KiB | 192 KiB | 5.73% |

The generated configurations confirm node ID 10, 500,000 bit/s CAN, the
CANopen TPDO worker enabled at a 1 ms period, and the correct A/B variant.
Binary inspection confirms `SPEEDER_PDU_F7HW20A001` and
`SPEEDER_PDU_F7HW20B001` respectively.

The runtime object dictionary contains 65 entries. TPDO1 through TPDO4 map
`0x3000:01..0F` (fifteen `uint16` current values in mA) and `0x2000:00`
(the active-output bitmap). All four TPDOs use a 100 ms event timer and a
100 ms inhibit time.

The EDS has unique section names, its manufacturer-object list contains all
23 manufacturer-specific indexes, and the XML source parses successfully with
unique object indexes. Matching A001 and B001 EDS files contain the correct
software identity.

## Update variants after dictionary expansion

| Image | Flash used | Region | Utilization |
|---|---:|---:|---:|
| MCUboot | 18,860 bytes | 64 KiB | 28.78% |
| CANopen update app A | 53,044 bytes | 64 KiB | 80.94% |
| CANopen update app B | 53,044 bytes | 64 KiB | 80.94% |

The generated hash-only update files remain 52,352 bytes each. SHA-256:

- A001: `17f775620e9f5f066b648db8d2dbb666958026fdfa887477014f252b9143130e`
- B001: `2242e84c9d59e869b294a1fc2ec4f360ef46c2f893ad7de57d750c690c942ee1`

The guarded flash-layout patch was reversed after the build; the shared Zephyr
flash driver was verified clean. These images remain compile-only and must not
be flashed.

# First hardware programming verification

Performed on 2026-11-11 after explicit authorization.

- Probe: STLINK-V3SET, firmware `V3J17M10B6S1`
- Target voltage: 3.28-3.29 V
- Device ID: `0x452`
- Target reported by STM32CubeProgrammer: STM32F72x/STM32F73x, revision A,
  Cortex-M7, 256 KiB NVM
- Image: normal standalone `A001` build
- Image SHA-256: `7d732ef670449c0f9bf4d3f262c50a6cea13f4ac3f1a9b7effd20d7a29f5e46f`
- Link/load address: `0x08000000`
- Programmed size: 47,568 bytes
- Erased sectors: 0 through 2 only
- Programmer result: download complete, byte verification successful, MCU
  software reset performed

The normal image initializes all fifteen high-side control pins inactive before
starting CANopen. SPI and the current-sense ADC remain disabled, so current PDO
values are `0xFFFF` with their validity bits clear. No output activation was
attempted. Runtime CAN reception, NMT transition, SDO access and TPDO traffic
have not yet been observed on a CAN analyzer.

No MCUboot or hash-only update image was flashed.

# SWO diagnostic programming and runtime verification

Performed on 2026-11-11 after explicit authorization.

- Variant: temporary standalone `A001-SWO`; normal A/B configurations unchanged
- SWO: PB3, ITM stimulus port 0, NRZ, 216 MHz reference, 2 MHz output
- Flash used: 56,808 bytes of 256 KiB (21.67%)
- RAM used: approximately 11 KiB of 192 KiB (5.73%)
- HEX SHA-256: `a6214489a6ceb246214ec7a87ae367fee3999bd1596c3148d4107b84cfa25051`
- Programmed address: `0x08000000`
- Programmer: STLINK-V3SET at 3.29 V and 1 MHz SWD
- Result: download and byte verification successful; MCU reset performed

STM32CubeProgrammer SWV connected under hardware reset and received:

```text
SPEEDER PDU A001-SWO boot: all 15 power outputs inactive
CANopen node 10 at 500 kbit/s; ADC and SPI disabled
CANopen is running; outputs remain inactive
heartbeat uptime=5 s outputs=0x0000
heartbeat uptime=10 s outputs=0x0000
heartbeat uptime=15 s outputs=0x0000
```

Hot attach could not identify the already-running core, so the working viewer
command uses under-reset/hardware-reset connection. A persistent viewer was
opened on the Linux desktop. No output activation was attempted.

# Signed CANopen firmware-update verification

Performed on 2026-11-11 with the PEAK PCAN-USB interface on Linux `can0` at
500 kbit/s. The CAN dump window remained open throughout the test.

- MCUboot RSA verification used its published development key (bench only).
- MCUboot linked at `0x08000000`; signed applications linked at `0x08010000`.
- MCUboot size: 29,704 bytes in the 64 KiB boot partition.
- Signed A/B image size: 53,384 bytes each in the 64 KiB active-image limit.
- MCUboot HEX SHA-256:
  `a1c7229404b8490daf5edeb0aa1e0be01b5a3401921bf93eaeb38429f4dbb041`
- Signed A HEX SHA-256:
  `f3fb4243ccdc07c45315e42fbe8c600da461db533caac949d9a62439932a1952`
- Signed B transfer BIN SHA-256:
  `bc1874756fd580635189fd8ed0c3d3ac5cb615e00ccecaf41bbe17abc2ebcd9d`

The one-time ST-LINK installation of MCUboot and signed A downloaded and
byte-verified successfully. CANopen then reported `A001`, program control 1,
flash status 0, and a Pre-operational heartbeat.

The initial staging-clear request exceeded the client's one-second SDO timeout
while erasing the 128 KiB sector. It nevertheless completed cleanly, reporting
program control 3 and flash status 0. The transfer resumed with a five-second
SDO timeout. All 53,384 bytes of signed B transferred in approximately 23
seconds. MCUboot activated it, the node rebooted, SDO `0x100A:00` returned
`B001`, and the new image was confirmed.

Independent post-update reads confirmed program control 1, flash status 0,
output bitmap `0x0000`, Pre-operational heartbeat `0x7F`, and zero CAN error
counters. This remains an overwrite-only layout without rollback, and the
development signing key must not be used for production.

# Functional current-protection A/B build

Compile-only verification performed on 2026-11-12. Nothing was flashed and no
output was activated during this verification.

- Device identity: `VIIALA_PDU1_V0`; hardware `HW20`
- Variant identities: `A001` and `B001`
- AD7490 current sampling: all 15 NCV84045 current-sense channels every 5 ms
- Application flash: 57,984 / 65,152 bytes (89.00%)
- Application RAM: 12,416 / 196,608 bytes (6.32%)
- Signed update size: 58,320 bytes each
- A signed-bin SHA-256:
  `d30274d3afb74b6b6b6a29bbd66d627c45752bded0598ce1b114a6bd4116fed2`
- B signed-bin SHA-256:
  `685d5c59401192bef77d45869a01cf85058ec121064c3d806e052ef52c1f6e40`
- Both EDS variants import successfully with 74 objects.
- GUI Python syntax and all protocol-helper unit tests pass.
- The temporary STM32F722RC flash-layout patch was removed cleanly after the
  build; the shared Zephyr driver has no remaining modification.

The protection logic has not yet been electrically validated against a known
load. Before normal use, bench-test raw ADC counts, current scaling, each output
mapping, continuous-limit timing, inrush timing and latched-fault recovery.
The images use MCUboot's public development key and are for controlled bench
use only.

## AD7490 mode correction and CAN update

Performed on 2026-11-12 after comparing the Zephyr driver with the known-good
VI_PDU3 CubeIDE implementation.

- Changed SPI from mode 2 to the hardware-verified mode 0 (CPOL=0, CPHA=0).
- Replaced two `0xFFFF` startup transfers with one valid channel-0 control
  transfer.
- Inactive outputs now publish 0 mA while retaining the NCV84045 OFF-state
  diagnostic level in the raw ADC object.
- A newly requested output skips the stale OFF-state ADC sample before current
  protection begins.
- Both A and B compiled at 58,000 / 65,152 bytes application flash (89.02%).
- Signed A/B update size: 58,336 bytes each.
- A signed-bin SHA-256:
  `3c6f6928749df4b9247557f1904cda88536d51cdf5fb024368404e720a013a9d`
- B signed-bin SHA-256:
  `8e6054107f5fe0c5c78f665f1c5651b4de3828b38014b13ecc853bc56e07b756`

The corrected A image was installed over CAN after confirming active and
requested output bitmaps were both zero. MCUboot activation succeeded and SDO
reads confirmed `VIIALA_PDU1_V0`, `A001`, output bitmap zero, request bitmap
zero, raw inactive-channel readings 4095, and published inactive currents
0 mA. Current scaling under a known powered load remains to be validated.
