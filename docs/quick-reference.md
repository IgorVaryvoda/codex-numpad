# Codex Deck — Quick Reference

DMQ Design SPIN · 12 keys · 3 clickable knobs · interactive version:
[`cheatsheet.html`](cheatsheet.html)

All three LEDs show the active mode together: **blue** for Codex, **cyan** for
the Codex prompt bank, **orange** for Media, and **green** for Herdr. A single
knob press changes all three LEDs; no double-click is required. Red means the
bootloader chord is being held.

## Global controls

| Control | Action |
|---|---|
| Turn bottom knob | Volume down/up in every mode |
| Tap top knob | Switch to **Codex** mode |
| Tap top knob again in Codex mode | Toggle the **Codex prompt bank** |
| Tap middle knob | Switch to **Media** mode |
| Tap bottom knob | Switch to **Herdr** mode |
| Hold bottom knob for at least 350 ms | Dictate globally; release to transcribe, type, and restore the previous mode |
| Hold all three knobs together for 2 seconds | Enter the QMK bootloader |

Dictation runs locally with Whisper and types into the field that currently has
the cursor. The Codex-mode **Dictation** key uses tap-to-start, tap-to-stop
instead of push-to-talk.

## Codex mode — blue LEDs

| | | |
|---|---|---|
| **Focus Codex** | **New task** | **Search tasks** |
| **Back** | **Quick Chat** | **Forward** |
| **Previous task** | **Find in task** | **Next task** |
| **Dictation toggle** | **Command menu** | **Enter / Act** |

| Knob | Turn left/right |
|---|---|
| Top | Previous/next Hyprland workspace |
| Middle | Previous/next window |
| Bottom | Volume down/up |

## Codex prompt bank — cyan LEDs

Tap the top knob while already in Codex mode to toggle this bank; tap it again
to return. Each key types a ready-made prompt into Codex without sending it, so
you can edit or extend it first. **Enter / Act** submits, in the same
bottom-right position as in Codex mode. Rows follow the work lifecycle: start,
fix, understand, ship.

| | | |
|---|---|---|
| **Plan** | **Go on** | **Finish** |
| **Fix** | **Test-fix** | **Review-fix** |
| **Status** | **Explain** | **Docs** |
| **Commit** | **Ship** | **Enter / Act** |

The knobs keep their Codex-mode behavior: workspaces, windows, and volume.

## Media mode — orange LEDs

| | | |
|---|---|---|
| **Home** | **Up** | **End** |
| **Left** | **Down** | **Right** |
| **Page Up** | **Mute** | **Page Down** |
| **Play/Pause** | **Previous track** | **Next track** |

| Knob | Turn left/right |
|---|---|
| Top | Screen brightness down/up |
| Middle | Seek active media backward/forward 10 seconds |
| Bottom | Volume down/up |

## Herdr mode — green LEDs

The pane cross sits on the same physical keys as the Media-mode arrows, so
directional muscle memory carries between the two modes.

| | | |
|---|---|---|
| **Focus Herdr** | **Pane up** | **New tab** |
| **Pane left** | **Pane down** | **Pane right** |
| **Previous agent** | **New workspace** | **Next agent** |
| **Split right** | **Zoom pane** | **Split down** |

| Knob | Turn left/right |
|---|---|
| Top | Previous/next Herdr workspace |
| Middle | Previous/next Herdr tab |
| Bottom | Volume down/up |

## Build, diagnose, and flash

```bash
make doctor
make build
make flash
```

On a first install, press the tiny side-mounted reset button directly underneath
the USB-C connector when the flasher starts waiting. After installing this
firmware, hold all three knobs together for a full two seconds. Keep holding
after all three LEDs turn red, until the device disconnects into the bootloader.
