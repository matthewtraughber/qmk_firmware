// https://docs.qmk.fm/#/getting_started_introduction?id=the-configh-file
#pragma once

/*
 * https://docs.qmk.fm/#/tap_hold?id=ignore-mod-tap-interrupt
 *
 * Enabling IGNORE_MOD_TAP_INTERRUPT is recommended when using Mod-Tap on alphanumeric keys
 * to avoid hold function taking precendence when the next key is pressed quickly.
 */
#define IGNORE_MOD_TAP_INTERRUPT

// https://docs.qmk.fm/#/tap_hold?id=tapping-term
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
