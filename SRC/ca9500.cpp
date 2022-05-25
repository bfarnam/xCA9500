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

#include "ca9500.hpp"
#include <Wire.h>

/// Prerequisites - Wire.begin(); must be called BEFORE CA9500 is invoked

/** 
 * Almost all functions can be called with or without the hex address passed to the function.
 * If the hex address is blank, the function will use the LAST hex address specified.
 * REMEMBER : portBits start at 0
 * When setting port values, you can pass an array, OR HEX
 * i.e. ALL LOW is 0x00 and ALL HIGH is 0xFF
 */


CA9500 :: CA9500( void )
{
	// Constructor Constructor What's Your Function?
}

CA9500 :: ~CA9500()
{
	// Destructor Destruction Oh My!
}

// begin assigns the hex address that is passed and checks to make sure the device is alive returning true or false
// The TwoWire Port does NOT need to be specified
bool CA9500::begin(uint8_t address, TwoWire &wirePort)
{
    _deviceAddress = address;
    _i2cPort = &wirePort;
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() != 0)
        {
            return (false); 										// NO ACKnowledge from requested address
        }
    return (true); 													// ACKknowledge received from requested address
}

// hexAddress assigns a new hex address if using multiple devices
void CA9500::hexAddress(uint8_t address)
{
    _deviceAddress = address;
}

// hexCheck is like begin but checks that the device at address responds properly
bool CA9500::hexCheck(uint8_t address)
{
    _deviceAddress = address;
    _i2cPort->beginTransmission(_deviceAddress);
    if (_i2cPort->endTransmission() != 0)
        {
            return (false); 										// NO ACKnowledge from requested address
        }
    return (true); 													// ACKknowledge received from requested address
}

// ping checks the device and returns the end transmission status code
uint8_t CA9500::ping(uint8_t address)
{
    _deviceAddress = address;
	_i2cPort->beginTransmission(_deviceAddress);
	return (_i2cPort->endTransmission());							// Returns 0 for success, anything else is the error
}

uint8_t CA9500::ping()
{
    _i2cPort->beginTransmission(_deviceAddress);
	return (_i2cPort->endTransmission());							// Returns 0 for success, anything else is the error
	
}

// perform a SOFT reset by setting all registers back to default values
	// pinMode(0xFF);
	// invertPin(0x00);
	// digitalWrite(0x00);
uint8_t CA9500::SoftReset(uint8_t address)
{
	_deviceAddress = address;
	return resetRegisters();
}
uint8_t CA9500::SoftReset()
{
	return resetRegisters();
}

// Register 0x00 - Input Port Register (Read Only Port) - reflects the incoming logic levels - Returns 0 (LOW) or 1 (HIGH)
bool CA9500::digitalRead(uint8_t portBIT, uint8_t address)
{
    _deviceAddress = address;
    _inputValue = readRegister(INPUT_PORT_REGISTER);
    return _inputValue & (1 << portBIT);
}

bool CA9500::digitalRead(uint8_t portBIT)
{
    _inputValue = readRegister(INPUT_PORT_REGISTER);
    return _inputValue & (1 << portBIT);
}

uint8_t CA9500::digitalReadPort(bool *inputValue)
{
    _inputValue = readRegister(INPUT_PORT_REGISTER);
    for (uint8_t bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            inputValue[bitPosition] = _inputValue & (1 << bitPosition);
        }
    return _inputValue;
}

// Register 0x01 - Output Port Register - reflects and sets the outgoing logic levels - Returns the set value
bool CA9500::digitalWrite(uint8_t portBIT, bool bValue, uint8_t address)
{
    _deviceAddress = address;
    _outputValue = readRegister(INPUT_PORT_REGISTER);
    _outputValue &= ~(1 << portBIT);
    _outputValue |= bValue << portBIT;
    return writeRegister(OUTPUT_PORT_REGISTER, _outputValue);
}

bool CA9500::digitalWrite(uint8_t portBIT, bool bValue)
{
    _outputValue = readRegister(INPUT_PORT_REGISTER);
    _outputValue &= ~(1 << portBIT);
    _outputValue |= bValue << portBIT;
    return writeRegister(OUTPUT_PORT_REGISTER, _outputValue);
}

bool CA9500::digitalWrite(bool *outputValue)
{
    _outputValue = 0;
    for (uint8_t bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            _outputValue |= outputValue[bitPosition] << bitPosition;
        }
    return writeRegister(OUTPUT_PORT_REGISTER, _outputValue);
}

// Register 0x02 - Polarity Inversion register - used to invert the polarity of the Input Port Register DATA
// 0 = Input Data Retained (Default Value)
bool CA9500::invertPin(uint8_t portBIT, bool bIsInverted, uint8_t address)
{
    _deviceAddress = address;
    _portInverted &= ~(1 << portBIT);
    _portInverted |= bIsInverted << portBIT;
    return writeRegister(POLARITY_INVERSION_REGISTER, _portInverted);
}

