#!/usr/bin/env bash
set -euo pipefail

gui_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
source "$gui_dir/../scripts/setup-env.sh"
python="$(python3 -c 'import os, sys; print(os.path.abspath(sys.executable))')"
"$gui_dir/run_pdu_gui.sh" --python "$python" --check-dependencies

applications_dir="${XDG_DATA_HOME:-"$HOME/.local/share"}/applications"
"$python" - "$gui_dir" "$applications_dir" <<'PY'
import sys
from pathlib import Path


def exec_argument(value):
    if any(character in value for character in "\r\n"):
        raise SystemExit("Desktop launcher paths cannot contain line breaks.")
    # Escape field codes, then Exec quoting, then desktop entry string escaping.
    value = value.replace("%", "%%")
    value = "".join("\\" + char if char in '\\"$`' else char for char in value)
    return '"' + value.replace("\\", "\\\\").replace("\t", "\\t") + '"'


gui_dir, applications_dir = map(Path, sys.argv[1:])
template = (gui_dir / "speeder-pdu.desktop").read_text(encoding="utf-8")
arguments = (str(gui_dir / "run_pdu_gui.sh"), "--python", sys.executable)
command = " ".join(map(exec_argument, arguments))
applications_dir.mkdir(parents=True, exist_ok=True)
entry = applications_dir / "speeder-pdu.desktop"
entry.write_text(template.replace("@PDU_GUI_EXEC@", command), encoding="utf-8")
print(f"Installed {entry}")
PY
