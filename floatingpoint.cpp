#include "floatingpoint.h"

using namespace std;

FloatingPoint::FloatingPoint(){
    bits.reset();
    setSign(bits[SIZE-1]);
    setExponent(bits);
    setMantissa(bits);

    bitsetToHex();
    bitsetToFloat();
}
FloatingPoint::FloatingPointNaN(){
    //NaN
    bits.set();
    sign = true;
    exponent.set();
    mantissa.set();

    bitsetToHex();
    bitsetToFloat();

    return true;
}

FloatingPoint::FloatingPoint(float num){

    realNum.input = num;
    bitset<SIZE> aux(realNum.output);
    bits = aux;

    setSign(bits[SIZE-1]);
    setExponent(bits);
    setMantissa(bits);

    bitsetToHex();
    bitsetToFloat();

}

FloatingPoint::FloatingPoint(bool sign, int exponent, bitset<MANTISSA_SIZE+1> mantissa){
    this->sign = sign;
    this->exponent = std::bitset<EXPONENT_SIZE>(exponent);
    for(int i = 0; i<MANTISSA_SIZE; i++){
        this->mantissa[i] = mantissa[i];
        bits[i] = mantissa[i];
    }
    int x = 0;
    for(int i = MANTISSA_SIZE; i<MANTISSA_SIZE+EXPONENT_SIZE; i++){
        bits[i] = this->exponent[x];
        x++;
    }
    bits[MANTISSA_SIZE+EXPONENT_SIZE]=sign;

    bitsetToHex();
    bitsetToFloat();
}

void FloatingPoint::setSign(bool sgn){
    sign = sgn;

    bitsetToHex();
    bitsetToFloat();
}

void FloatingPoint::setExponent(std::bitset<SIZE> &bits){
    int x = 7;
    for(int i = SIZE-2; i>SIZE-10; i--){
        exponent[x] = bits[i];
        x--;
    }

    bitsetToHex();
    bitsetToFloat();
}

void FloatingPoint::setMantissa(std::bitset<SIZE> &bits){
    int x = 22;
    for(int i = SIZE-10; i>0; i--){
        mantissa[x] = bits[i];
        x--;
    }

    bitsetToHex();
    bitsetToFloat();
}



void FloatingPoint::bitsetToHex(){
    std::stringstream aux;
    aux << std::hex << std::uppercase << bits.to_ulong();
    hexVal = "0x" + aux.str();
}

float FloatingPoint::bitsetToFloat(){

    float exp= exponent.to_ulong();
    exp-=127;

    int i = MANTISSA_SIZE;
    float x = 0.0;
    float mant;
    while (i>0){
        if(mantissa[i]){
            mant += pow(2.0,-x);
        }
        i--;
        x++;
    }
    mant+=1;

    float result = pow(2.0, exp) * mant;

    if (sign){
        result*=-1;
    }

    return result;
}

bool FloatingPoint::getSign(){
    return sign;
}

std::bitset<EXPONENT_SIZE> FloatingPoint::getExponent(){
    return exponent;
}

std::bitset<MANTISSA_SIZE> FloatingPoint::getMantissa(){
    return mantissa;
}

string FloatingPoint::getHexValue(){
    return hexVal;
}

std::bitset<SIZE> FloatingPoint::getBits()
{
    return bits;
}
float FloatingPoint::getDecValue(){
    if(bits==0)
        return 0;
    float manDec;
    int x=1;
    for(int i=MANTISSA_SIZE-1; i>0; i--){
        if(mantissa[i])
        manDec =  manDec + pow(2,-x);
        x++;
    }
    manDec++;

    float exp = exponent.to_ulong();
    float res = pow(2, exp-127)*manDec;

    if(sign){
        return -res;
    }else{
        return res;
    }

}
