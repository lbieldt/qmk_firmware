/*
 * ┌───┬───┬───┐                ┌───┬───┬───┐
 * │   │   │   │ LED(pwr)       │   │   │   │
 * ├───┼───┼───┤ LED(D5) (B0)   ├───┼───┼───┤
 * │   │   │   │                │   │   │   │
 * ├───┼───┼───┤                ├───┼───┼───┤
 * │   │   │   │                │   │   │   │
 * └───┴───┴───┘                └───┴───┴───┘
 */


#include QMK_KEYBOARD_H

enum custom_keycodes {
    EM_WK = SAFE_RANGE,  // Row1 Col1
    EM_PR,               // Row1 Col2
    PW_TZR               // Row1 Col3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        /* Row 0 physical positions */
        TO(1), KC_1, KC_X, KC_E, KC_Q, KC_S,
        
        /* Row 1 physical positions */
        EM_WK, EM_PR, PW_TZR, KC_Y, KC_L, KC_K,
        
        /* Row 2 physical positions */
        KC_Q, KC_W, KC_E, KC_J, KC_H, KC_DOT
    ),
    [1] = LAYOUT( //Word commands layer
        /* Row 0 physical positions */
        TO(2), KC_2, KC_3,      LALT(KC_LEFT),  LSFT(KC_F5), LCTL(KC_J),
        
        /* Row 1 physical positions */
        KC_4, KC_5, KC_6,       KC_A,           LCTL(KC_BACKSLASH),       LCTL(KC_EQUAL),
        
        /* Row 2 physical positions */
        KC_7, KC_8, KC_9,       LCTL(KC_X), LCTL(KC_C), LCTL(KC_V)
    ),
    [2] = LAYOUT( //Word commands layer
        /* Row 0 physical positions */
        TO(0), KC_3, KC_3, KC_Q, KC_W, LCTL(KC_J),
        
        /* Row 1 physical positions */
        KC_4, KC_5, KC_6, KC_A, KC_S, KC_D,
        
        /* Row 2 physical positions */
        KC_7, KC_8, KC_9, LCTL(KC_X), LCTL(KC_C), LCTL(KC_V)
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t boot_timer = 0;

    switch (keycode) {
        case (QK_TO | 1):
            if (record->event.pressed) {
                boot_timer = timer_read();
                return true;
            } else {
                if (timer_elapsed(boot_timer) >= 1000) {
                    bootloader_jump();
                }
            }
            return false;

    case EM_WK:
        if (record->event.pressed) {
            SEND_STRING("lbieldt.ctr" SS_AT "x-energy.com");
        }
        return false;

    case EM_PR:
        if (record->event.pressed) {
            SEND_STRING("lbieldt" SS_AT "gmail.com");
        }
        return false;

    case PW_TZR:
        if (record->event.pressed) {
            SEND_STRING("Tzr125" SS_PCT SS_PCT "55");
        }
        return false;


        default:
            return true;
    }
}

void matrix_init_user(void) {
    setPinOutput(B0);  // TX LED pin
    setPinOutput(D5);  // RX LED pin
    writePinHigh(B0);  // Start LEDs off (high = off)
    writePinHigh(D5);
}

void matrix_scan_user(void) {
    static uint8_t prev_layer = 255;  // Track previous layer
    uint8_t current_layer = get_highest_layer(layer_state);
    
    if (current_layer != prev_layer) {  // Only update on layer change
        switch (current_layer) {
            case 0: 
                writePinLow(B0);   // Layer 0: TX on
                writePinHigh(D5);  // RX off
                break;
            case 1: 
                writePinHigh(B0);  // Layer 1: TX off
                writePinLow(D5);   // RX on
                break;
            case 2: 
                writePinLow(B0);  // Layer 2: TX on
                writePinLow(D5);  // RX on
                break;
            default: 
                writePinHigh(B0); 
                writePinHigh(D5); 
                break;
        }
        prev_layer = current_layer;
    }
}
