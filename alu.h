☻#ifndef ALU_H
#define ALU_H
#include "floatingpoint.h"

#include <stdexcept>
#include <bitset>

class Alu
{
public:
    Alu();
    ~Alu();

    FloatingPoint additionFP(FloatingPoint op1, FloatingPoint op2);
    FloatingPoint productFP(FloatingPoint op1, FloatingPoint op2);
    FloatingPoint divisionFP(FloatingPoint op1, FloatingPoint op2);

    bool bitsetAdd(std::bitset<MANTISSA_SIZE+1> &x, const std::bitset<MANTISSA_SIZE+1> &y);
    bool bitsetAdd(std::bitset<(MANTISSA_SIZE+1)*2>& x, const std::bitset<(MANTISSA_SIZE+1)*2>& y);
    bool bitsetAdd(std::bitset<MANTISSA_SIZE+2>& x, const std::bitset<MANTISSA_SIZE+2>& y);
    void bitsetSubtract(std::bitset<(MANTISSA_SIZE+1)>& x, const std::bitset<(MANTISSA_SIZE+1)>& y);

    bool bitsetGtEq(const std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y);

    std::bitset<(MANTISSA_SIZE+1)*2> bitsetMultiply(std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y, bool &carry);
    std::bitset<MANTISSA_SIZE+2> bitsetDivide(std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y);


    int main();

private:
    FloatingPoint operatorFP_1, operatorFP_2;
};
#endif // ALU_H
