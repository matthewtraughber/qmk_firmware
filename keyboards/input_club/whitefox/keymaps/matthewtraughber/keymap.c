#include QMK_KEYBOARD_H

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_TRIPLE_SINGLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// lshift & rshift tap dance (state determination)
td_state_t shift_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    if (state->count == 3) {
        if (!state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// lshift & rshift tap dance (state)
static td_tap_t shift_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// lshift tap dance (tap dance interrupted or ended)
void lshift_finished(qk_tap_dance_state_t *state, void *user_data) {
    shift_tap_state.state = shift_dance(state);
    switch (shift_tap_state.state) {
        case TD_SINGLE_TAP: register_code16(S(KC_9)); break;
        case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
        case TD_DOUBLE_TAP: register_code(KC_LBRC); break;
        case TD_DOUBLE_HOLD: register_code(KC_LSFT); break;     // TODO:
        case TD_TRIPLE_TAP: register_code16(S(KC_LBRC)); break;
        case TD_TRIPLE_HOLD: register_code(KC_LSFT); break;     // TODO:
        default: break;
    }
}

void lshift_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (shift_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code16(S(KC_9)); break;
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case TD_DOUBLE_TAP: unregister_code(KC_LBRC); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LSFT); break;       // TODO:
        case TD_TRIPLE_TAP: unregister_code16(S(KC_LBRC)); break;
        case TD_TRIPLE_HOLD: unregister_code(KC_LSFT); break;       // TODO:
        default: break;
    }
    shift_tap_state.state = TD_NONE;
}

// rshift tap dance (tap dance interrupted or ended)
void rshift_finished(qk_tap_dance_state_t *state, void *user_data) {
    shift_tap_state.state = shift_dance(state);
    switch (shift_tap_state.state) {
        case TD_SINGLE_TAP: register_code16(S(KC_0)); break;
        case TD_SINGLE_HOLD: register_code(KC_RSFT); break;
        case TD_DOUBLE_TAP: register_code(KC_RBRC); break;
        case TD_DOUBLE_HOLD: register_code(KC_RSFT); break;     // TODO:
        case TD_TRIPLE_TAP: register_code16(S(KC_RBRC)); break;
        case TD_TRIPLE_HOLD: register_code(KC_RSFT); break;     // TODO:
        default: break;
    }
}

void rshift_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (shift_tap_state.state) {
        case TD_SINGLE_TAP: unregister_code16(S(KC_0)); break;
        case TD_SINGLE_HOLD: unregister_code(KC_RSFT); break;
        case TD_DOUBLE_TAP: unregister_code(KC_RBRC); break;
        case TD_DOUBLE_HOLD: unregister_code(KC_RSFT); break;   // TODO:
        case TD_TRIPLE_TAP: unregister_code16(S(KC_RBRC)); break;
        case TD_TRIPLE_HOLD: unregister_code(KC_RSFT); break;   // TODO:
        default: break;
    }
    shift_tap_state.state = TD_NONE;
}

// tap dance enums
enum {
    UP_PGUP,
    DOWN_PGDN,
    LEFT_HOME,
    RGHT_END,
    LSFT_SPACECMDR,
    RSFT_SPACECMDR
};

// tap dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [UP_PGUP] = ACTION_TAP_DANCE_DOUBLE(KC_UP, KC_PGUP),        // TODO:
    [DOWN_PGDN] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, KC_PGDN),    // TODO:
    [LEFT_HOME] = ACTION_TAP_DANCE_DOUBLE(KC_LEFT, KC_HOME),    // TODO:
    [RGHT_END] = ACTION_TAP_DANCE_DOUBLE(KC_RGHT, KC_END),      // TODO:
    [LSFT_SPACECMDR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshift_finished, lshift_reset),
    [RSFT_SPACECMDR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rshift_finished, rshift_reset),
};