bool CA9500::invertPin(uint8_t portBIT, bool bIsInverted)
{
    _portInverted &= ~(1 << portBIT);
    _portInverted |= bIsInverted << portBIT;
    return writeRegister(POLARITY_INVERSION_REGISTER, _portInverted);
}

bool CA9500::invertPin(bool *bIsInverted)
{
    _portInverted = 0;
    for (uint8_t bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            _portInverted |= bIsInverted[bitPosition] << bitPosition;
        }
    return writeRegister(POLARITY_INVERSION_REGISTER, _portInverted);
}

bool CA9500::isPinInverted(uint8_t portBIT, uint8_t address)
{
	_deviceAddress = address;
    _inputValue = readRegister(POLARITY_INVERSION_REGISTER);
    return _inputValue & (1 << portBIT);
}

bool CA9500::isPinInverted(uint8_t portBIT)
{
	_inputValue = readRegister(POLARITY_INVERSION_REGISTER);
    return _inputValue & (1 << portBIT);
}


/** Register 0x03 - Configuration Register - configures the directions of the I/O pins
 *		0 = OUTPUT
 *		1 = INPUT (Default value)
 *	This is OPPOSITE of Arduino.  In order to use the standard Arduino OUTPUT and INPUT
 *	alias, the bool bDirection is XORd to itself
 *		INPUT_PULLUP is recast to INPUT bfore the XOR
 */

bool CA9500::pinMode(uint8_t portBIT, bool bDirection, uint8_t address)
{
    _deviceAddress = address;
	if (bDirection == INPUT_PULLUP) bDirection = INPUT;
    bDirection = !bDirection;  										// inversion required
    _portMode &= ~(1 << portBIT);
    _portMode |= bDirection << portBIT;
    return writeRegister(CONFIGURATION_REGISTER, _portMode);
}

bool CA9500::pinMode(uint8_t portBIT, bool bDirection)
{
    if (bDirection == INPUT_PULLUP) bDirection = INPUT;
	bDirection = !bDirection;  										// inversion required
    _portMode &= ~(1 << portBIT);
    _portMode |= bDirection << portBIT;
    return writeRegister(CONFIGURATION_REGISTER, _portMode);
}

bool CA9500::pinMode(bool *portMode)
{
    _portMode = 0;
    for (uint8_t bitPosition = 0; bitPosition < 8; bitPosition++)
        {
            if (portMode[bitPosition] == INPUT_PULLUP) portMode[bitPosition] = INPUT;
			_portMode |= !portMode[bitPosition] << bitPosition;  	// inversion required
        }
    return writeRegister(CONFIGURATION_REGISTER, _portMode);
}

bool CA9500::getPinMode(uint8_t portBIT, uint8_t address)
{
	_deviceAddress = address;
    _inputValue = readRegister(CONFIGURATION_REGISTER);
    return _inputValue & (1 << portBIT);
}

bool CA9500::getPinMode(uint8_t portBIT)
{
	_inputValue = readRegister(CONFIGURATION_REGISTER);
    return _inputValue & (1 << portBIT);
}

/** the following sub-functions all assume that the hex address is already set **/

bool CA9500::readBit(uint8_t regAddr, uint8_t bitAddr)
{
    return ((readRegister(regAddr) & (1 << bitAddr)) >> bitAddr);
}

bool CA9500::writeBit(uint8_t regAddr, uint8_t bitAddr, bool bitToWrite)
{
    _portMode &= ~(1 << bitAddr);
    _portMode |= bitToWrite << bitAddr;
    return writeRegister(regAddr, _portMode);
}

uint8_t CA9500::readRegister(uint8_t addr)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(addr);
    _i2cPort->endTransmission();

    //typecasting the 1 parameter in requestFrom so that the compiler
    //doesn't give us a warning about multiple candidates
    if (_i2cPort->requestFrom(static_cast<uint8_t>(_deviceAddress), static_cast<uint8_t>(1)) != 0)
    {
        return _i2cPort->read();
    }
    return false;
}

bool CA9500::writeRegister(uint8_t addr, uint8_t val)
{
    _i2cPort->beginTransmission(_deviceAddress);
    _i2cPort->write(addr);
    _i2cPort->write(val);

    if (_i2cPort->endTransmission() != 0)
	{
        return (false); 							// NO ACKnowledge from requested address
	}
    return(true);									// ACKknowledge recieved from requested address
}

uint8_t CA9500::resetRegisters()
{
	_i2cPort->beginTransmission(_deviceAddress);
	_i2cPort->write(CONFIGURATION_REGISTER);
	_i2cPort->write(0xFF);
	_i2cPort->write(POLARITY_INVERSION_REGISTER);
	_i2cPort->write(0x00);
	_i2cPort->write(OUTPUT_PORT_REGISTER);
	_i2cPort->write(0x00);
	return _i2cPort->endTransmission();
}
