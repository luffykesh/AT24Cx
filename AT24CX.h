/**

AT24CX.h
Library for using the EEPROM AT24C32/64

Copyright (c) 2014 Christian Paul

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
#ifndef AT24CX_h
#define AT24CX_h

// includes
#include <Arduino.h>

// byte
typedef uint8_t byte;

// AT24Cx I2C adress
// 80
// 0x50
#define AT24CX_ID B1010000

enum eeprom_type_t{
	AT24CX_t,
	AT24C32_t,
	AT24C64_t,
	AT24C128_t,
	AT24C256_t,
	AT24C512_t,
	AT24CM01_t,
	AT24CM02_t,
};

// general class definition
class AT24CX {
public:
	AT24CX(byte index=0, byte pageSize=32);
	void write(unsigned int address, byte data);
	void write(unsigned int address, byte *data, int n);
	void writeInt(unsigned int address, unsigned int data);
	void writeLong(unsigned int address, unsigned long data);
	void writeFloat(unsigned int address, float data);
	void writeDouble(unsigned int address, double data);
	void writeChars(unsigned int address, char *data, int length);
	byte read(unsigned int address);
	void read(unsigned int address, byte *data, int n);
	unsigned int readInt(unsigned int address);
	unsigned long readLong(unsigned int address);
	float readFloat(unsigned int address);
	double readDouble(unsigned int address);
	void readChars(unsigned int address, char *data, int n);
protected:
	void init(byte index, byte pageSize);
	int _id;
	byte _b[8];
	uint16_t _pageSize;
	eeprom_type_t eepromType;
private:
	void read(unsigned int address, byte *data, int offset, int n);
	void write(unsigned int address, byte *data, int offset, int n);
};

// AT24C32 class definiton
class AT24C32 : public AT24CX {
public:
	AT24C32(byte index=0);
};

// AT24C64 class definiton
class AT24C64 : public AT24CX {
public:
	AT24C64(byte index=0);
};

// AT24C128 class definiton
class AT24C128 : public AT24CX {
public:
	AT24C128(byte index=0);
};

// AT24C256 class definiton
class AT24C256 : public AT24CX {
public:
	AT24C256(byte index=0);
};

// AT24C512 class definiton
class AT24C512 : public AT24CX {
public:
	AT24C512(byte index=0);
};

//AT24CM02 class definition
class AT24CM02 : public AT24CX {
public:
	AT24CM02(byte index=0);
	void write(unsigned int address, byte data){
		_id=(_id&0xFC)|(address>>16&0x3); //insert 17th and 16th bit of data word address in i2c address
		AT24CX::write(address,data);
	}
	void write(unsigned int address, byte *data, int n){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::write(address,data,n);}
	void writeInt(unsigned int address, unsigned int data){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::writeInt(address,data);}
	void writeLong(unsigned int address, unsigned long data){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::writeLong(address,data);}
	void writeFloat(unsigned int address, float data){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::writeFloat(address,data);}
	void writeDouble(unsigned int address, double data){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::writeDouble(address,data);}
	void writeChars(unsigned int address, char *data, int length){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::writeChars(address,data,length);}
	byte read(unsigned int address){_id=(_id&0xFC)|(address>>16&0x3);return AT24CX::read(address);}
	void read(unsigned int address, byte *data, int n){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::read(address,data,n);}
	unsigned int readInt(unsigned int address){_id=(_id&0xFC)|(address>>16&0x3);return AT24CX::readInt(address);}
	unsigned long readLong(unsigned int address){_id=(_id&0xFC)|(address>>16&0x3);return AT24CX::readLong(address);}
	float readFloat(unsigned int address){_id=(_id&0xFC)|(address>>16&0x3);return AT24CX::readFloat(address);}
	double readDouble(unsigned int address){_id=(_id&0xFC)|(address>>16&0x3);return AT24CX::readDouble(address);}
	void readChars(unsigned int address, char *data, int n){_id=(_id&0xFC)|(address>>16&0x3);AT24CX::readChars(address,data,n);}
};

//AT24CM01 class definition
class AT24CM01 : public AT24CX {
public:
	AT24CM01(byte index=0);
	void write(unsigned int address, byte data){
		_id=(_id&0xFE)|(address>>16&0x1); //insert 16th bit of data word address in i2c address
		AT24CX::write(address,data);
	}
	void write(unsigned int address, byte *data, int n){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::write(address,data,n);}
	void writeInt(unsigned int address, unsigned int data){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::writeInt(address,data);}
	void writeLong(unsigned int address, unsigned long data){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::writeLong(address,data);}
	void writeFloat(unsigned int address, float data){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::writeFloat(address,data);}
	void writeDouble(unsigned int address, double data){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::writeDouble(address,data);}
	void writeChars(unsigned int address, char *data, int length){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::writeChars(address,data,length);}
	byte read(unsigned int address){_id=(_id&0xFE)|(address>>16&0x1);return AT24CX::read(address);}
	void read(unsigned int address, byte *data, int n){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::read(address,data,n);}
	unsigned int readInt(unsigned int address){_id=(_id&0xFE)|(address>>16&0x1);return AT24CX::readInt(address);}
	unsigned long readLong(unsigned int address){_id=(_id&0xFE)|(address>>16&0x1);return AT24CX::readLong(address);}
	float readFloat(unsigned int address){_id=(_id&0xFE)|(address>>16&0x1);return AT24CX::readFloat(address);}
	double readDouble(unsigned int address){_id=(_id&0xFE)|(address>>16&0x1);return AT24CX::readDouble(address);}
	void readChars(unsigned int address, char *data, int n){_id=(_id&0xFE)|(address>>16&0x1);AT24CX::readChars(address,data,n);}
};

#endif
