/* Copyright 2021 Stefano Pace
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */ 

#include QMK_KEYBOARD_H

#define COMPOSE_KEY KC_APP

#define LTFN_ESC LT(_FN, KC_ESC)
#define LTSYM_F LT(_SYM, KC_F)
#define LTSYM_J LT(_SYM, KC_J)
#define TD_SHCAP TD(TD_SHIFT_CAPS_LOCK)
#define SFT_SLSH RSFT_T(KC_SLSH)

#define UNDO LCTL(KC_Z)
#define REDO LCTL(KC_Y)
#define CUT LCTL(KC_X)
#define COPY LCTL(KC_C)
#define PASTE LCTL(KC_V)

#define TERM LCA(KC_EQL)

enum layers {
    _QWE,
    _SYM,
    _NUM,
    _NAV,
    _NV2,
    _FN,
    _LED,
};

enum custom_keycodes {
    M_A_GRV = SAFE_RANGE,
    M_I_GRV,
    M_O_GRV,
    M_U_GRV,
};

enum tapdance {
    TD_E_GRV_ACU,
    TD_O_GRV_SCHWA,
    TD_DOLLAR_EUR,
};

static uint8_t shifts_state;
static uint8_t weak_mods;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
       /* Base qwerty Layer
        * 
        * Holding Esc activates the Fn layer
        * Holding F or J activates Symbols layer
        * Holding / acts as Shift
        * Term sends Ctrl + Alt + '=' which in my os give focus to the terminal
        * 
        * ,----------------------------------------------------------------------------.
        * |Esc-Fn|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | Bspace  |
        * |----------------------------------------------------------------------------+
        * | Tab   |  A  |  S  |  D  |F-Sym|  G  |  H  |J-Sym|  K  |  L  |  '   | Enter |
        * |----------------------------------------------------------------------------+
        * | Shift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |/-Shift|Term |
        * |----------------------------------------------------------------------------+
        * | Ctrl | Gui | Alt |(Led)|(Nav) |   Space   |(Num) |Left |Down | Up   |Right |
        * `----------------------------------------------------------------------------'
        */
    [_QWE] = LAYOUT(
        LTFN_ESC,  KC_Q,     KC_W,     KC_E,      KC_R,      KC_T,    KC_Y,  KC_U,      KC_I,     KC_O,     KC_P,      KC_BSPC,
        KC_TAB,    KC_A,     KC_S,     KC_D,      LTSYM_F,   KC_G,    KC_H,  LTSYM_J,   KC_K,     KC_L,     KC_QUOT,   KC_ENT,
        KC_LSFT,   KC_Z,     KC_X,     KC_C,      KC_V,      KC_B,    KC_N,  KC_M,      KC_COMM,  KC_DOT,   SFT_SLSH,  TERM,
        KC_LCTL,   KC_LGUI,  KC_LALT,  MO(_LED),  MO(_NAV),       KC_SPC,    MO(_NUM),  KC_LEFT,  KC_DOWN,  KC_UP,     KC_RGHT
    ),


       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  |  {  |  %  |  ^  |  }  |  *  |  ;  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  &  |  -  |  (  |  +  |  =  |  )  |  |  |  :  |   "  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  | $/€ |  _  |  [  |     |  \  |  ]  |  <  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_SYM] = LAYOUT(
        _______,  KC_GRV,   KC_AT,              KC_HASH,  KC_LCBR, KC_PERC,  KC_CIRC,  KC_RCBR,  KC_ASTR,  KC_SCLN,  XXXXXXX,  KC_DEL,
        _______,  KC_TILD,  KC_AMPR,            KC_MINS,  KC_LPRN, KC_PLUS,  KC_EQL,   KC_RPRN,  KC_PIPE,  KC_COLN,  KC_DQUO,  _______,
        _______,  KC_EXLM,  TD(TD_DOLLAR_EUR),  KC_UNDS,  KC_LBRC, XXXXXXX,  KC_BSLS,  KC_RBRC,  KC_LT,      KC_GT,  KC_QUES,  _______,
        _______,  _______,  _______,            _______,  XXXXXXX,      _______,       XXXXXXX,  _______,  _______,  _______,  _______
    ),

       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  | $/€ |  %  |  ^  |  &  |  *  |  ;  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  {  |  -  |  [  |  +  |  =  |  (  |  |  |  :  |   "  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  |  }  |  _  |  ]  |     |  \  |  )  |  <  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    // [_SYM] = LAYOUT(
    //     _______,  KC_GRV,   KC_AT,    KC_HASH,  TD(TD_DOLLAR_EUR),   KC_PERC,  KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_SCLN,  XXXXXXX,  KC_DEL,
    //     _______,  KC_TILD,  KC_LCBR,  KC_MINS,  KC_LBRC,             KC_PLUS,  KC_EQL,   KC_LPRN,  KC_PIPE,  KC_COLN,  KC_DQUO,  _______,
    //     _______,  KC_EXLM,  KC_RCBR,  KC_UNDS,  KC_RBRC,             XXXXXXX,  KC_BSLS,  KC_RPRN,  KC_LT,      KC_GT,  KC_QUES,  _______,
    //     _______,  _______,  _______,  _______,  XXXXXXX,                  _______,       XXXXXXX,  _______,  _______,  _______,  _______
    // ),
      
       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  | $/€ |  %  |     |  &  |  *  |  ^  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  {  |  -  |  [  |  +  |  =  |  (  |  |  |  ;  |   "  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  |  }  |  _  |  ]  |  \  |  :  |  )  |  <  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    // [_SYM] = LAYOUT(
    //     _______,  KC_GRV,   KC_AT,    KC_HASH,  TD(TD_DOLLAR_EUR),   KC_PERC,  XXXXXXX,  KC_AMPR,  KC_ASTR,  KC_CIRC,  XXXXXXX,  KC_DEL,
    //     _______,  KC_TILD,  KC_LCBR,  KC_MINS,  KC_LBRC,             KC_PLUS,  KC_EQL,   KC_LPRN,  KC_PIPE,  KC_SCLN,  KC_DQUO,  _______,
    //     _______,  KC_EXLM,  KC_RCBR,  KC_UNDS,  KC_RBRC,             KC_BSLS,  KC_COLN,  KC_RPRN,  KC_LT,      KC_GT,  KC_QUES,  _______,
    //     _______,  _______,  _______,  _______,  XXXXXXX,                  _______,       XXXXXXX,  _______,  _______,  _______,  _______
    // ),

       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  | $/€ |  %  |     |  &  |  *  |  ^  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  \  |  -  |  _  |  +  |  =  |  :  |  |  |  ;  |   "  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  |  {  |  }  |  [  |  ]  |  (  |  )  |  <  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    // [_SYM] = LAYOUT(
    //     _______,  KC_GRV,   KC_AT,    KC_HASH,  TD(TD_DOLLAR_EUR),   KC_PERC,  XXXXXXX,  KC_AMPR,  KC_ASTR,  KC_CIRC,  XXXXXXX,  KC_DEL,
    //     _______,  KC_TILD,  KC_BSLS,  KC_MINS,  KC_UNDS,             KC_PLUS,  KC_EQL,   KC_COLN,  KC_PIPE,  KC_SCLN,  KC_DQUO,  _______,
    //     _______,  KC_EXLM,  KC_LCBR,  KC_RCBR,  KC_LBRC,             KC_RBRC,  KC_LPRN,  KC_RPRN,  KC_LT,      KC_GT,  KC_QUES,  _______,
    //     _______,  _______,  _______,  _______,  XXXXXXX,                  _______,       XXXXXXX,  _______,  _______,  _______,  _______
    // ),

       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  | $/€ |  %  |     |  &  |  *  |  ^  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  {  |  -  |  (  |  +  |  =  |  )  |  |  |  }  |   ;  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  |  <  |  _  |  [  |     |  :  |  ]  |  \  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    // [_SYM] = LAYOUT(
    //     _______,  KC_GRV,   KC_AT,    KC_HASH,  TD(TD_DOLLAR_EUR),   KC_PERC,  XXXXXXX,  KC_AMPR,  KC_ASTR,  KC_CIRC,  XXXXXXX,  KC_DEL,
    //     _______,  KC_TILD,  KC_LCBR,  KC_MINS,  KC_LPRN,             KC_PLUS,  KC_EQL,   KC_RPRN,  KC_PIPE,  KC_RCBR,  KC_SCLN,  _______,
    //     _______,  KC_EXLM,  KC_LT,    KC_UNDS,  KC_LBRC,             XXXXXXX,  KC_COLN,  KC_RBRC,  KC_BSLS,  KC_GT,    KC_QUES,  _______,
    //     _______,  _______,  _______,  _______,  XXXXXXX,                  _______,       XXXXXXX,  _______,  _______,  _______,  _______
    // ),

       /* Numbers Layer (Num)
        *
        * ,----------------------------------------------------------------------------.
        * |      |     |     | è/é |     |    |Ctrl-y|  ù  |  ì  |  ò  |     |         |
        * |----------------------------------------------------------------------------+
        * |   à   |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0   |       |
        * |----------------------------------------------------------------------------+
        * |        |Ctr-z|Ctr-x|Ctr-c|Ctr-v|     |     |     |  ,  |  .  |      |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |    |       |           |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_NUM] = LAYOUT(
        _______,  XXXXXXX,  XXXXXXX, TD(TD_E_GRV_ACU),  XXXXXXX,   XXXXXXX,  REDO,     M_U_GRV,  M_I_GRV,  TD(TD_O_GRV_SCHWA),  XXXXXXX,  _______,
        M_A_GRV,  KC_1,     KC_2,    KC_3,              KC_4,      KC_5,     KC_6,     KC_7,     KC_8,     KC_9,                KC_0,     _______,
        _______,  UNDO,     CUT,     COPY,              PASTE,     XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_COMM,  KC_DOT,              _______,  _______,
        _______,  _______,  _______, _______,           XXXXXXX,        _______,       _______,  _______,  _______,             _______,  _______
    ),

       /* Navigation Layer (Nav)
        * 
        * This layer is optimized for gui handling and code navigation
        * It makes easier to reach common key combinations as: Ctrl+Shift, Gui+Shift, Ctrl+Alt, Alt+Tab, Ctrl+F
        * 
        * Holding (Nav2) replaces arrow keys with Home PgDown PgUp End
        * 
        * ,----------------------------------------------------------------------------.
        * |      |Shift|Shift|Shift|     |     |     |     |     |     |     |         |
        * |----------------------------------------------------------------------------+
        * |(Nav2) |Ctrl | Gui | Alt | Tab |     |Left |Down | Up  |Right|   F  |       |
        * |----------------------------------------------------------------------------+
        * |CapsLock|     |     |     |     |     |     |     |     |     |      |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |       |    |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_NAV] = LAYOUT(
        _______,   KC_LSFT,  KC_LSFT,  KC_LSFT,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        MO(_NV2),  KC_LCTL,  KC_LGUI,  KC_LALT,  KC_TAB,   _______,  KC_LEFT,  KC_DOWN,   KC_UP,    KC_RGHT,  KC_F,     _______,
        KC_CAPS,   _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        _______,   _______,  _______,  _______,  _______,       _______,       XXXXXXX,   _______,  _______,  _______,  _______
    ),

       /* Speed Navigation Layer (Nav2)
        * ,----------------------------------------------------------------------------.
        * |      |     |     |     |     |     |     |     |     |     |     |         |
        * |----------------------------------------------------------------------------+
        * |       |     |     |     |     |     |Home |PgUp |PgDown|End |      |       |
        * |----------------------------------------------------------------------------+
        * |        |     |     |     |     |     |     |     |     |     |      |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |Reset |
        * `----------------------------------------------------------------------------'
        */
    [_NV2] = LAYOUT(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,       _______,       _______,  _______,  _______,  _______,  RESET
    ),

       /* Fn Layer (Fn)
        * ,----------------------------------------------------------------------------.
        * |      |     |Print|SLock| Ins |     |     | F1  | F2  | F3  | F10 |         |
        * |----------------------------------------------------------------------------+
        * |       |     |     |     |     |     |     | F4  | F5  | F6  | F11  |       |
        * |----------------------------------------------------------------------------+
        * |        |     |     |     |     |     |     | F7  | F8  | F9  | F12  |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_FN] = LAYOUT(
        _______,  XXXXXXX, KC_PSCR,  KC_SLCK, KC_INS,  XXXXXXX, XXXXXXX,  KC_F1,    KC_F2,    KC_F3,    KC_F10,  XXXXXXX,
        XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_F4,    KC_F5,    KC_F6,    KC_F11,  XXXXXXX,
        XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_F7,    KC_F8,    KC_F9,    KC_F12,  XXXXXXX,
        XXXXXXX,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,      XXXXXXX,      XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX
    ),

        /* Led layer
        * ,----------------------------------------------------------------------------.
        * |      |     |     |     |     |     |     |     |     |     |     |         |
        * |----------------------------------------------------------------------------+
        * |       |     |     |     |     |     |     |vol+ |vol- |mute |      |       |
        * |----------------------------------------------------------------------------+
        * |        |     |     |     |     |     |     |prev |next |play |stop  |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_LED] = LAYOUT(
        XXXXXXX, RGB_TOG, RGB_RMOD, RGB_MOD, RGB_HUD,  RGB_HUI,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, RGB_SAD, RGB_SAI,  RGB_VAD, RGB_VAI,  RGB_SPD,  RGB_SPI, KC_VOLD, KC_VOLU, KC_MUTE, XXXXXXX, XXXXXXX,
        XXXXXXX, RGB_M_P, RGB_M_B,  RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX,  _______, XXXXXXX,       XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    ),
};

void type_accented_letter(uint16_t keycode, uint16_t accent_keycode) {
    shifts_state = get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT));
    weak_mods = get_weak_mods();

    clear_weak_mods();
    unregister_mods(shifts_state);

    tap_code(COMPOSE_KEY);
    tap_code(accent_keycode);

    add_weak_mods(weak_mods);
    register_mods(shifts_state);
    
    tap_code(keycode);
}

void type_accented_e (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        type_accented_letter(KC_E, KC_GRV);
    } else {
        type_accented_letter(KC_E, KC_QUOT);
    }
}

void type_schwa(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        type_accented_letter(KC_O, KC_GRV);
    } else {
        type_accented_letter(KC_E, KC_E);
    }
}

void type_dollar_or_euro (qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        tap_code16(KC_DLR);
    } else {
        type_accented_letter(KC_E, KC_EQL);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {         
        switch (keycode) {
            case M_A_GRV:
                type_accented_letter(KC_A, KC_GRV);
                return false;
            case M_I_GRV:
                type_accented_letter(KC_I, KC_GRV);
                return false;
            case M_U_GRV:
                type_accented_letter(KC_U, KC_GRV);
                return false;
        }
    }
    return true;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_E_GRV_ACU] = ACTION_TAP_DANCE_FN (type_accented_e),
    [TD_O_GRV_SCHWA] = ACTION_TAP_DANCE_FN (type_schwa),
    [TD_DOLLAR_EUR] = ACTION_TAP_DANCE_FN (type_dollar_or_euro)
};