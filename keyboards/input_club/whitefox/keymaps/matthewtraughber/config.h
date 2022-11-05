// https://docs.qmk.fm/#/getting_started_introduction?id=the-configh-file
#pragma once

// https://docs.qmk.fm/#/tap_hold?id=tapping-term
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

// https://docs.qmk.fm/#/tap_hold?id=permissive-hold
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY

// https://docs.qmk.fm/#/tap_hold?id=hold-on-other-key-press
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

/*
 * https://docs.qmk.fm/#/tap_hold?id=ignore-mod-tap-interrupt
 *
 * Enabling IGNORE_MOD_TAP_INTERRUPT is recommended when using Mod-Tap on alphanumeric keys
 * to avoid hold function taking precendence when the next key is pressed quickly.
 */
#define IGNORE_MOD_TAP_INTERRUPT
