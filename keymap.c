#include QMK_KEYBOARD_H

enum layer_names {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST
};

#define LOWER  MO(_LOWER)
#define RAISE  MO(_RAISE)

#define CT_Q  LCTL_T(KC_Q)
#define CT_CM RCTL_T(KC_COMM)
#define SF_Z  LSFT_T(KC_Z)
#define SF_SS RSFT_T(KC_SLSH)
#define AL_X  LALT_T(KC_X)
#define AL_DT RALT_T(KC_DOT)
#define LO_SP LT(LOWER, KC_SPC)
#define RA_SP LT(RAICE, KC_SPC)
#define DBL_P ACTION_TAP_DANCE_DOUBLE(KC_P, KC_GRV)
#define DBL_Q ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC)
#define DBL_TAB ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ENT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
         CT_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    DBL_P,
         KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT, 
         SF_Z,    AL_X,    KC_C,    KC_V,    KC_B,          KC_N,    KC_M,    CT_CM,   AL_DT,   SF_SS, 
                          _______, _______,                                          _______, _______,
                                            DBL_TAB,  _______,              _______,  ADJUST,
                                            _______,  RA_SP,        LO_SP,  _______,
                                            _______, _______,       _______, _______
    ),

    [_LOWER] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
        KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,       KC_QUOT, KC_LBRC, KC_RBRC,  KC_BSLS, KC_SCLN,
        KC_LSFT, KC_ESC,  KC_LGUI, KC_LALT, KC_QUOT,       KC_HOME, KC_END,  KC_PGUP, KC_PGDN, KC_QUOT,
                          _______, _______,                                           _______, _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, _______
    ),

    [_RAISE] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,          KC_MINS, KC_EQL, KC_GRV,  KC_TILD, KC_COLN,
        KC_LSFT, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,       KC_LBRC, KC_RBRC, KC_BSLS, KC_DQUO, KC_DEL,
                          _______, _______,                                           _______,  _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, ______
    ),

    [_ADJUST] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PSCR,
        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_F11,  KC_F12,  KC_CAPS, XXXXXXX, XXXXXXX,       RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          _______, _______,                                           _______,  _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, _______,
                                            _______, _______,       _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
// Initialize variable holding the binary
// representation of active modifiers.
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Store the current modifier state in the variable for later reference
    mod_state = get_mods();
    switch (keycode) {

    case KC_BSPC:
        {
        // Initialize a boolean variable that keeps track
        // of the delete key status: registered or not?
        static bool delkey_registered;
        if (record->event.pressed) {
            // Detect the activation of either shift keys
            if (mod_state & MOD_MASK_SHIFT) {
                // First temporarily canceling both shifts so that
                // shift isn't applied to the KC_DEL keycode
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                // Update the boolean variable to reflect the status of KC_DEL
                delkey_registered = true;
                // Reapplying modifier state so that the held shift key(s)
                // still work even after having tapped the Backspace/Delete key.
                set_mods(mod_state);
                return false;
            }
        } else { // on release of KC_BSPC
            // In case KC_DEL is still being sent even after the release of KC_BSPC
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        // Let QMK process the KC_BSPC keycode as usual outside of shift
        return true;
    }

    }
    return true;
};

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_ALT, KC_BSPC, KC_ESC);
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SPC, KC_BSPC);
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_LCTL, KC_SPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    NULL // Null terminate the array of overrides!
};
