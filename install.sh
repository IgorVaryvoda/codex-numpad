#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HYPR_USER_CONFIG="${HYPR_USER_CONFIG:-$HOME/.config/hypr/UserConfigs/UserKeybinds.conf}"
HYPR_LINK="$HOME/.config/hypr/UserConfigs/CodexNumpad.conf"
BIN_LINK="$HOME/.local/bin/kbd-codex"
HERDR_BIN_LINK="$HOME/.local/bin/kbd-herdr"
DICTATE_BIN_LINK="$HOME/.local/bin/kbd-dictate"
SOURCE_LINE='source = $HOME/.config/hypr/UserConfigs/CodexNumpad.conf'
FLASH=0

if [[ "${1:-}" == '--flash' ]]; then
    FLASH=1
elif [[ -n "${1:-}" ]]; then
    printf 'Usage: %s [--flash]\n' "$0" >&2
    exit 2
fi

for command in git make hyprctl ydotool jq; do
    command -v "$command" >/dev/null 2>&1 || {
        printf 'Missing required command: %s\n' "$command" >&2
        exit 1
    }
done

mkdir -p "$(dirname "$BIN_LINK")" "$(dirname "$HYPR_LINK")"
ln -sfn "$ROOT/bin/kbd-codex" "$BIN_LINK"
ln -sfn "$ROOT/bin/kbd-herdr" "$HERDR_BIN_LINK"
ln -sfn "$ROOT/bin/kbd-dictate" "$DICTATE_BIN_LINK"
ln -sfn "$ROOT/hypr/codex-numpad.conf" "$HYPR_LINK"

if ! grep -Fqx "$SOURCE_LINE" "$HYPR_USER_CONFIG"; then
    printf '\n# Codex Deck / DMQ SPIN\n%s\n' "$SOURCE_LINE" >> "$HYPR_USER_CONFIG"
fi

hyprctl reload >/dev/null
"$ROOT/scripts/qmk-build" build

if [[ "$FLASH" == 1 ]]; then
    printf '\nPut the SPIN into bootloader mode when QMK asks for it.\n'
    "$ROOT/scripts/qmk-build" flash
else
    printf '\nInstalled and compiled. To flash the board:\n  %s --flash\n' "$0"
fi
