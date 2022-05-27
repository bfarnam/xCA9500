# xCA9500

## Arduino Compatible PCA95xx/TCA95xx i2c GPIO Library

This is a Arduino compatible xCA9500 i2c GPIO Expander library which is compatible with neraly _ALL_ 95xx i2c GPIO devices and allows you to connect to and manipulate multiple devices while only substantiating a single instance of the library and allows standard notation such as HIGH/LOW, INPUT/OUTPUT, digitalRead, digitalWrite, and pinMode!

So far I have tested this with the Texas Instruments TCA9534 8-bit I/O expander with interrupt as used on the SparkFun Qwicc GPIO and with the NXP PCA9538 (Same as TI TCA9538/PCA9538 just higher grade) 8-bit I/O expander with interrupt and reset on both break out boards and in an embedded system.

One of the most annoying things about all the i2c libraries and device handlers is that the library must be invoked more than once to communicate with multiple devices.  I could not find a library that allowed you to address multiple i2c GPIO expanders without having to call the library multiple times using different friendly names.  _Why is this so hard?_

The other annoying thing seems to be that every other i2c library used non-standard calls, instead of digitalWrite, digitalRead, and pinMode as well as utilized non-standard input and output aliases instead of the standard INPUT and OUTPUT.  _Again, why is this so hard?_

Well I was frustrated every time I turned around to test a new i2c device and needed a device specific library from Seed or Spark that used different aliases.

So I said enough is enough and wrote this library to work with _MOST_ i2c devices with out modifying the way you write code or having to invoke the library multiple times and waste precious ram.

For the Arduino IDE, this relies on the standard Wire.h functions.  While this is not preferred due to the buffer size and the interupt disables, it is confirmed to work with 3 devices at 400 kHz clock speed making near simultanious calls using an ATMEGA328P in an embedded controller.  I have tested this down to a crawling speed of 25 kHz and up to 400 kHz.

I will include an example .ino file at a later date for Arduino users but this works just as well using a different two wire library and compiling through your favorite compiler.  Just be sure to update the src files for any compiler changes due to the specific Arduino macros utilized and referenced via Arduino.h.

### Using The Library

#### Includes

The library is included and invoked through a simple include statment.  The library files ca9500.hpp and ca9500.cpp should reside in your source directory.  If the files are placed elsewhere, be sure to include the path.  You can also assign a more friendly name to call the library functions as I have done below.

```CPP
// include the library
#include "ca9500.hpp"

// assign a friendly name
CA9500 _i2c;
```

#### Functions

The following functions are supported:

##### begin(uint8_t address, TwoWire &wirePort)

This attaches the library and should be called AFTER Wire.h or the two wire library of your choice.

Begin takes two parameters.  The first is the HEX address of the first device you are connecting to.  The second is simply "Wire" for the Wire.h library and attaches the wirePort to the i2c handler.  For single port devices, this should never change.  

Returns TRUE if succesful or FALSE if there is an error.

```CPP
// Start Wire for i2c
Wire.begin();

// set the clock speed in Hz for twi - optional as most default to 100 kHz (100000)
Wire.setClock(100000);

// Start the first instance of i2c
_i2c.begin(0x70, Wire);
````

##### hexAddress(uint8_t address)

This assigns a new address in HEX for i2c communications.  This does not check to make sure the device is responding and does not return a result code.

Returns nothing.

```CPP
_i2c.hexAddress(0x71);
```

##### hexCheck(uint8_t address)

Like hexAddress this assigns a new address in HEX for i2c communications.  BUT this actually performs a call to the device to make sure it is alive.  Think of it as a PING.  

Returns TRUE if succesful or FALSE if there is an error.

```CPP
_i2c.hexCheck(0x72);
```

##### ping(uint8_t address)

Like hexAddress this assigns a new address in HEX for i2c communications.  BUT this actually performs a call to the device to make sure it is alive.  Additionally, this returns the actual error code for advanced diagnosis instead of TRUE/FALSE.  

This can also be called without the address and the last used address will be utilized.

Returns 0 for success, anything else is the error.

```CPP
_i2c.ping(0x72);
```

##### digitalRead(uint8_t portBIT, uint8_t address)

This performs a digital read of the specified port bit.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common port is 0x00 for register read.  If your device uses a different register, you will have to change the variable INPUT_PORT_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

This can also be called without the address and the last used address will be utilized.

Returns the value.

```CPP
// Read Port 0 from the last used i2c address
_i2c.digitalRead(0);
```

##### digitalWrite(uint8_t portBIT, bool bValue, uint8_t address)

This performs a digital write of the specified port bit.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common address is 0x01 for register write.  If your device uses a different register, you will have to change the variable OUTPUT_PORT_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

This can also be called without the address and the last used address will be utilized.

Returns the value.

**This is where the magic happens and what allows this i2c handler to address multiple devices.  Just before the write is substantiated, a read call is made to ensure that the current output values are read into a bit register before makeing the modifications and commiting the new output values.**

```CPP
// Set port 2 high for i2c device at address 0x70
_i2c.digitalWrite(2, HIGH, 0x70);
```

##### digitalWrite(bool *outputValue)

This performs a digital write of the entire _PORT_.  The port values are passed in as array.  _This will utilize the last used i2c address._  If a new address is desired, you will have to make a call to hexAddress first.  Generally the common address is 0x01 for register write.  If your device uses a different register, you will have to change the variable OUTPUT_PORT_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

Returns the value.

**This is where the magic happens and what allows this i2c handler to address multiple devices.  Just before the write is substantiated, a read call is made to ensure that the current output values are read into a bit register before makeing the modifications and commiting the new output values.**

```CPP
// define the port output states
// remember that the order is revered in the array compared to the port bits so this is the same as 0b00000011
bool port_output_state[8] = {HIGH,HIGH,LOW,LOW,LOW,LOW,LOW,LOW};

