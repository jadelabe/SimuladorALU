#include "alu.h"

Alu::Alu()
{

}

FloatingPoint Alu::additionFP(FloatingPoint op1, FloatingPoint op2)
{
    if((op1.getExponent()==op2.getExponent()) && (op1.getMantissa()==op2.getMantissa()) && (op1.getSign()!=op2.getSign())){
        return FloatingPoint();

    }

    // 0 Inizialitation of operators.
    std::bitset<MANTISSA_SIZE+1> A;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        A[i]=op1.getMantissa()[i];
    }
    A[A.size()-1] = 1;

    std::bitset<MANTISSA_SIZE+1> B;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        B[i]=op2.getMantissa()[i];
    }
    B[B.size()-1] = 1;

    int expA = op1.getExponent().to_ulong();
    int expB = op2.getExponent().to_ulong();
    bool signA = op1.getSign();
    bool signB = op2.getSign();


// 1.
    const int mantSize = A.size();
    std::bitset<mantSize> P;

    bool guard = 0;
    bool round = 0;
    bool sticky = 0;
    bool interchange_Op = false;
    bool complemented_P = false;
    std::bitset<A.size()> one;
    one.set(0, 1);

// 2.
    if(expA < expB) {
        int tmp = expA;
        expA = expB;
        expB = tmp;

        bool tmp2 = signA;
        signA = signB;
        signB = tmp2;

        std::bitset<A.size()> aux = A;
        A = B;
        B = aux;

        interchange_Op = true;
    }

// 3.
    int expSol = expA;
    int difExp = expA - expB;
    int difAux = difExp;

// 4.
    if(signA != signB) {
        B.flip();
        bitsetAdd(B,one);
    }
// 5.
    P = B;
// 6.
    if (difAux > 1) {
        difAux--;
        guard = P[difAux];
        difAux--;
        round = P[difAux];

        while (difAux > 0){
            difAux--;
            sticky = sticky || P[difAux];
        }
    } else if(difAux == 1) {
        difAux--;
        guard = P[difAux];
    }
// 7.
    bool extraBits = false;
    P = P >> difExp;
    if(signA!=signB){
        extraBits = true;
    }
    for( int i = difExp; i>0; i--){
        P[P.size()-i] = extraBits;
    }
// 8.
    bool carry = bitsetAdd(P, A);
// 9.
    if ((signA!=signB) && (P[mantSize-1]) && !carry){
        bitsetAdd(P.flip(), one);
        complemented_P= true;
    }

//10.

    if ((signA==signB) && carry){
        sticky = guard || round || sticky;
        round = P[0];
        std::bitset<MANTISSA_SIZE+1> P2;
        for(int i = 0; i<MANTISSA_SIZE; i++){
            P2[i]=P[i];
        }

        P2 = P2 >> 1;
        P2.set(23, carry);
        P = P2;

        expSol += 1;
    } else {
        int i = mantSize-1;
        int despl = 0;
        while (!P[i]) {
            despl++;
            i--;
        }
        if(despl == 0) {
            sticky = round || sticky;
            round = guard;
        } else if (despl > 1) {
            round = 0;
            sticky = 0;
        }
        if (despl > 0){
            P = P << 1;
            P[0] = guard;
            P = P << despl-1;
            expSol -= despl;
        }
    }
// 11.
    if((round && sticky) || (round && !sticky && P[23])){
            bool carry2 = bitsetAdd(P, one);
            if(carry2){
                std::bitset<MANTISSA_SIZE+1> P3;
                for(int i = 0; i<MANTISSA_SIZE; i++){
                    P3[i]=P[i];
                }
                P3 = P3 >> 1;
                P3.set(24, carry);
                expSol += 1;
                P = P3;
            }
    }
    bool signSol;
    if(!interchange_Op && complemented_P){
        signSol = signB;
    } else {
        signSol = signA;
    }

    return FloatingPoint(signSol, expSol, P);
}

