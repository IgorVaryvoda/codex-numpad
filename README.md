# Codex + Herdr Deck for the DMQ Design SPIN

This turns the connected 12-key, three-encoder SPIN macro pad into a global
control surface for Codex Desktop, Hyprland, and Herdr. It keeps the user's two
fixed media controls—bottom-knob volume and the bottom-row transport buttons—
while giving every other control a real job.

![Codex Deck layout](docs/layout.svg)

**[Open the complete quick reference](docs/quick-reference.md)** for every key,
knob gesture, mode color, and reset control on one page, or open
[`docs/cheatsheet.html`](docs/cheatsheet.html) in a browser for an interactive
version — click the knobs or press 1–4 to relight the board in each mode.

## The three modes

Press an encoder knob once to select its mode: top for **Codex**, middle for
**Media**, or bottom for **Herdr**. The change happens on the first press—no
double-click is required—and all three LEDs change together: blue for Codex,
orange for Media, and green for Herdr. Tapping the top knob a second time while
already in Codex mode toggles the cyan **Codex prompt bank**; tapping the
bottom knob a second time while in Herdr mode toggles the chartreuse **Herdr
prompt bank**. Turning the bottom knob always controls volume, regardless of
mode.

### Codex mode (blue)

|            |            |            |
|------------|------------|------------|
| Focus      | New task   | Search     |
| Back       | Quick Chat | Forward    |
| Prev task  | Find       | Next task  |
| Dictation  | Command    | Enter/Act  |

- Top knob: previous/next open Hyprland workspace
- Middle knob: previous/next window on the current workspace
- Bottom knob: volume down/up

Tap the top knob to enter Codex mode. Hold the **bottom knob** for at least
350 ms in any mode to record global dictation; release to transcribe locally,
type at the current cursor, and return to the mode you were using. A short
bottom-knob press selects Herdr instead. The dedicated Dictation key toggles
recording when tap-to-start/tap-to-stop is more convenient.

Every action first verifies that Codex actually has focus. If focus fails, the
helper refuses to type into another app.

### Codex prompt bank (cyan)

Tap the top knob again while in Codex mode to toggle a second bank of keys that
type ready-made prompts into Codex. Nothing is sent automatically: each key
types its prompt, leaves the cursor in place so you can edit it, and
**Enter/Act**—in the same bottom-right position as in Codex mode—submits it.
Rows follow the work lifecycle: start, fix, understand, ship.

|          |            |             |
|----------|------------|-------------|
| Plan     | Go on      | Finish      |
| Fix      | Test-fix   | Review-fix  |
| Status   | Explain    | Docs        |
| Commit   | Ship       | Enter/Act   |

The knobs keep their Codex assignments: workspaces, windows, and volume.

### Media mode (orange)

|             |      |              |
|-------------|------|--------------|
| Home        | Up   | End          |
| Left        | Down | Right        |
| Page Up     | Mute | Page Down    |
| Play/Pause  | Previous | Next     |

- Top knob: screen brightness down/up
- Middle knob: seek active media backward/forward 10 seconds
- Bottom knob: volume down/up

### Herdr mode (green)

|              |                |               |
|--------------|----------------|---------------|
| Focus Herdr  | Pane up        | New tab       |
| Pane left    | Pane down      | Pane right    |
| Prev agent   | New workspace  | Next agent    |
| Split right  | Zoom pane      | Split down    |

The pane cross sits on the same physical keys as the Media-mode arrows, so
directional muscle memory carries between the two modes. The Herdr helper uses
its local socket API rather than injecting prefix-key sequences into a
terminal. New workspaces and tabs inherit the focused pane's working directory.

### Herdr prompt bank (chartreuse)

Tap the bottom knob again while in Herdr mode to toggle a bank of the phrases
actually mined from this user's Codex and Claude history — `go on` alone was
typed 300+ times. Keys type into the focused Herdr pane without submitting;
**Send** presses Enter. Shared intents sit on the same physical keys as the
Codex prompt bank.

|               |            |              |
|---------------|------------|--------------|
| Plan          | Go on      | Do it        |
| Fix all       | Autoreview | Adversarial  |
| Status?       | Resume     | Improve-codex |
| Migrate       | Land it    | Send ⏎       |

Edit the prompt texts in `bin/kbd-herdr` (the `prompt-*` actions) — no
reflash needed.

- Top knob: previous/next Herdr workspace
- Middle knob: previous/next Herdr tab
- Bottom knob: volume down/up

Tap the bottom knob to enter Herdr mode; hold it to dictate instead. The LEDs
may briefly turn green when the hold begins because the firmware distinguishes
the tap from the dictation hold by timing it.

## Install and build

### Requirements

- Linux with Hyprland
- A DMQ Design SPIN and a local QMK checkout (by default `~/qmk_firmware`)
- `git`, `make`, an AVR QMK toolchain, `hyprctl`, `jq`, `ydotool`, and
  `playerctl`
- For offline dictation: `cmake`, a C/C++ compiler, `curl`, `parec`, and
  `ffmpeg`; NVIDIA CUDA is used automatically when `nvcc` is available
- Codex Desktop and Herdr for their respective modes

Override the QMK checkout or revision with `QMK_SOURCE` and `QMK_REF`.

The installer creates symlinks for the helper, Hyprland bindings, and QMK
keymap. It appends one `source` line to the existing user Hyprland config and
compiles the firmware in an isolated, current QMK worktree. Your existing
dirty QMK checkout is not modified. The installer does **not** flash unless
explicitly requested.

The symlinks embed the repo's absolute path. If you later move or rename the
repo, re-run `./install.sh` from the new location — otherwise the deck
degrades confusingly: Media mode keeps working (it emits plain keycodes) while
every Codex, Herdr, and dictation action silently does nothing. Check for
stragglers with `find ~/.local/bin ~/.config/hypr -xtype l`. Whisper builds
made before this note may also need `rm -rf .runtime/whisper.cpp/build &&
make dictation` after a move; current builds are relocatable.

```bash
./install.sh
make dictation
```

Run diagnostics at any time:

```bash
make doctor
```

Flash after putting the SPIN into its ATmega32U4 bootloader:

```bash
make flash
```

For the first install, when the flasher starts waiting, press the SPIN's tiny
side-mounted reset button directly underneath the USB-C connector. Once this
firmware is installed, press and hold all three knobs together for a full two
seconds. All three LEDs turn red as soon as the chord is recognized; keep
holding until the device disconnects and enters the bootloader. The flash
helper returns the board to the deck firmware after a successful write.

## Development

```bash
make test
make build
```

Set `KBD_CODEX_DRY_RUN=1` to print an action instead of focusing or typing:

```bash
KBD_CODEX_DRY_RUN=1 bin/kbd-codex new
```

The QMK firmware only emits uncommon `F13`–`F24` signals. Desktop behavior
lives in `hypr/codex-numpad.conf`, `bin/kbd-codex`, and `bin/kbd-herdr`, so most
actions can be changed without reflashing the board. Mode colors use QMK
RGBLight Layers with one full-strip segment per mode, which keeps all three LED
zones synchronized.

Dictation prefers a connected Razer Seiren Mini, falls back to the default
PulseAudio source, and runs a local `whisper.cpp` model. No recording is
uploaded. Override the microphone with `KBD_DICTATE_SOURCE`, or the default
`base.en` model with `KBD_DICTATE_MODEL`.
