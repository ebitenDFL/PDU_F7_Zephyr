#!/usr/bin/env bash
set -euo pipefail

gui_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
if [[ "${1:-}" == --python ]]; then
	python="${2:?--python requires a Python interpreter}"
	shift 2
else
	source "$gui_dir/../scripts/setup-env.sh"
	python=python3
fi

if ! "$python" -c 'import tkinter, can, canopen'; then
	echo "The selected Python environment needs tkinter, python-can and canopen." >&2
	echo "Set PDU_VENV to an environment with these dependencies, or reinstall the desktop entry." >&2
	exit 1
fi
if [[ "${1:-}" == --check-dependencies ]]; then
	exit 0
fi
can_interface="${PDU_CAN_IFACE:-can0}"
can_bitrate="${PDU_CAN_BITRATE:-500000}"

# An SSH shell does not inherit the graphical session. Reuse the active
# Xwayland environment so the window opens on the Linux laptop's desktop.
if [[ -z "${DISPLAY:-}" ]]; then
	desktop_pid="$(pgrep -u "$(id -u)" -x Xwayland | head -n 1 || true)"
	if [[ -z "$desktop_pid" || ! -r "/proc/$desktop_pid/environ" ]]; then
		echo "No active Linux desktop display was found." >&2
		echo "Log in graphically, or connect with SSH X forwarding (ssh -X)." >&2
		exit 1
	fi
	for variable in DISPLAY XAUTHORITY WAYLAND_DISPLAY XDG_RUNTIME_DIR DBUS_SESSION_BUS_ADDRESS; do
		value="$(tr '\0' '\n' <"/proc/$desktop_pid/environ" | sed -n "s/^${variable}=//p" | head -n 1)"
		if [[ -n "$value" ]]; then
			export "$variable=$value"
		fi
	done
fi

# Leave an already-correct interface untouched. If configuration is needed,
# use an existing passwordless sudo rule or request elevation graphically.
set +e
"$gui_dir/setup_can.sh" "$can_interface" "$can_bitrate"
can_status=$?
set -e
if (( can_status == 77 )); then
	if ! sudo -n "$gui_dir/setup_can.sh" "$can_interface" "$can_bitrate" 2>/dev/null; then
		if command -v pkexec >/dev/null 2>&1; then
			pkexec "$gui_dir/setup_can.sh" "$can_interface" "$can_bitrate"
		else
			echo "Administrator permission is required to start $can_interface." >&2
			exit 1
		fi
	fi
elif (( can_status != 0 )); then
	echo "Unable to prepare $can_interface for the PDU workbench." >&2
	exit "$can_status"
fi

exec "$python" "$gui_dir/pdu_gui.py"
