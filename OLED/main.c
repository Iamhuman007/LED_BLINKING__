#include <msp430.h>

// OLED I2C Address
#define SSD1306_I2C_ADDR  0x3C

// I2C Functions
void I2C_Init();
void I2C_WriteByte(unsigned char addr, unsigned char data);
void OLED_Command(unsigned char command);
void OLED_Init();

// Simple Delay Function
void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_cycles(1000); // Assuming 1MHz clock (1ms delay)
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog timer

    I2C_Init();  // Initialize I2C
    OLED_Init(); // Initialize OLED

    while (1) {
        // OLED Test
        OLED_Command(0xA5); // Entire display ON (for testing)
        delay_ms(1000);
        OLED_Command(0xA4); // Normal display
        delay_ms(1000);
    }
}

// Initialize I2C for MSP430
void I2C_Init() {
    P1SEL  |= BIT6 + BIT7;  // P1.6 -> SCL, P1.7 -> SDA
    P1SEL2 |= BIT6 + BIT7;

    UCB0CTL1 |= UCSWRST;    // Reset USCI module
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, Synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST; // Use SMCLK, keep in reset state
    UCB0BR0 = 12;   // Set I2C speed to ~100kHz (SMCLK = 1MHz)
    UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST;   // Enable USCI module
}

// Send byte over I2C
void I2C_WriteByte(unsigned char addr, unsigned char data) {
    while (UCB0CTL1 & UCTXSTP); // Ensure stop condition is sent
    UCB0I2CSA = SSD1306_I2C_ADDR; // Set OLED I2C address
    UCB0CTL1 |= UCTR + UCTXSTT; // Transmit mode & Start condition

    while (!(IFG2 & UCB0TXIFG)); // Wait for TX buffer ready
    UCB0TXBUF = addr; // Send register (Command or Data Mode)

    while (!(IFG2 & UCB0TXIFG)); // Wait for TX buffer ready
    UCB0TXBUF = data; // Send data

    while (!(IFG2 & UCB0TXIFG)); // Ensure last byte is sent
    UCB0CTL1 |= UCTXSTP; // Stop condition
}

// Send command to OLED
void OLED_Command(unsigned char command) {
    I2C_WriteByte(0x00, command);
}

// OLED Initialization
void OLED_Init() {
    delay_ms(100); // Wait after power on

    OLED_Command(0xAE); // Display OFF
    OLED_Command(0xD5); // Set Display Clock
    OLED_Command(0x80);
    OLED_Command(0xA8); // Set Multiplex Ratio
    OLED_Command(0x3F);
    OLED_Command(0xD3); // Set Display Offset
    OLED_Command(0x00);
    OLED_Command(0x40); // Set Display Start Line
    OLED_Command(0x8D); // Charge Pump
    OLED_Command(0x14);
    OLED_Command(0xA1); // Segment Remap
    OLED_Command(0xC8); // COM Scan Direction
    OLED_Command(0xDA); // COM Pins Configuration
    OLED_Command(0x12);
    OLED_Command(0x81); // Contrast
    OLED_Command(0x7F);
    OLED_Command(0xD9); // Precharge Period
    OLED_Command(0xF1);
    OLED_Command(0xDB); // VCOMH Level
    OLED_Command(0x40);
    OLED_Command(0xA4); // Resume to RAM
    OLED_Command(0xA6); // Normal Display
    OLED_Command(0xAF); // Display ON
}
