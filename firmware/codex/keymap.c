/*
 * Codex Deck keymap for the DMQ Design SPIN.
 *
 * The firmware emits otherwise-unused F13-F24 chords: plain in Codex mode,
 * Shift in the Codex prompt bank, and Alt in Herdr mode. Hyprland turns those
 * signals into app-aware actions. This keeps the board useful in every app
 * and avoids baking desktop-specific commands into QMK.
 */
#include QMK_KEYBOARD_H

enum layers {
    _CODEX,
    _MEDIA,
    _HERDR,
    _PROMPTS,
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

    /* Codex prompt bank: tap the top knob again while in Codex mode.
     * Shift+F24 is reserved for dictation, so the bottom-right key stays
     * plain F24 (Enter/Act) to submit a typed prompt. */
    [_PROMPTS] = LAYOUT(
        S(KC_F13),    S(KC_F14),    S(KC_F15),    MODE_CODEX,
        S(KC_F16),    S(KC_F17),    S(KC_F18),    MODE_MEDIA,
        S(KC_F19),    S(KC_F20),    S(KC_F21),    MODE_HERDR,
        S(KC_F22),    S(KC_F23),    KC_F24
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

    if (layer == _CODEX || layer == _PROMPTS) {
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

const rgblight_segment_t PROGMEM codex_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_BLUE}
);
const rgblight_segment_t PROGMEM media_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_ORANGE}
);
const rgblight_segment_t PROGMEM herdr_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_GREEN}
);
const rgblight_segment_t PROGMEM prompts_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_CYAN}
);
const rgblight_segment_t PROGMEM reset_rgb_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_RED}
);
const rgblight_segment_t *const PROGMEM mode_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    codex_rgb_layer,
    media_rgb_layer,
    herdr_rgb_layer,
    prompts_rgb_layer,
    reset_rgb_layer
);

#define RESET_RGB_LAYER 4

static void paint_layer(uint8_t layer) {
    rgblight_set_layer_state(RESET_RGB_LAYER, false);
    rgblight_set_layer_state(0, layer == _CODEX);
    rgblight_set_layer_state(1, layer == _MEDIA);
    rgblight_set_layer_state(2, layer == _HERDR);
    rgblight_set_layer_state(3, layer == _PROMPTS);
}

static bool     mode_knob_pressed[3] = {false, false, false};
static bool     reset_combo_latched  = false;
static uint16_t reset_combo_timer    = 0;
static bool     dictation_knob_held = false;
static bool     dictation_held   = false;
static uint16_t dictation_timer  = 0;
static uint8_t  dictation_return_layer = _CODEX;

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
        if (knob == 0) {
            /* A second top-knob press while in Codex toggles the prompt bank. */
            layer_move(get_highest_layer(layer_state) == _CODEX ? _PROMPTS : _CODEX);
        } else if (knob == 1) {
            layer_move(_MEDIA);
        } else {
            dictation_return_layer = get_highest_layer(layer_state);
            layer_move(_HERDR);
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

        if (knob != 2) {
            return false;
        }

        dictation_knob_held = false;
        if (dictation_held) {
            unregister_code16(S(KC_F24));
            dictation_held = false;
            layer_move(dictation_return_layer);
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
        rgblight_set_layer_state(RESET_RGB_LAYER, true);
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
    rgblight_layers = mode_rgb_layers;
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    paint_layer(_CODEX);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    paint_layer(get_highest_layer(state));
    return state;
}