// ensure the i2c address is correct
_i2c.hexAddress(0x72);

// set the port state
_i2c.digitalWrite( port_output_state );
```

##### invertPin(uint8_t portBIT, bool bIsInverted, uint8_t address)

This performs an inversion of the specified port bit.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common address is 0x02 for register inversion.  If your device uses a different register, you will have to change the variable POLARITY_INVERSION_REGISTER in CA9500.hpp.  Generally for most i2c devices, 0 (FALSE) = Input Data Retained (Default Value).  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

This can also be called without the address and the last used address will be utilized.

Returns the value.

```CPP
// invert port 2 for i2c device at address 0x71
_i2c.digitalWrite(2, TRUE, 0x71);
```

##### isPinInverted(uint8_t portBIT, uint8_t address)

This checks to see if a specified port bit is inverted.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common address is 0x02 for register inversion.  If your device uses a different register, you will have to change the variable POLARITY_INVERSION_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

This can also be called without the address and the last used address will be utilized.

Returns the value, 0 (FALSE) = Input Data Retained, 1 (TRUE) = Input Data Inverted

```CPP
// check inversion for port 3 for i2c device at address 0x71
_i2c.isPinInverted(3, 0x71);
```

##### pinMode(uint8_t portBIT, bool bDirection, uint8_t address)

This sets the mode to input or output of the specified port bit.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common address is 0x03 for the port mode.  If your device uses a different register, you will have to change the variable CONFIGURATION_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

**This is where more magic happens and what allows this i2c handler to use standard INPUT and OUTPUT.  The Arduino IDE (and others) define the alias INPTUT as 0 and OUTPUT as 1.  However, most i2c devices utilize 0 as the OUTPUT and 1 as the INPUT, the opposite.  So as the data is constructed the specifed direction in the function is xORd to itself effectviely inverting the data before it is written.**

This can also be called without the address and the last used address will be utilized.

Returns the value.

```CPP
// set port 6 as an OUTPUT for i2c device at address 0x73
_i2c.pinMode(6, OUTPUT, 0x73);
```

##### pinMode(bool *portMode)

This sets the mode to input or output of the specified _PORT_.  The port values are passed in as array.  _This will utilize the last used i2c address._  If a new address is desired, you will have to make a call to hexAddress first.  Generally the common address is 0x03 for the port mode.  If your device uses a different register, you will have to change the variable CONFIGURATION_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device.  

**This is where more magic happens and what allows this i2c handler to use standard INPUT and OUTPUT.  The Arduino IDE (and others) define the alias INPTUT as 0x0 and OUTPUT as 0x1.  However, most i2c devices utilize 0x0 as the OUTPUT and 0x1 as the INPUT, the complete opposite.  So as the data is constructed the specifed direction in the function is xORd to itself effectviely inverting the data before it is written.**

Returns the value.

```CPP
// define the port state
// remember that the order is revered in the array compared to the port bits so this is the same as 0b11101111
bool port_mode[8] = {INPUT,INPUT,INPUT,INPUT,OUTPUT,INPUT,INPUT,INPUT};

// ensure the i2c address is correct
_i2c.hexAddress(0x73);

// set the port state
_i2c.pinMode( port_mode );
```

##### getPinMode(uint8_t portBIT, uint8_t address)

This checks to see if a specified port bit is an output or input.  Port bits are labeled 0-7 for 8 bit devices and 0-15 for 16 bit devices.  Generally the common address is 0x03 for the port mode.  If your device uses a different register, you will have to change the variable CONFIGURATION_REGISTER in CA9500.hpp.  There is NO checking to make sure the port or bit exists.  This _may_ be added later by creating enums for each device. 

This can also be called without the address and the last used address will be utilized.

Returns the value, 0 = OUTPUT, 1 = INPUT  **NOTE:** _This is where the Magic Should Have Happened! The return is NOT inverted. So a return value of 0 is not an input BUT an output.  This is the opposite of what you would expect and the opposite of pinMode above.  I will fix this at a later date._

```CPP
// check the pinMode for port 7 for i2c device at address 0x71
_i2c.getPinMode(7, 0x71);
```

### To Be Implemented

There are several things I have yet to do and will get to them as time allows:

1. Add optional input specifiers for the functions so that multiple functions are not defined both with and without the address
2. Add a method for converting from a HEX address (specified via 0x) to a binary (specified via 0b) and to a decimal (specifed as a whole number)
3. Add the optional address specifier to the PORT functions
4. Ensure that all register functions (Read, Write, Inversion, Mode) are congruent in thier methods of call and variables passed.
5. Add an external function for changing the standard registers (Read, Write, Inversion, Mode) to non-standard address identifiers
6. Change the getPinMode output to match the input of pinMode
7. Add enums for all the common TCA/PCA 95xx GPIO expanders to make it easier and for validation.  Add this as both an external function for specifying the device (like hexAdress) AND as an optional variable input to the existing functions.  Also allow direct modification of the variable data for an OTHER device for custom implmentations.
8. Finish the SoftReset function to initilize a i2c GPIO device and return it to it's just turned on state
9. Add a method for sending eithier a bool array, HEX, BIN, or DEC code to a device when writing the whole port at once.
10. Add a function for inverting the output level (HIGH/LOW) for the entire port at once.

I can't think of anything else right now.

If you would like to contribute, contact me and I would gladly except the help as my free time is _extreamly_ limited.
