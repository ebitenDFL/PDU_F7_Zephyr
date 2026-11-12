# VIIALA_PDU1_V0 CANopen Workbench

Linux desktop GUI for the custom Zephyr PDU firmware. It uses SocketCAN and
the PEAK PCAN-USB adapter through `can0` at 500 kbit/s.

## Run

Bring `can0` up first:

```sh
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000 listen-only off restart-ms 100
sudo ip link set can0 up
```

Then launch:

```sh
/home/dfl/Projects/PDU_F7_Zephyr/gui/run_pdu_gui.sh
```

The launcher uses the existing Zephyr virtual environment, which already
contains Python CANopen and SocketCAN support. No packages are installed by the
GUI.

## Features and safety boundary

- live identity, heartbeat, NMT state and diagnostic status;
- fifteen-channel state/current/ADC/peak/trip table;
- raw SDO inspector and relevant CAN-frame viewer;
- guarded per-output ON/OFF requests and latched-fault clear;
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
