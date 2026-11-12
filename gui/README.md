# SPEEDER PDU CANopen Workbench

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
- NMT Pre-operational, Start, Stop and Reset commands;
- guarded signed MCUboot `.bin` transfer with a ten-second staging erase
  timeout, progress display, reboot identity check and confirmation;
- no output-switching controls because the current safe firmware deliberately
  exposes no output-control object.

The current update layout is overwrite-only and has no rollback. The repository
uses MCUboot's public development signing key for bench testing; it must be
replaced before field deployment.

## Test without hardware

```sh
source /home/dfl/zephyrproject/.venv/bin/activate
cd /home/dfl/Projects/PDU_F7_Zephyr/gui
python3 -m unittest -v test_pdu_can.py
```
