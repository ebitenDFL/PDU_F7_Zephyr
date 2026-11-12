#!/usr/bin/env bash
set -euo pipefail

project_dir=/home/dfl/Projects/PDU_F7_Zephyr

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

source /home/dfl/zephyrproject/.venv/bin/activate
exec python3 "$project_dir/gui/pdu_gui.py"
