# Verified hardware baseline

Authoritative source: `SPEEDER_PDU_v2.pdf`, generated 2026-11-11. The earlier
VIIALA Zephyr board definitions were not used as hardware authority.

## MCU and clocks

- MCU: STM32F722RCT6, LQFP64
- Flash: 256 KiB (the `C` density variant)
- HSE: 16 MHz crystal, ABM8AIG-16.000MHZ-8-1Z-T
- SWD: PA13/SWDIO, PA14/SWCLK, PB3/SWO, NRST

## Verified signal map

| Function | MCU pin | Milestone 1 state |
|---|---:|---|
| OUT1..OUT5 enables | PC0..PC4 | GPIO inactive/low |
| OUT6..OUT15 enables | PC6..PC15 | GPIO inactive/low |
| NCV84045 CS_EN | PB15 | GPIO inactive/low |
| AD7490 CS | PA15 | GPIO inactive/high |
| AD7490 SCK/MISO/MOSI | PA5/PA6/PA7 | Peripheral disabled |
| CAN1 RX/TX | PB8/PB9 | Peripheral disabled |
| UART TX/RX | PA2/PA3 | Peripheral disabled |
| Expansion | PB0, PB1, PB6, PB7, PB10, PB11 | Not configured |

## Safety notes

- The fifteen NCV84045 input nets have hardware pull-downs, defining OFF while
  the MCU is in reset, unpowered, or high-impedance.
- The first application configures all fifteen output enables and CS_EN with
  `GPIO_OUTPUT_INACTIVE` and does not contain any path that turns them on.
- The AD7490 chip select is active-low and is initialized inactive (high).
- CAN1 and SPI1/AD7490 are enabled. UART and persistent settings storage remain disabled.
- Outputs boot inactive and require Operational state, valid ADC data and an explicit SDO request.
- No flash/program command is part of this milestone.

## Conflicts resolved

- Old board definitions disagreed on STM32F722 RC versus ZE and on flash size.
  The updated schematic specifies STM32F722RCT6 and 256 KiB flash.
- An older schematic used an 8 MHz HSE. The updated schematic specifies 16 MHz.
- A legacy firmware comment also referred to a 16 MHz HSE; the updated schematic
  now confirms it.
