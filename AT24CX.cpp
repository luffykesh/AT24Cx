/**

AT24CX.cpp
Library for using the EEPROM AT24C32-512/M01/M02

Original work Copyright (c) 2014 Christian Paul
Modified work Copyright (c) 2019 Rushikesh Patel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

 */
#include "AT24CX.h"
#include <Wire.h>

/**
 * Constructor with AT24Cx EEPROM at given index and size of page
 */
AT24CX::AT24CX(byte index, byte pageSize) {
	init(index, pageSize);
	eepromType=AT24CX_t;
}

/**
 * Constructor with AT24Cx EEPROM at given index(default 0)
 */
AT24C32::AT24C32(byte index) {
	init(index, 32);
	eepromType=AT24C32_t;
}

/**
 * Constructor with AT24C64 EEPROM at given index(default 0)
 */
AT24C64::AT24C64(byte index) {
	init(index, 32);
	eepromType=AT24C64_t;
}

/**
 * Constructor with AT24C128 EEPROM at given index(default 0)
 */
AT24C128::AT24C128(byte index) {
	init(index, 64);
	eepromType=AT24C128_t;
}

/**
 * Constructor with AT24C128 EEPROM at given index(default 0)
 */
AT24C256::AT24C256(byte index) {
	init(index, 64);
	eepromType=AT24C256_t;
}

/**
 * Constructor with AT24C512 EEPROM at given index(default 0)
 */
AT24C512::AT24C512(byte index) {
	init(index, 128);
	eepromType=AT24C512_t;
}

/*
 * Constructor with AT24CM02 EEPROM at given index(default 0)
 */
AT24CM02::AT24CM02(byte index){
	_id = AT24CX_ID | (index<<2 & 0x4);
	_pageSize = 256;
	eepromType=AT24CM02_t;
	Wire.begin();
}

/*
 * Constructor with AT24CM01 EEPROM at given index(default 0)
 */
AT24CM01::AT24CM01(byte index){
	_id = AT24CX_ID | (index<<1 & 0x6);
	_pageSize = 256;
	eepromType=AT24CM01_t;
	Wire.begin();
}

/**
 * Init
 */
void AT24CX::init(byte index, byte pageSize) {
	_id = AT24CX_ID | (index & 0x7);
	_pageSize = pageSize;
	Wire.begin();
}

/**
 * Write byte
 */
void AT24CX::write(unsigned int address, byte data) {
    Wire.beginTransmission(_id);
    if(Wire.endTransmission()==0) {
    	Wire.beginTransmission(_id);
    	Wire.write(address >> 8);
    	Wire.write(address & 0xFF);
      	Wire.write(data);
    	Wire.endTransmission();
    	delay(10);
    }
}

/**
 * Write integer
 */
void AT24CX::writeInt(unsigned int address, unsigned int data) {
	write(address, (byte*)&data, 2);
}

/**
 * Write long
 */
void AT24CX::writeLong(unsigned int address, unsigned long data) {
	write(address, (byte*)&data, 4);
}

/**
 * Write float
 */
void AT24CX::writeFloat(unsigned int address, float data) {
	write(address, (byte*)&data, 4);
}

/**
 * Write double
 */
void AT24CX::writeDouble(unsigned int address, double data) {
	write(address, (byte*)&data, 8);
}

/**
 * Write chars
 */
void AT24CX::writeChars(unsigned int address, char *data, int length) {
	write(address, (byte*)data, length);
}

/**
 * Read integer
 */
unsigned int AT24CX::readInt(unsigned int address) {
	memset(_b,0,sizeof(unsigned int));//for platforms that have 32bit integers
	read(address, _b, 2);
	return *(unsigned int*)&_b[0];
}

/**
 * Read long
 */
unsigned long AT24CX::readLong(unsigned int address) {
	read(address, _b, 4);
	return *(unsigned long*)&_b[0];
}

/**
 * Read float
 */
