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
#define TD_QUOT TD(TD_SINGLE_QUOTE_DOUBLE_QUOTES)
#define SFT_SLSH RSFT_T(KC_SLSH)

#define UNDO LCTL(KC_Z)
#define REDO LCTL(KC_Y)
#define CUT LCTL(KC_X)
#define COPY LCTL(KC_C)
#define PASTE LCTL(KC_V)

#define CUSTOM1 LCA(KC_EQL)
#define CUSTOM2 MEH(KC_PLUS)

#define L_SHIFT_HELD (get_mods() & (MOD_BIT(KC_LSFT)))
#define R_SHIFT_HELD (get_mods() & (MOD_BIT(KC_RSFT)))

enum layers {
    _QWE,
    _SYM,
    _NUM,
    _NAV,
    _NV2,
    _FN,
    _ACC,
};

enum custom_keycodes {
    M_A_GRV = SAFE_RANGE,
    M_I_GRV,
    M_O_GRV,
    M_U_GRV,
};

enum tapdance {
    TD_SINGLE_QUOTE_DOUBLE_QUOTES,
    TD_E_GRV_ACU
};

static uint8_t ls;
static uint8_t rs;

void dance_e_finished (qk_tap_dance_state_t *state, void *user_data) {
    ls = (get_mods() & (MOD_BIT(KC_LSHIFT)));
    rs = (get_mods() & (MOD_BIT(KC_RSHIFT)));

    if (ls) {
        unregister_mods(MOD_BIT(KC_LSHIFT));
    }
    if (rs) {
        unregister_mods(MOD_BIT(KC_RSHIFT));
    }

    register_code(COMPOSE_KEY);
    unregister_code(COMPOSE_KEY);

    if (state->count == 1) {
        register_code(KC_GRV);
        unregister_code(KC_GRV);
    } else {
        register_code(KC_QUOT);
        unregister_code(KC_QUOT);
    }

    if (ls) {
        register_mods(MOD_BIT(KC_LSHIFT));
    }
    if (rs) {
        register_mods(MOD_BIT(KC_RSHIFT));
    }

    register_code(KC_E);
    unregister_code(KC_E);
}


qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_SINGLE_QUOTE_DOUBLE_QUOTES] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
    [TD_E_GRV_ACU] = ACTION_TAP_DANCE_FN (dance_e_finished)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
       /* Base qwerty Layer
        * 
        * Holding Esc activates the Fn layer
        * Holding F or J activates Symbols layer
        * Holding / acts as Shift
        * Double tap on ' acts as "
        * Double tap on Menu activates CapsLock
        * 
        * Nav and Num layers are togglable with triple tapping on relative keys while holding them will activate layers momentarily.
        * 
        * I personally use Pause to mute/unmute microphone
        * I personally use Menu to act as compose key to input accented letters
        * 
        * ,----------------------------------------------------------------------------.
        * |Esc-Fn|  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | Bspace  |
        * |----------------------------------------------------------------------------+
        * | Tab   |  A  |  S  |  D  |F-Sym|  G  |  H  |J-Sym|  K  |  L  |  '*  | Enter |
        * |----------------------------------------------------------------------------+
        * | Shift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |/-Shift|Menu*|
        * |----------------------------------------------------------------------------+
        * | Ctrl | Gui | Alt |Pause|(Nav) |   Space   |(Num) |Left |Down | Up   |Right |
        * `----------------------------------------------------------------------------'
        */
    [_QWE] = LAYOUT(
        LTFN_ESC,  KC_Q,     KC_W,     KC_E,     KC_R,           KC_T,    KC_Y,           KC_U,      KC_I,     KC_O,     KC_P,      KC_BSPC,
        KC_TAB,    KC_A,     KC_S,     KC_D,     LTSYM_F,        KC_G,    KC_H,           LTSYM_J,   KC_K,     KC_L,     TD_QUOT,   KC_ENT,
        KC_LSFT,   KC_Z,     KC_X,     KC_C,     LT(_ACC, KC_V), KC_B,    KC_N,    LT(_ACC, KC_M),   KC_COMM,  KC_DOT,   SFT_SLSH,  RGB_TOG,
        KC_LCTL,   KC_LGUI,  KC_LALT,  KC_CAPS,  MO(_NAV),            KC_SPC,            MO(_NUM),   KC_LEFT,   KC_DOWN,  KC_UP,    KC_RGHT
    ),

       /* Symbols Layer (Sym)
        * ,----------------------------------------------------------------------------.
        * |      |  `  |  @  |  #  |  $  |  %  |     |  &  |  *  |  ^  |     |  Del    |
        * |----------------------------------------------------------------------------+
        * |       |  ~  |  {  |  -  |  (  |  +  |  =  |  )  |  |  |  }  |   ;  |       |
        * |----------------------------------------------------------------------------+
        * |        |  !  |  <  |  _  |  [  |     |  :  |  ]  |  \  |  >  |  ?   |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_SYM] = LAYOUT(
        _______,  KC_GRV,   KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  XXXXXXX,  KC_AMPR,  KC_ASTR,  KC_CIRC,  XXXXXXX,  KC_DEL,
        _______,  KC_TILD,  KC_LCBR,  KC_PMNS,  KC_LPRN,  KC_PLUS,  KC_EQL,   KC_RPRN,  KC_PIPE,  KC_RCBR,  KC_SCLN,  _______,
        _______,  KC_EXLM,  KC_LT,    KC_UNDS,  KC_LBRC,  XXXXXXX,  KC_COLN,  KC_RBRC,  KC_BSLS,  KC_GT,    KC_QUES,  _______,
        _______,  _______,  _______,  _______,  XXXXXXX,       _______,       XXXXXXX,  _______,  _______,  _______,  _______
    ),

       /* Numbers Layer (Num)
        *
        * Custom2 will act as Ctrl+Shift+Alt+"+" that can be used for example to spawn new terminal instances
        * ,----------------------------------------------------------------------------.
        * |      |     |     |     |     |    |Ctrl-y|     |     |     |     |         |
        * |----------------------------------------------------------------------------+
        * |       |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0   |       |
        * |----------------------------------------------------------------------------+
        * |        |Ctr-z|Ctr-x|Ctr-c|Ctr-v|     |     |     |  ,  |  .  |      |      |
        * |----------------------------------------------------------------------------+
        * |      |     |     |    |Custom2|           |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_NUM] = LAYOUT(
        _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,  REDO,     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______,
        _______,  KC_1,     KC_2,     KC_3,     KC_4,      KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     _______,
        _______,  UNDO,     CUT,      COPY,     PASTE,     XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_COMM,  KC_DOT,   _______,  _______,
        _______,  _______,  _______,  _______,  CUSTOM2,        _______,       _______,  _______,  _______,  _______,  _______
    ),

       /* Navigation Layer (Nav)
        * 
        * This layer is optimized for gui handling and code navigation
        * It makes easier to reach common key combinations as: Ctrl+Shift, Gui+Shift, Ctrl+Alt, Alt+Tab, Ctrl+F, Ctrl+S
        * 
        * Holding (Nav2) replaces arrow keys with Home PgDown PgUp End
        * 
        * 
        * Custom1 will act as Ctrl+Alt+"=" that can be used for example to focus last spawned terminal
        * 
        * ,----------------------------------------------------------------------------.
        * |      |Shift|Shift|Shift|     |     |     |     |     |     |     |         |
        * |----------------------------------------------------------------------------+
        * |(Nav2) |Ctrl | Gui | Alt | Tab |     |Left |Down | Up  |Right|   F  |       |
        * |----------------------------------------------------------------------------+
        * |        |Ctr-z|Ctr-x|Ctr-c|Ctr-v|     |     |     |  L  |     |  A   |  S   |
        * |----------------------------------------------------------------------------+
        * |      |     |     |     |     |            |Custom1|    |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_NAV] = LAYOUT(
        _______,   KC_LSFT,  KC_LSFT,  KC_LSFT,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,
        MO(_NV2),  KC_LCTL,  KC_LGUI,  KC_LALT,  KC_TAB,   _______,  KC_LEFT,  KC_DOWN,   KC_UP,    KC_RGHT,  KC_F,     _______,
        _______,   UNDO,     CUT,      COPY,     PASTE,    _______,  _______,  _______,   KC_L,     KC_DOT,   KC_A,     KC_S,
        _______,   _______,  _______,  _______,  _______,       _______,       CUSTOM1,   _______,  _______,  _______,  _______
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
        * |      |     |     |     |     |     |     | F1  | F2  | F3  | F10 |         |
        * |----------------------------------------------------------------------------+
        * | Print |     |     |     |     |     |     | F4  | F5  | F6  | F11  |       |
        * |----------------------------------------------------------------------------+
        * |ScrLock |     |     |     |     |     |     | F7  | F8  | F9  | F12  |      |
        * |----------------------------------------------------------------------------+
        * | Ins  |     |     |     |     |            |      |     |     |      |      |
        * `----------------------------------------------------------------------------'
        */
    [_FN] = LAYOUT(
        _______,  RGB_TOG, RGB_RMOD, RGB_MOD, RGB_HUD,  RGB_HUI,  XXXXXXX,  KC_F1,    KC_F2,    KC_F3,    KC_F10,  XXXXXXX,
        KC_PSCR,  RGB_SAD, RGB_SAI,  RGB_VAD, RGB_VAI,  RGB_SPD,  RGB_SPI,  KC_F4,    KC_F5,    KC_F6,    KC_F11,  XXXXXXX,
        KC_SLCK,  RGB_M_P, RGB_M_B,  RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K,  KC_F7,    KC_F8,    KC_F9,    KC_F12,  XXXXXXX,
        KC_INS,   XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,       XXXXXXX,       XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX
    ),
           
        /* Accented letters layer
        * 
        */
    [_ACC] = LAYOUT(
        _______, _______, _______, TD(TD_E_GRV_ACU), _______, _______, _______, M_U_GRV, M_I_GRV, M_O_GRV, _______, _______,
        _______, M_A_GRV, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______,          _______, _______,      _______,     _______, _______, _______, _______, _______
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_A_GRV:
            if (record->event.pressed) {
                ls = get_mods() & MOD_BIT(KC_LSHIFT);
                rs = get_mods() & MOD_BIT(KC_RSHIFT);

                if (ls) {
                    unregister_code(KC_LSHIFT);
                }
                if (rs) {
                    unregister_code(KC_RSHIFT);
                }
                register_code(COMPOSE_KEY);
                unregister_code(COMPOSE_KEY);
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                if (ls) {
                    register_code(KC_LSHIFT);
                }
                if (rs) {
                    register_code(KC_RSHIFT);
                }
                register_code(KC_A);
                unregister_code(KC_A);
            }
            break;
        case M_I_GRV:
            if (record->event.pressed) {
                ls = get_mods() & MOD_BIT(KC_LSHIFT);
                rs = get_mods() & MOD_BIT(KC_RSHIFT);

                if (ls) {
                    unregister_code(KC_LSHIFT);
                }
                if (rs) {
                    unregister_code(KC_RSHIFT);
                }
                register_code(COMPOSE_KEY);
                unregister_code(COMPOSE_KEY);
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                if (ls) {
                    register_code(KC_LSHIFT);
                }
                if (rs) {
                    register_code(KC_RSHIFT);
                }
                register_code(KC_I);
                unregister_code(KC_I);
            }
            break;
        case M_O_GRV:
            if (record->event.pressed) {
                ls = get_mods() & MOD_BIT(KC_LSHIFT);
                rs = get_mods() & MOD_BIT(KC_RSHIFT);

                if (ls) {
                    unregister_code(KC_LSHIFT);
                }
                if (rs) {
                    unregister_code(KC_RSHIFT);
                }
                register_code(COMPOSE_KEY);
                unregister_code(COMPOSE_KEY);
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                if (ls) {
                    register_code(KC_LSHIFT);
                }
                if (rs) {
                    register_code(KC_RSHIFT);
                }
                register_code(KC_O);
                unregister_code(KC_O);
            }
            break;
        case M_U_GRV:
            if (record->event.pressed) {
                ls = get_mods() & MOD_BIT(KC_LSHIFT);
                rs = get_mods() & MOD_BIT(KC_RSHIFT);

                if (ls) {
                    unregister_code(KC_LSHIFT);
                }
                if (rs) {
                    unregister_code(KC_RSHIFT);
                }
                register_code(COMPOSE_KEY);
                unregister_code(COMPOSE_KEY);
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                if (ls) {
                    register_code(KC_LSHIFT);
                }
                if (rs) {
                    register_code(KC_RSHIFT);
                }
                register_code(KC_U);
                unregister_code(KC_U);
            }
            break;
    }
    return true;
}