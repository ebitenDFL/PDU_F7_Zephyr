#!/usr/bin/env bash
set -euo pipefail

project_dir=/home/dfl/Projects/PDU_F7_Zephyr
source /home/dfl/zephyrproject/.venv/bin/activate
exec python3 "$project_dir/gui/pdu_gui.py"
