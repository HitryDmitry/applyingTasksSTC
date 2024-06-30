#ifndef COMPLEXNUMBERS_H
#define COMPLEXNUMBERS_H

#pragma once

#include <iostream>
#include <string>
#include <cmath>

template <class Type>
class ComplexNumber
{
private:
    Type Real;
    Type Imag;
public:
    // Конструктор по умолчанию
    ComplexNumber();
    // Конструктор от одного аргумента
    ComplexNumber(Type realPart);
    // Конструктор от двух аргументов
    ComplexNumber(Type realPart, Type imaginaryPart);

    // Объявление метода Re() для чтения
    Type Re() const;

    // Объявление метода Re() для модификации
    Type & Re();

    // Объявление метода Im() для чтения
    Type Im() const;

    // Объявление метода Im() для модификации
    Type & Im();

    // Объявление метода, возвращающего модуль
    Type Abs();

    // Объявление метода, возвращающего фазу в радианах
    Type Arg();

    // Объявление метода, возвращающего фазу в градусах
    Type ArgDeg();

    // Объявление метода, возвращающего сопряженное число
    ComplexNumber Conj();
};


// Создаем интерфейс, от которого будут наследоваться 2 класса
class Interface
{
public:
    virtual ~Interface() = default;
    virtual ComplexNumber<double> generate() = 0;
};

// Класс для создания комплексного аргумента по фазе в радианах
template <class Type>
class CreateComplexFromPhaseRad: public Interface
{
public:
    CreateComplexFromPhaseRad(Type phaseRad) {
        this->phaseRad = phaseRad;
    }
    ComplexNumber<double> generate(){
        return ComplexNumber(cos(phaseRad), sin(phaseRad));
    }
private:
    Type phaseRad = 0;
};

// Класс для создания комплексного аргумента по фазе в градусах
template <class Type>
class CreateComplexFromPhaseGrad: public Interface
{
public:
    explicit CreateComplexFromPhaseGrad(Type phaseGrad) {
        this->phaseGrad = phaseGrad;
    }
    ComplexNumber<Type> generate() override{
        return ComplexNumber(cos(phaseGrad*M_PI/180), sin(phaseGrad*M_PI/180));
    }
private:
    Type phaseGrad = 0;
};

// Класс, фиксирующий типы передаваемых в функцию MakeObjectFactory первых параметров
enum class ObjectType {RAD, GRAD};

// Функция, позволяющая получить указатели на объекты классов CreateComplexFromPhaseRad или CreateComplexFromPhaseGrad
template <typename Type>
Interface* MakeObjectFactory(ObjectType type, Type angleValue);


#endif //COMPLEXNUMBERS_H

