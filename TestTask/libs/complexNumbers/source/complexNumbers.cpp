#include "include/complexNumbers/complexNumbers.h"

template <typename Type>
ComplexNumber<Type>::ComplexNumber(){
    Real = 0;
    Imag = 0;
};

template <typename Type>
ComplexNumber<Type>::ComplexNumber(Type realPart){
    Real = realPart;
}

template <typename Type>
ComplexNumber<Type>::ComplexNumber(Type realPart, Type imaginaryPart){
    Real = realPart;
    Imag = imaginaryPart;
}

template <typename Type>
Type ComplexNumber<Type>::Re() const{
    return Real;
}

template <typename Type>
Type & ComplexNumber<Type>::Re(){
    return Real;
}

template <typename Type>
Type ComplexNumber<Type>::Im() const{
    return Imag;
}

template <typename Type>
Type & ComplexNumber<Type>::Im(){
    return Imag;
}

template <typename Type>
Type ComplexNumber<Type>::Abs(){
    return sqrt(Real*Real + Imag*Imag);
}

template <typename Type>
Type ComplexNumber<Type>::Arg(){
    return atan2(Imag, Real);
}

template <typename Type>
Type ComplexNumber<Type>::ArgDeg(){
    return 180/M_PI*atan2(Imag, Real);
}

template <typename Type>
ComplexNumber<Type> ComplexNumber<Type>::Conj(){
    ComplexNumber num(this->Real, -1*this->Imag);
    return num;
}

// Функция, позволяющая получить указатели на объекты классов CreateComplexFromPhaseRad или CreateComplexFromPhaseGrad
template <typename Type>
Interface* MakeObjectFactory(ObjectType type, Type angleValue){
    if(ObjectType::RAD==type){
        return new CreateComplexFromPhaseRad(angleValue);
    }
    else if(ObjectType::GRAD==type){
        return new CreateComplexFromPhaseGrad(angleValue);
    }
    return nullptr;
}