FloatingPoint Alu::productFP(FloatingPoint op1, FloatingPoint op2)
{
    //FloatingPoint se encarga de que por muy alto q sea el numero, no sea infinito,
    //no entra nunca aqui
    //zero * infinity   (NaN)
    if(((op1.getExponent() == 0 && op1.getMantissa() == 0) ||
        (op2.getExponent() == 0 && op2.getMantissa() == 0)) && (
            (op1.getExponent() == 128 && op1.getMantissa() == 0) ||
            (op2.getExponent() == 128 && op2.getMantissa() == 0))){

        FloatingPoint* a = new FloatingPoint;
        a->FloatingPointNaN();
        std::cout << a->getBits() << std::endl;
        return a->FloatingPointNaN();
    }
    //zero
    if((op1.getExponent() == 0 && op1.getMantissa() == 0) ||
            (op2.getExponent() == 0 && op2.getMantissa() == 0)){
        return FloatingPoint();
    }

    std::bitset<MANTISSA_SIZE+1> A;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        A[i]=op1.getMantissa()[i];
    }
    A[A.size()-1] = 1;

    std::bitset<MANTISSA_SIZE+1> B;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        B[i]=op2.getMantissa()[i];
    }
    B[B.size()-1] = 1;

    int expA = op1.getExponent().to_ulong();
    int expB = op2.getExponent().to_ulong();
    bool signA = op1.getSign();
    bool signB = op2.getSign();

    const int mantSize = A.size();
    std::bitset<mantSize> P;

// 1.
    bool signSol = signA ^ signB;
// 2.
    int expSol = expA-127 + expB-127;
// 3.
    //3.1
    bool carry;
    std::bitset<(MANTISSA_SIZE+1)*2> prod;
    prod = bitsetMultiply(A, B,carry);
    //3.2
    if(!prod[(((MANTISSA_SIZE+1)*2)-1)]){
        prod<<=1;
    } else {
        expSol++;
    }
    //3.3
    bool round = prod[MANTISSA_SIZE];
    //3.4
    int x = MANTISSA_SIZE;
    bool sticky;
    while (x > 0){
        x--;
        sticky = sticky || prod[x];
    }
    //3.5
    if((round && sticky) || (round && !sticky && prod[MANTISSA_SIZE+1])){
        std::bitset<prod.size()> one;
        one.set(0, 1);
        bitsetAdd(prod, one);
    }
        //Desbordamientos a infinito (overflow)
        if(expSol>128){
            expSol=128;
            prod.reset();
        //Desbordamiento a 0 (underflow)
        } else if(expSol<-126){
            int despl = 1-expSol;
            if(despl< MANTISSA_SIZE){
                prod >>= despl;
                expSol = -126;
            }
        }
    std::bitset<MANTISSA_SIZE+1> manSol;
    for(int i = 0; i<MANTISSA_SIZE+1; i++){
        manSol[i] = prod[i+MANTISSA_SIZE+1];
    }
    return FloatingPoint( signSol, expSol+127, manSol);
}

FloatingPoint Alu::divisionFP(FloatingPoint op1, FloatingPoint op2)
{
    //Division por 0
    if((op2.getExponent()==0) && (op2.getMantissa()==0)){
        std::cout << "No se puede dividir por 0" << std::endl;
            FloatingPoint *a = new FloatingPoint();
            return a->FloatingPointNaN();
    }
    //Mismo numero
    if((op1.getExponent()==op2.getExponent()) &&
            (op1.getMantissa()==op2.getMantissa())){
        std::bitset<MANTISSA_SIZE+1> a;
        FloatingPoint one(0, 127, a);
        if(op1.getSign()!=op2.getSign()){ //Distinto signo
            one.setSign(1);

        }
        return one;
    }

    std::bitset<MANTISSA_SIZE+1> A;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        A[i]=op1.getMantissa()[i];
    }
    A[A.size()-1] = 1;

    std::bitset<MANTISSA_SIZE+1> B;
    for(int i = 0; i<MANTISSA_SIZE; i++){
        B[i]=op2.getMantissa()[i];
    }
    B[B.size()-1] = 1;

    int expA = op1.getExponent().to_ulong();
    int expB = op2.getExponent().to_ulong();
    bool signA = op1.getSign();
    bool signB = op2.getSign();

    //Calculo Signo
    bool signSol = signA ^ signB;
    if(bitsetGtEq(A, B)){
        //Alinear Dividendo
    }

    int expSol = (expA-127) - (expB-127);
    if(expSol>expB){
        //comprobar overflow
    } else {
        //comprobar underflow
    }

    std::bitset<MANTISSA_SIZE+2> remainder;    remainder = bitsetDivide(A, B);

    return FloatingPoint();
}


