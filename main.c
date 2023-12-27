#include "header.h"

int main(void) {
    // parametrit: gpio-tyyppi (täl hetkel vaan "BTN" tai "LED", gpio-määrä, gpio:t (eli voi inittaa monta samanlaista kerralla)
    initialize_gpios("BTN", 1, BUTTON_PIN);
    initialize_gpios("LED", 1, LED_PIN);

    while (1) {
        tight_loop_contents();
    }
}