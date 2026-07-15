/*
 * Codex Deck keymap for the DMQ Design SPIN.
 *
 * The firmware emits otherwise-unused F13-F24 chords in Codex and Control
 * modes. Hyprland turns those signals into app-aware actions. This keeps the
 * board useful in every app and avoids baking desktop-specific commands into
 * QMK.
 */
#include QMK_KEYBOARD_H

enum layers {
    _CODEX,
    _MEDIA,
    _HERDR,
};

enum custom_keycodes {
    MODE_CODEX = SAFE_RANGE,
    MODE_MEDIA,
    MODE_HERDR,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_CODEX] = LAYOUT(
        KC_F13,       KC_F14,       KC_F15,       MODE_CODEX,
        KC_F16,       KC_F17,       KC_F18,       MODE_MEDIA,
        KC_F19,       KC_F20,       KC_F21,       MODE_HERDR,
        KC_F22,       KC_F23,       KC_F24
    ),

    /* Preserve volume and the bottom media row; streamline everything else. */
    [_MEDIA] = LAYOUT(
        KC_HOME,      KC_UP,        KC_END,       MODE_CODEX,
        KC_LEFT,      KC_DOWN,      KC_RIGHT,     MODE_MEDIA,
        KC_PGUP,      KC_MUTE,      KC_PGDN,      MODE_HERDR,
        KC_MPLY,      KC_MPRV,      KC_MNXT
    ),

    [_HERDR] = LAYOUT(
        A(KC_F13),    A(KC_F14),    A(KC_F15),    MODE_CODEX,
        A(KC_F16),    A(KC_F17),    A(KC_F18),    MODE_MEDIA,
        A(KC_F19),    A(KC_F20),    A(KC_F21),    MODE_HERDR,
        A(KC_F22),    A(KC_F23),    A(KC_F24)
    ),
};

static void tap(uint16_t keycode) {
    tap_code16(keycode);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    const uint8_t layer = get_highest_layer(layer_state);

    /* Physical bottom knob (encoder 2): volume is invariant in every mode. */
    if (index == 2) {
        tap(clockwise ? KC_VOLU : KC_VOLD);
        return false;
    }

    if (layer == _CODEX) {
        if (index == 0) { /* physical top */
            tap(clockwise ? C(KC_F14) : C(KC_F13)); /* Hypr workspace */
        } else if (index == 1) { /* physical middle */
            tap(clockwise ? C(KC_F16) : C(KC_F15)); /* Hypr window */
        }
    } else if (layer == _MEDIA) {
        if (index == 0) { /* physical top */
            tap(clockwise ? KC_BRIU : KC_BRID);
        } else if (index == 1) { /* physical middle */
            tap(clockwise ? C(KC_F18) : C(KC_F17)); /* media seek */
        }
    } else if (layer == _HERDR) {
        if (index == 0) { /* physical top */
            tap(clockwise ? C(A(KC_F14)) : C(A(KC_F13))); /* workspace */
        } else if (index == 1) { /* physical middle */
            tap(clockwise ? C(A(KC_F16)) : C(A(KC_F15))); /* tab */
        }
    }

    return false;
}

static void paint_mode(uint8_t red, uint8_t green, uint8_t blue, uint8_t active) {
    /* Bathe the whole deck in the mode color, with its selector brightest. */
    for (uint8_t index = 0; index < 3; index++) {
        rgblight_setrgb_at(red / 10, green / 10, blue / 10, index);
    }
    rgblight_setrgb_at(red, green, blue, active);
}

static void paint_layer(uint8_t layer) {
    if (layer == _CODEX) {
        paint_mode(0, 110, 255, 0);     /* blue */
    } else if (layer == _MEDIA) {
        paint_mode(255, 90, 0, 1);      /* amber */
    } else {
        paint_mode(0, 210, 90, 2);      /* Herdr green */
    }
}

static bool     mode_knob_pressed[3] = {false, false, false};
static bool     reset_combo_latched  = false;
static uint16_t reset_combo_timer    = 0;
static bool     dictation_knob_held = false;
static bool     dictation_held   = false;
static uint16_t dictation_timer  = 0;

static int8_t mode_knob_index(uint16_t keycode) {
    switch (keycode) {
        case MODE_CODEX: return 0;
        case MODE_MEDIA: return 1;
        case MODE_HERDR: return 2;
        default: return -1;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const int8_t knob = mode_knob_index(keycode);
    if (knob < 0) {
        return true;
    }

    if (record->event.pressed) {
        mode_knob_pressed[knob] = true;
        if (knob == 2) {
            dictation_knob_held = true;
            dictation_held      = false;
            dictation_timer     = timer_read();
        }
    } else {
        mode_knob_pressed[knob] = false;

        /* Suppress mode changes and dictation until a reset chord is released. */
        if (reset_combo_latched) {
            if (!mode_knob_pressed[0] && !mode_knob_pressed[1] && !mode_knob_pressed[2]) {
                reset_combo_latched = false;
                paint_layer(get_highest_layer(layer_state));
            }
            return false;
        }

        if (knob == 0) {
            layer_move(_CODEX);
            return false;
        }
        if (knob == 1) {
            layer_move(_MEDIA);
            return false;
        }

        dictation_knob_held = false;
        if (dictation_held) {
            unregister_code16(S(KC_F24));
            dictation_held = false;
        } else {
            layer_move(_HERDR);
        }
    }
    return false;
}

void matrix_scan_user(void) {
    /* Track the actual knob key events; raw matrix polling was unreliable. */
    const bool reset_combo = mode_knob_pressed[0] && mode_knob_pressed[1] && mode_knob_pressed[2];

    if (reset_combo && !reset_combo_latched) {
        reset_combo_latched = true;
        reset_combo_timer   = timer_read();
        dictation_knob_held = false;
        if (dictation_held) {
            unregister_code16(S(KC_F24));
            dictation_held = false;
        }
        rgblight_setrgb(255, 0, 0);
    } else if (reset_combo && timer_elapsed(reset_combo_timer) >= 2000) {
        bootloader_jump();
    }

    /* Hold the bottom knob to dictate globally; tap it to select Herdr mode. */
    if (!reset_combo_latched && dictation_knob_held && !dictation_held && timer_elapsed(dictation_timer) >= 350) {
        dictation_held = true;
        register_code16(S(KC_F24));
    }
}

void keyboard_post_init_user(void) {
    paint_layer(_CODEX);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    paint_layer(get_highest_layer(state));
    return state;
}
