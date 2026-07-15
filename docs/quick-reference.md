# Codex Deck — Quick Reference

DMQ Design SPIN · 12 keys · 3 clickable knobs

All three LEDs show the active mode together: **blue** for Codex, **amber** for
Media, and **green** for Herdr. Red means the bootloader chord is being held.

## Global controls

| Control | Action |
|---|---|
| Turn bottom knob | Volume down/up in every mode |
| Tap top knob | Switch to **Codex** mode |
| Tap middle knob | Switch to **Media** mode |
| Tap bottom knob | Switch to **Herdr** mode |
| Hold bottom knob | Dictate globally; release to transcribe and type |
| Hold all three knobs for 2 seconds | Enter the QMK bootloader |

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

## Media mode — amber LEDs

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

| | | |
|---|---|---|
| **Focus Herdr** | **New workspace** | **New tab** |
| **Pane left** | **Pane up** | **Pane right** |
| **Previous agent** | **Pane down** | **Next agent** |
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
firmware, hold all three knobs for two seconds to enter the bootloader.
