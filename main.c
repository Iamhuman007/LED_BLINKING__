#include <msp430.h>

void __attribute__((interrupt(WDT_VECTOR))) watchdog_timer(void) {
    P3OUT ^= BIT2;  // Toggle P3.2
}

int main(void) {
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL;  // Use SMCLK for WDT, interval mode
    IE1 |= WDTIE;  // Enable WDT interrupt

    P3DIR |= BIT2;  // Set P3.2 as output

    __bis_SR_register(GIE);  // Enable global interrupts

    while (1);  // Keep running
}
