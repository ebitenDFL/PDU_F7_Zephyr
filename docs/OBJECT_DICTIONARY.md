# VIIALA_PDU1_V0 CANopen object dictionary

This dictionary describes the fifteen protected outputs and their current-sense
channels. Output requests and protection settings are writable through SDO.
Every output boots OFF; actuation additionally requires CANopen Operational
state and valid current telemetry. Losing either condition turns every output
OFF and clears the request bitmap, so restoration cannot re-energize a channel
without a new SDO request.

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
| `0x2103` | `uint16` | Requested output bitmap, bits 0..14 | - |
| `0x2104` | `uint16` | Fault-clear pulse bitmap; requested output must be OFF | - |
| `0x2105` | `uint8` | Pending CANopen node ID, 1..127 | - |
| `0x2106` | `uint16` | Pending bitrate: 125, 250, 500 or 1000 kbit/s | - |
| `0x2107` | `uint8` | Write `0xA5` to apply communication settings | - |
| `0x3100:01..0F` | `uint16[15]` | Continuous current limit in mA | - |
| `0x3101:01..0F` | `uint16[15]` | Continuous overcurrent delay in ms | - |
| `0x3102:01..0F` | `uint16[15]` | Inrush current limit in mA | - |
| `0x3103:01..0F` | `uint16[15]` | Inrush allowance time in ms | - |

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

The firmware samples all fifteen AD7490 current channels every 5 ms. Until a
valid scan completes the current values remain `0xFFFF`, output requests cannot
energize hardware, and the total-current value remains `0xFFFFFFFF`.

## Hardware measurement boundary

The updated schematic connects fifteen NCV84045 current-sense outputs to the
AD7490. The ADC is 12-bit with a 2.5 V reference. Its sixteenth input is not a
battery-voltage channel. `0x2007` therefore stays unavailable until a verified
voltage-measurement source is added.

The initial conversion uses the NCV84045 typical current-sense ratio of 1230
and the fitted 301 ohm sense resistor: approximately 2.49 mA per ADC count.
This is a nominal engineering conversion, not a production calibration. Raw
counts at `0x3001` are retained so each hardware revision can be calibrated.
Inactive outputs report zero current even when the raw ADC channel carries an
NCV84045 OFF-state diagnostic level. The raw value remains visible at `0x3001`.

Default protection per output is 5.0 A continuous, 100 ms continuous trip
delay, 8.0 A inrush, and 500 ms inrush allowance. Accepted ranges are
0.1–10 A, 1–5000 ms continuous delay, 0.1–10 A inrush and 0–10000 ms inrush
allowance. An inrush-limit exceedance trips on the next 5 ms sample; after the
inrush window, the continuous limit must remain exceeded for its configured
delay. Faults latch OFF and require an OFF request before clearing.

Node ID and bitrate changes are deliberately RAM-only because the 256 KiB
internal flash layout has no safe settings erase sector. They survive the
CANopen communication reset but revert to node 10 at 500 kbit/s after power
loss.

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
- `objdict/objdict.xml`: retained legacy CANopenEditor project; the EDS and
  generated C files are authoritative for the new writable objects.
- `objdict/CO_OD.c` and `CO_OD.h`: firmware representation.
- `src/pdu_telemetry.c`: safe runtime publication helper.
