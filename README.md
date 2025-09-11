# Bare-metal-I2C-driver-for-LCD - Atmega328P
Displaying Temperature in LCD from sensor using I2C driver(TWI interface) | ATMega328P - bare metal programming

Blog post: https://embedded-electronicsproject.blogspot.com/2023/11/displaying-temperature-from-sensor-in.html

**Programming:**

There is a dedicated I2C pin available in Arduino (Nano - A4, A5) which are used by respective internal I2C registers for performing I2C protocol (TWI). User has to configure and provide data like clock frequency, slave address, data to transfer, etc. to use the feature.

When using TWI, normally the SDA and SCL pins are a open drain circuit hence we connect resistors between the data lines and 5V. Instead internal built in resistors can also be used by writing '1' to the pins:
digitalWrite(SDA, HIGH);
digitalWrite(SCL, HIGH);

**Bit rate calculation:**

SCL frequency = CPU clock frequency of Nano / 16 + 2(TWBR) x Prescale value

Serial clock speed (SCL) required for I2C interface - NXP PCF8574T is 100KHz

16 + 2(TWBR) x Prescale value = 16MHz / 100KHz
 2(TWBR) x Prescale value  = 160 - 16
TWBR x Prescale value = 144 / 2
TWBR x (1) = 72
TWBR = 72

Find slave address using I2C scanner program from libraries or refer the I2C interface controller datasheet.

I2C has 7-bit or 10-bit address configuration. When using 7-bit configuration, up to 127 devices can be connected. Here, to use the slave address in I2C protocol, convert 8-bit address to 7-bit (shift << 1) and add (| OR) a Read/Write bit to the value.