// tapping terms for dual-role keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(LSFT_SPACECMDR): case TD(RSFT_SPACECMDR):
            return TAPPING_TERM + 200;
        default:
            return TAPPING_TERM;
    }
}

// "permissive" mode for dual-role keys
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(1, KC_ESC):
            return true;
        default:
            return false;
    }
}

// “hold on other key press” mode for dual-role keys
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(1, KC_ESC):
            return true;
        default:
            return false;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   /*
    * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    * │GEs│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │Del│Bck│LdT│
    * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┼───┤
    * │Tab  │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │Ld+│
    * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
    * │L1/Esc│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │Enter   │Ld-│
    * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
    * │ShftSC  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ShftSC│ ↑ │   │
    * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬─┬───┼───┼───┤
    * │LCtl│LGUI│LAlt│         Space          │ L1 │RAlt│ │ ← │ ↓ │ → │
    * └────┴────┴────┴────────────────────────┴────┴────┘ └───┴───┴───┘
    */
  [0] = LAYOUT_truefox( \
    QK_GESC,            KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,            KC_EQL,  KC_DEL,    KC_BSPC,    BL_TOGG, \
    KC_TAB,             KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,            KC_RBRC, KC_BSLS,   BL_INC,  \
    LT(1, KC_ESC),      KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,            KC_ENT,  BL_DEC,  \
    TD(LSFT_SPACECMDR), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, TD(RSFT_SPACECMDR), KC_UP,   KC_CAPS, \
    KC_LCTL,            KC_LGUI, KC_LALT,                   KC_SPC,                    MO(1),   KC_RALT, KC_LEFT, KC_DOWN,            KC_RGHT  \
  ),

   /*
    * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    * │Grv│F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│F13│F14│F15│
    * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┼───┤
    * │     │Prv│Ply│Nxt│Pse│   │   │   │↑PU│   │PrS│   │   │     │   │
    * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
    * │      │   │VoD│VoU│Mut│   │   │←Ho│↓PD│→En│   │   │        │   │
    * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
    * │        │   │   │   │   │   │   │   │   │   │   │      │   │Blr│
    * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬─┬───┼───┼───┤
    * │    │    │    │       Backspace        │    │    │ │   │   │   │
    * └────┴────┴────┴────────────────────────┴────┴────┘ └───┴───┴───┘
    */
  [1] = LAYOUT_truefox( \
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,  KC_F14, KC_F15, \
    _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_PAUSE, _______, _______, KC_PGUP, KC_UP,   KC_PGDN, KC_PSCR, _______, _______, _______, AG_TOGG,  \
    _______, _______, KC_VOLD, KC_VOLU, KC_MUTE,  _______, KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END,  _______, _______, _______,  \
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,  \
    _______, _______, _______, KC_BSPC, _______,  _______, _______, _______, _______  \
  ),

   /*
    * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
    * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┼───┤
    * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │   │
    * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
    * │      │   │   │   │   │   │   │   │   │   │   │   │        │   │
    * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
    * │        │   │   │   │   │   │   │   │   │   │   │      │   │   │
    * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬─┬───┼───┼───┤
    * │    │    │    │                        │    │    │ │   │   │   │
    * └────┴────┴────┴────────────────────────┴────┴────┘ └───┴───┴───┘
    */
  [2] = LAYOUT_truefox( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______  \
  ),

   /*
    * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
    * │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │   │
    * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┼───┤
    * │     │   │   │   │   │   │   │   │   │   │   │   │   │     │   │
    * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
    * │      │   │   │   │   │   │   │   │   │   │   │   │        │   │
    * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
    * │        │   │   │   │   │   │   │   │   │   │   │      │   │   │
    * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬─┬───┼───┼───┤
    * │    │    │    │                        │    │    │ │   │   │   │
    * └────┴────┴────┴────────────────────────┴────┴────┘ └───┴───┴───┘
    */
  [3] = LAYOUT_truefox( \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  \
    _______, _______, _______, _______, _______, _______, _______, _______, _______  \
  ),
};
