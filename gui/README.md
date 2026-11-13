# VIIALA_PDU1_V0 CANopen Workbench

Linux desktop GUI for the custom Zephyr PDU firmware. It uses SocketCAN and
the PEAK PCAN-USB adapter through `can0` at 500 kbit/s.

## Run

Connect the PEAK adapter, then launch:

```sh
/home/dfl/Projects/PDU_F7_Zephyr/gui/run_pdu_gui.sh
```

The launcher checks `can0` and leaves it untouched when it is already active at
500 kbit/s. Otherwise it configures and starts the interface with automatic
bus-off recovery. Linux may show one administrator-password prompt because CAN
network configuration requires elevated permission. The launcher uses the
existing Zephyr virtual environment, which already contains Python CANopen and
SocketCAN support. No packages are installed by the GUI.

Advanced users can override the defaults with `PDU_CAN_IFACE` and
`PDU_CAN_BITRATE`, for example `PDU_CAN_BITRATE=250000 run_pdu_gui.sh`.

## Features and safety boundary

- live identity, heartbeat, NMT state and diagnostic status;
- fifteen-channel state/current/ADC/peak/trip table with dedicated ON, OFF and
  Reset buttons on every row;
- raw SDO inspector and relevant CAN-frame viewer;
- guarded per-output ON requests, immediate OFF requests and an output Reset
  command that first forces the selected output OFF and then clears its latched
  fault;
- continuous current, trip delay, inrush current and inrush time editing;
- volatile node-ID and bitrate change with communication reset;
- confirmed raw SDO writes using little-endian hexadecimal bytes;
- NMT Pre-operational, Start, Stop and Reset commands;
- guarded signed MCUboot `.bin` transfer with a ten-second staging erase
  timeout, progress display, reboot identity check and confirmation;

An ON request only actuates when the node is Operational and current sampling
is valid. Check the load and protection limits before confirming. Communication
settings revert to node 10 / 500 kbit/s after a power cycle.

The current update layout is overwrite-only and has no rollback. The repository
uses MCUboot's public development signing key for bench testing; it must be
replaced before field deployment.

## Test without hardware

```sh
source /home/dfl/zephyrproject/.venv/bin/activate
cd /home/dfl/Projects/PDU_F7_Zephyr/gui
python3 -m unittest -v test_pdu_can.py
```
