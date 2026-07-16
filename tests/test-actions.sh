#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DECK="$ROOT/bin/kbd-codex"
HERDR_DECK="$ROOT/bin/kbd-herdr"

assert_output() {
    local action="$1"
    local expected="$2"
    local actual
    actual="$(KBD_CODEX_DRY_RUN=1 KBD_CODEX_NOTIFY=0 "$DECK" "$action")"
    [[ "$actual" == "$expected" ]] || {
        printf 'FAIL %s\nexpected:\n%s\nactual:\n%s\n' "$action" "$expected" "$actual" >&2
        exit 1
    }
}

assert_output new $'OPEN codex://threads/new'
assert_output search $'FOCUS codex-desktop\nKEYS 29:1 34:1 34:0 29:0'
assert_output previous $'FOCUS codex-desktop\nKEYS 29:1 42:1 26:1 26:0 42:0 29:0'
assert_output command $'FOCUS codex-desktop\nKEYS 29:1 42:1 25:1 25:0 42:0 29:0'
assert_output act $'FOCUS codex-desktop\nKEYS 28:1 28:0'
assert_output finish $'FOCUS codex-desktop\nTYPE Finish this end to end. Verify it before reporting back.'
assert_output ship $'FOCUS codex-desktop\nTYPE Commit and push the current scoped changes. Keep unrelated work untouched and verify the remote handoff.'
assert_output plan $'FOCUS codex-desktop\nTYPE Make a concise implementation plan, then execute it.'
assert_output go-on $'FOCUS codex-desktop\nTYPE Go on. Continue from the current state and finish the task.'

assert_herdr_output() {
    local action="$1"
    local expected="$2"
    local actual
    actual="$(KBD_HERDR_DRY_RUN=1 "$HERDR_DECK" "$action")"
    [[ "$actual" == "$expected" ]] || {
        printf 'FAIL herdr %s\nexpected:\n%s\nactual:\n%s\n' "$action" "$expected" "$actual" >&2
        exit 1
    }
}

assert_output find $'FOCUS codex-desktop\nKEYS 29:1 33:1 33:0 29:0'
assert_output workspace-prev $'HYPR workspace e-1'
assert_output window-next $'HYPR cyclenext'
assert_output seek-forward $'MEDIA seek 10+'
assert_herdr_output workspace-next $'FOCUS herdr\nHERDR workspace-next'
assert_herdr_output tab-prev $'FOCUS herdr\nHERDR tab-prev'
assert_herdr_output split-right $'FOCUS herdr\nHERDR split-right'
assert_herdr_output pane-up $'FOCUS herdr\nHERDR pane-up'
assert_herdr_output new-workspace $'FOCUS herdr\nHERDR new-workspace'
assert_herdr_output prompt-go-on $'FOCUS herdr\nTYPE go on'
assert_herdr_output prompt-commit-push $'FOCUS herdr\nTYPE commit n push'
assert_herdr_output prompt-resume $'FOCUS herdr\nTYPE Continue from where you left off.'
assert_herdr_output send $'FOCUS herdr\nKEYS 28:1 28:0'

[[ "$(KBD_DICTATE_DRY_RUN=1 "$ROOT/bin/kbd-dictate" start)" == 'DICTATE start' ]]
[[ "$(KBD_DICTATE_DRY_RUN=1 "$ROOT/bin/kbd-dictate" stop)" == 'DICTATE stop' ]]

printf 'All action routing tests passed.\n'
