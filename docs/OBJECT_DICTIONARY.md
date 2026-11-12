# Speeder PDU CANopen object dictionary

This dictionary describes the fifteen protected outputs and their current-sense
channels. All manufacturer-specific telemetry is read-only. Output control is
not exposed by this compile-only milestone.

## Default TPDO broadcast

The four standard TPDOs are asynchronous (`transmission type 254`) with a
100 ms event timer and a 100 ms inhibit time. CANopen multibyte values are
little-endian. PDO transmission starts only after the node enters the CANopen
Operational state.

For the default node ID 10:

| PDO | COB-ID | Byte 0..7 |
|---|---:|---|
| TPDO1 | `0x18A` | Current 1, 2, 3, 4 as four `uint16` mA values |
| TPDO2 | `0x28A` | Current 5, 6, 7, 8 as four `uint16` mA values |
| TPDO3 | `0x38A` | Current 9, 10, 11, 12 as four `uint16` mA values |
| TPDO4 | `0x48A` | Current 13, 14, 15 as three `uint16` mA values, then active-output bitmap |

The output bitmap uses bits 0..14 for outputs 1..15. Bit 15 is reserved and
always zero.

The broadcast consumes 40 classic CAN data frames per second. It is about
10 kbit/s including conservative CAN frame overhead, or roughly 2% of a
500 kbit/s bus before bit stuffing.

## Manufacturer objects

| Index | Type | Meaning | Unavailable value |
|---|---|---|---:|
| `0x2000` | `uint16` | Active output bitmap | - |
| `0x2001` | `uint16` | Commanded output bitmap | - |
| `0x2002` | `uint16` | Combined output-fault bitmap | validity bit clear |
| `0x2003` | `uint16` | Overcurrent-fault bitmap | validity bit clear |
| `0x2004` | `uint16` | Open-load-fault bitmap | validity bit clear |
| `0x2005` | `uint16` | Administratively disabled outputs | validity bit clear |
| `0x2006` | `uint32` | Total current in mA | `0xFFFFFFFF` |
| `0x2007` | `uint16` | Supply voltage in mV | `0xFFFF` |
| `0x2008` | `int16` | Board temperature in 0.1 degC | `0x7FFF` |
| `0x2009` | `int16` | MCU temperature in 0.1 degC | `0x7FFF` |
| `0x200A` | `uint32` | Uptime in seconds | - |
| `0x200B` | `uint16` | Reset-reason code | `0xFFFF` |
| `0x200C` | `uint16` | Brownout counter | validity bit clear |
| `0x200D` | `uint16` | CAN bus-off counter | validity bit clear |
| `0x200E` | `uint16` | Warning flags | validity bit clear |
| `0x200F` | `uint32` | Telemetry-validity bitmap | - |
| `0x3000:01..0F` | `uint16[15]` | Output currents in mA | `0xFFFF` |
| `0x3001:01..0F` | `uint16[15]` | AD7490 raw counts | `0xFFFF` |
| `0x3002:01..0F` | `uint16[15]` | Peak output currents in mA | `0xFFFF` |
| `0x3003:01..0F` | `uint32[15]` | Overcurrent trip counters | validity bit clear |

The existing CANopenNode diagnostic objects at `0x2100..0x2102` remain
available.

## Telemetry validity (`0x200F`)

| Bits | Meaning |
|---:|---|
| 0 | Active and commanded output states valid |
| 1..15 | Current channels 1..15 valid |
| 16 | Total current valid |
| 17 | Supply voltage valid |
| 18 | Board temperature valid |
| 19 | MCU temperature valid |
| 20 | Fault masks valid |
| 21 | Reset, brownout and CAN diagnostic counters valid |
| 22..31 | Reserved |

At this milestone only bit 0 is set. SPI and the AD7490 are still disabled, so
current and raw-ADC values deliberately remain unavailable. When all fifteen
current channels have valid samples, the telemetry helper calculates total
current and sets bit 16.

## Hardware measurement boundary

The updated schematic connects fifteen NCV84045 current-sense outputs to the
AD7490. The ADC is 12-bit with a 2.5 V reference. Its sixteenth input is not a
battery-voltage channel. `0x2007` therefore stays unavailable until a verified
voltage-measurement source is added.

## Useful future motorcycle signals

These are deliberately not assigned indexes until their source, scale and
ownership are defined:

- ignition, kill-switch and engine-running states;
- engine speed, vehicle speed and selected gear received from the vehicle CAN;
- minimum cranking voltage and brownout duration;
- per-output energy or amp-hour accumulation;
- board thermal derating state;
- load short-to-battery, short-to-ground and intermittent-fault counters;
- CAN receive age for safety-critical input signals;
- service-hour and key-cycle counters.

Signal age and validity should accompany any values consumed from another ECU;
the PDU must not continue treating stale motorcycle data as current data.

## Source files

- `objdict/objdict.eds`: canonical commissioning description for variant A.
- `objdict/speeder_pdu_A001.eds` and `speeder_pdu_B001.eds`: matching variant
  files.
- `objdict/objdict.xml`: editable CANopenEditor project.
- `objdict/CO_OD.c` and `CO_OD.h`: firmware representation.
- `src/pdu_telemetry.c`: safe runtime publication helper.