float AT24CX::readFloat(unsigned int address) {
	read(address, _b, 4);
	return *(float*)&_b[0];
}

/**
 * Read double
 */
double AT24CX::readDouble(unsigned int address) {
	read(address, _b, 8);
	return *(double*)&_b[0];
}

/**
 * Read chars
 */
void AT24CX::readChars(unsigned int address, char *data, int n) {
	read(address, (byte*)data, n);
}

/**
 * Write sequence of n bytes
 */
void AT24CX::write(unsigned int address, byte *data, int n) {
	// status quo
	int c = n;						// bytes left to write
	int offD = 0;					// current offset in data pointer
	int offP;						// current offset in page
	int nc = 0;						// next n bytes to write

	// write all bytes in multiple steps
	while (c > 0) {
		// calc offset in page
		offP = address % _pageSize;
		// maximal 30 bytes to write
		nc = min(min(c, 30), _pageSize - offP);
		write(address, data, offD, nc);
		c-=nc;
		offD+=nc;
		address+=nc;
		if(address%65536==0)
		{
			//re-calculate i2c address in case of AT24CM01 and AT24CM02 when crossing 65536 bytes border
			if(eepromType==AT24CM02_t)
				_id=(_id&0xFC)|(address>>16&0x3);
			else if(eepromType==AT24CM01_t)
				_id=(_id&0xFE)|(address>>16&0x1);
		}
	}
}

/**
 * Write sequence of n bytes from offset
 */
void AT24CX::write(unsigned int address, byte *data, int offset, int n) {
  Wire.beginTransmission(_id);
  if (Wire.endTransmission()==0) {
   	Wire.beginTransmission(_id);
  	Wire.write(address >> 8);
  	Wire.write(address & 0xFF);
  	byte *adr = data+offset;
  	Wire.write(adr, n);
  	Wire.endTransmission();
  	delay(10);
  }
}

/**
 * Read byte
 */
byte AT24CX::read(unsigned int address) {
	byte b = 0;
	int r = 0;
	Wire.beginTransmission(_id);
    if (Wire.endTransmission()==0) {
     	Wire.beginTransmission(_id);
    	Wire.write(address >> 8);
    	Wire.write(address & 0xFF);
    	if (Wire.endTransmission()==0) {
			Wire.requestFrom(_id, 1);
			while (Wire.available() > 0 && r<1) {
				b = (byte)Wire.read();
				r++;
			}
    	}
    }
    return b;
}

/**
 * Read sequence of n bytes
 */
void AT24CX::read(unsigned int address, byte *data, int n) {
	int c = n;
	int offD = 0;
	int offP;						// current offset in page
	int nc = c;
	// read until are n bytes read
	while (c > 0) {
		// calc offset in page
		offP = address % _pageSize;
		// read maximal 32 bytes
		nc = min(min(c,32), _pageSize - offP);
		read(address, data, offD, nc);
		address+=nc;
		offD+=nc;
		c-=nc;
		if(address%65536==0)
		{
			//re-calculate i2c id in case of CM01 and CM02, when page border encountered
			if(eepromType==AT24CM02_t)
				_id=(_id&0xFC)|(address>>16&0x3);
			else if(eepromType==AT24CM01_t)
				_id=(_id&0xFE)|(address>>16&0x1);
		}
	}
}


/**
 * Read sequence of n bytes to offset
 */
void AT24CX::read(unsigned int address, byte *data, int offset, int n) {
	Wire.beginTransmission(_id);
    if (Wire.endTransmission()==0) {
     	Wire.beginTransmission(_id);
    	Wire.write(address >> 8);
    	Wire.write(address & 0xFF);
    	if (Wire.endTransmission()==0) {
			int r = 0;
    		Wire.requestFrom(_id, n);
			while (Wire.available() > 0 && r<n) {
				data[offset+r] = (byte)Wire.read();
				r++;
			}
    	}
    }
}