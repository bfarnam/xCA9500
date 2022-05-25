/*
 * 
 * ca9500.cpp
 * 
 * AVR Library for NXP PCA9538PW/Q900 Automotive Grade Remote 8-bit 
 * I2C / SMBus Low Power I/O Port Expander w/Interrupt and Reset.
 * 
 * This Library was written to be code modular and allow multiple devices
 * to co-exist on the same I2C bus.  The library was then ported over into
 * the Arduino ecosystem.  This library should be compatible with 
 * all xCA95xx devices.  
 * 
 * The device address is not hard coded into the library.
 * 
 * Written by Brett A. Farnam
 * 
 * Licensed Under CC BY-NC-SA
 * 
 * Copyright (c) 2021 Brett A. Farnam (brett_farnam@yahoo.com)
 * 
 */

#ifndef CA9500_hpp
#define CA9500_hpp

#include <Arduino.h>
#include <Wire.h>

class CA9500
{
    public:

        CA9500();
        ~CA9500();

        // Intialize
        //bool begin();
        bool begin(uint8_t address, TwoWire &wirePort = Wire);

        // Change Address - no error code return
        void hexAddress(uint8_t address);

        // Change Address and check to make sure responds
        bool hexCheck(uint8_t address);

        // check a device and make sure a response is recieved
        uint8_t ping(uint8_t address);
        uint8_t ping();
        
        // resets all registers back to default values
        uint8_t SoftReset(uint8_t address);
        uint8_t SoftReset();
                
        // Scan Bus
        // to be implemented later

        // pinMode Configuration Register 0x03
        bool pinMode(uint8_t portBIT, bool bDirection, uint8_t address);
        bool pinMode(uint8_t portBIT, bool bDirection);
        bool pinMode(bool *bDirection);
        bool getPinMode(uint8_t portBIT, uint8_t address);
        bool getPinMode(uint8_t portBIT);

        // invertPin Inversion Register 0x02
        bool invertPin(uint8_t portBIT, bool bIsInverted, uint8_t address);
        bool invertPin(uint8_t portBIT, bool bIsInverted);
        bool invertPin(bool *bIsInverted);
        bool isPinInverted(uint8_t portBIT, uint8_t address);
        bool isPinInverted(uint8_t portBIT);
        
        // digitalWrite Output Register 0x01
        bool digitalWrite(uint8_t portBIT, bool bValue, uint8_t address);
        bool digitalWrite(uint8_t portBIT, bool bValue);
        bool digitalWrite(bool *outputValue);

        // digitalRead Input Register 0x00
        bool digitalRead(uint8_t portBIT, uint8_t address);
        bool digitalRead(uint8_t portBIT);
        uint8_t digitalReadPort(bool *inputValue);

        bool readBit(uint8_t regAddr, uint8_t bitAddr);
        bool writeBit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite);
        uint8_t readRegister(uint8_t addr);
        bool writeRegister(uint8_t addr, uint8_t val);
        uint8_t resetRegisters();

    private:

        uint8_t _outputValue = 0xFF;
        uint8_t _inputValue;
        uint8_t _portMode = 0xFF;
        uint8_t _portInverted = 0;
        uint8_t _deviceAddress;
        TwoWire *_i2cPort;
};

enum {
    INPUT_PORT_REGISTER           = 0x00,
    OUTPUT_PORT_REGISTER          = 0x01,
    POLARITY_INVERSION_REGISTER   = 0x02,
    CONFIGURATION_REGISTER        = 0x03,
};

/*
// Allowed 7-bit Device Addresses in hex and binary
typedef enum {
CA9534_20  = 0x20,	// xCA95 34/54    0100 000x
CA9534_21  = 0x21,  // xCA95 34/54    0100 001x
CA9534_22  = 0x22,  // xCA95 34/54    0100 010x
CA9534_23  = 0x23,  // xCA95 34/54    0100 011x
CA9534_24  = 0x24,  // xCA95 34/54    0100 100x
CA9534_25  = 0x25,  // xCA95 34/54    0100 101x
CA9534_26  = 0x26,  // xCA95 34/54    0100 110x
CA9534_27  = 0x27,  // xCA95 34/54    0100 111x
CA9534A38  = 0x38,  // xCA95 34A/54A  0111 000x
CA9534A39  = 0x39,  // xCA95 34A/54A  0111 001x
CA9534A3A  = 0x3A,  // xCA95 34A/54A  0111 010x
CA9534A3B  = 0x3B,  // xCA95 34A/54A  0111 011x
CA9534A3C  = 0x3C,  // xCA95 34A/54A  0111 100x
CA9534A3D  = 0x3D,  // xCA95 34A/54A  0111 101x
CA9534A3E  = 0x3E,  // xCA95 34A/54A  0111 110x
CA9534A3F  = 0x3F,  // xCA95 34A/54A  0111 111x

CA9538_70  = 0x70,  // XCA9538      1110 000x
CA9538_71  = 0x71,  // XCA9538      1110 001x
CA9538_72  = 0x72,  // XCA9538      1110 010x
CA9538_73  = 0x73,	// XCA9538      1110 011x

CA9557_18  = 0x18,	// XCA9557      0011 000x
CA9557_19  = 0x19,  // XCA9557      0011 001x
CA9557_1A  = 0x1A,  // XCA9557      0011 010x
CA9557_1B  = 0x1B,  // XCA9557      0011 011x
CA9557_1C  = 0x1C,  // XCA9557      0011 100x
CA9557_1D  = 0x1D,  // XCA9557      0011 101x
CA9557_1E  = 0x1E,  // XCA9557      0011 110x
CA9557_1F  = 0x1F,  // XCA9557      0011 111x

CA9571_4A  = 0x4A,  // XCA9571      0100 101x
CA9571_00  = 0x00,  // XCA9571      0000 0000 * SOFTWARE RESET
CA9571_FC  = 0xFC,  // XCA9571      1111 1100 * READ ID INFO
}  
 */


#endif
/** CA9500_hpp **/