//Acarreo Suma
bool fullAdder(bool b1, bool b2, bool& carry){
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

//Suma
bool Alu::bitsetAdd(std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y){
    bool carry = false;
    for (int i = 0; i < MANTISSA_SIZE+1; i++){
        x[i] = fullAdder(x[i], y[i], carry);
    }
    return carry;
}
bool Alu::bitsetAdd(std::bitset<MANTISSA_SIZE+2>& x, const std::bitset<MANTISSA_SIZE+2>& y){
    bool carry = false;
    for (int i = 0; i < MANTISSA_SIZE+2; i++){
        x[i] = fullAdder(x[i], y[i], carry);
    }
    return carry;
}
bool Alu::bitsetAdd(std::bitset<(MANTISSA_SIZE+1)*2>& x, const std::bitset<(MANTISSA_SIZE+1)*2>& y){
    bool carry = false;
    for (int i = 0; i < MANTISSA_SIZE+1; i++){
        x[i] = fullAdder(x[i], y[i], carry);
    }
    return carry;
}

//Mayor o igual >=
bool Alu::bitsetGtEq(const std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y){
    for (int i=SIZE-1; i >= 0; i--) {
        if (x[i] && !y[i]) return true;
        if (!x[i] && y[i]) return false;
    }
    return true;
}

//Multiplicacion
std::bitset<(MANTISSA_SIZE+1)*2> Alu::bitsetMultiply(std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y, bool &carry){

    std::bitset<MANTISSA_SIZE+1> P;
    std::bitset<(MANTISSA_SIZE+1)*2> result;
    for ( int i = 0; i<MANTISSA_SIZE+1;i++){
        if(x[0]) carry = bitsetAdd(P, y);
        x >>= 1;
        x[MANTISSA_SIZE] = P[0];
        P >>= 1;
        P[MANTISSA_SIZE] = carry;
        carry >>= 1;

    }

    for(int i = 0; i<MANTISSA_SIZE+1; i++){
        result[i]=x[i];
        result[i+MANTISSA_SIZE+1]= P[i];
    }
    return result;      //sobran los 2 primeros
}

//Division
std::bitset<MANTISSA_SIZE+2> Alu::bitsetDivide(std::bitset<MANTISSA_SIZE+1>& x, const std::bitset<MANTISSA_SIZE+1>& y){

    std::bitset<MANTISSA_SIZE+2> one;
    one.set(0, 1);
    std::bitset<MANTISSA_SIZE+2> ycomp;
    for(int i = 0; i<MANTISSA_SIZE+1; i++){
        ycomp[i]=y[i];
    }
    std::bitset<MANTISSA_SIZE+2> y2 = ycomp;
    bitsetAdd(ycomp.flip(), one);

    std::bitset<MANTISSA_SIZE+2>& P = one;
    P.reset();

    int n = MANTISSA_SIZE+1;
    while (n--){
           P<<=1;
           P[0] = x[MANTISSA_SIZE];
           x<<=1;
           bitsetAdd(P, ycomp);
           if(P[MANTISSA_SIZE+1]){
               x[0] = 0;
               bitsetAdd(P,y2);
           } else {
               x[0] = 1;
           }

    }
     std::cout << P << " " << x <<  " " <<std::endl;
    return P;
}
