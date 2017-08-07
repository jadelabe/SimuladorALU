#ifndef FLOATINGPOINT_H
#define FLOATINGPOINT_H

#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <math.h>

static const int SIZE = sizeof(float) * CHAR_BIT;
//static const int SIZE = 32;       //Linux
static const int EXPONENT_SIZE = 8;
static const int MANTISSA_SIZE = 23;



class FloatingPoint
{
public:
    FloatingPoint();
    FloatingPoint(float num);
    FloatingPoint(bool sign, int exponent, std::bitset<MANTISSA_SIZE+1> mantissa);

    FloatingPointNaN();

    void setSign(bool sgn);
    void setExponent(std::bitset<SIZE> &bits);
    void setMantissa(std::bitset<SIZE> &bits);
    void bitsetToHex();
    float bitsetToFloat();

    bool getSign();
    std::bitset<EXPONENT_SIZE> getExponent();
    std::bitset<MANTISSA_SIZE> getMantissa();
    std::string getHexValue();
    std::bitset<SIZE> getBits();
    float getDecValue();

private:
    union {
        float input;
        int output;
    } realNum;

    std::string hexVal;


    std::bitset<SIZE> bits;

    bool sign;
    std::bitset<EXPONENT_SIZE> exponent;
    std::bitset<MANTISSA_SIZE> mantissa;
};

#endif // FLOATINGPOINT_H
