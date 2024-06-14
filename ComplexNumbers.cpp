#include <iostream>
#include <string>
#include <cmath>
using namespace std;


template <class Type>
class ComplexNumber
{
private:
    Type Real;
    Type Imag;
public:
    // Конструктор по умолчанию
    ComplexNumber(){
        Real = 0;
        Imag = 0;
    };
    // Конструктор от одного аргумента
    ComplexNumber(Type realPart){
        Real = realPart;
    }
    // Конструктор от двух аргументов
    ComplexNumber(Type realPart, Type imaginaryPart){
        Real = realPart;
        Imag = imaginaryPart;
    }

    // Реализация метода Re() для чтения
    Type Re() const{
        return Real;
    }

    // Реализация метода Re() для модификации
    Type & Re(){
        return Real;
    }

    // Реализация метода Im() для чтения
    Type Im() const{
        return Imag;
    }

    // Реализация метода Im() для модификации
    Type & Im(){
        return Imag;
    }

    // Метод, возвращающий модуль
    Type Abs(){
        return sqrt(Real*Real + Imag*Imag);
    }

    // Метод, возвращающий фазу в радианах
    Type Arg(){
        return atan2(Imag, Real);
    }

    // Метод, возвращающий фазу в градусах
    Type ArgDeg(){
        return 180/M_PI*atan2(Imag, Real);
    }

    ComplexNumber Conj(){
        ComplexNumber num(this->Real, -1*this->Imag);
        return num;
    }
};

// Создаем интерфейс, от которого будут наследоваться 2 класса
class Interface
{
    public:
        virtual ~Interface() {}
        virtual ComplexNumber<double> generate() = 0;
};

// Класс для создания комплексного аргумента по фазе в радианах
class CreateComplexFromPhaseRad: public Interface
{
    public:
        CreateComplexFromPhaseRad(double phaseRad) {
            this->phaseRad = phaseRad;
        }
        ComplexNumber<double> generate(){
            return ComplexNumber(cos(phaseRad), sin(phaseRad));
        }
    private:
        double phaseRad = 0;
};

// Класс для создания комплексного аргумента по фазе в градусах
class CreateComplexFromPhaseGrad: public Interface
{
    public:
        CreateComplexFromPhaseGrad(double phaseGrad) {
            this->phaseGrad = phaseGrad;
        }
        ComplexNumber<double> generate(){
            return ComplexNumber(cos(phaseGrad*M_PI/180), sin(phaseGrad*M_PI/180));
        }
    private:
        double phaseGrad = 0;
};

// Класс, фиксирующий типы передаваемых в функцию MakeObjectFactory первых параметров
enum class ObjectType {RAD, GRAD};

// Функция, позволяющая получить указатели на объекты классов CreateComplexFromPhaseRad или CreateComplexFromPhaseGrad
Interface* MakeObjectFactory(ObjectType type, double angleValue){
    if(ObjectType::RAD==type){
        return new CreateComplexFromPhaseRad(angleValue);
    }
    else if(ObjectType::GRAD==type){
        return new CreateComplexFromPhaseGrad(angleValue);
    }
    return nullptr;
}

int main(){
    // Создаем 3 различных комплексных числа (задействованы различные конструкторы)
    ComplexNumber<int> num1;
    ComplexNumber<float> num2(1.5);
    ComplexNumber<double> num3(3.24, 4.15);

    // Получаем значения Re и Im для каждого числа
    cout << num1.Re() << " " << num1.Im() << endl;
    cout << num2.Re() << " " << num2.Im() << endl;
    cout << num3.Re() << " " << num3.Im() << endl;
    
    // Изменяем мнимую и действительную часть числа num3
    // Получаем ссылку на действит часть
    double & Re3 = num3.Re();
    // Присваиваем новое значение
    Re3 = 6.78;
    // Получаем ссылку на мнимую часть
    double & Im3 = num3.Im();
    // Присваиваем новое значение
    Im3 = 8.92;

    // Выводим результат
    cout << num3.Re() << " " << num3.Im() << endl;


    ComplexNumber<double> num4(1, 1);
    // Метод Abs
    cout << num4.Abs() << endl;
    // Метод Arg
    cout << num4.Arg() << endl;
    // Метод ArgDeg
    cout << num4.ArgDeg() << endl;
    // Метод Conj
    ComplexNumber<double> num5 = num4.Conj();
    cout << num5.Re() << " " << num5.Im() << endl;


    // Тестируем фабрики
    Interface* ComplexNum1 = MakeObjectFactory(ObjectType::RAD, M_PI/3);
    Interface* ComplexNum2 = MakeObjectFactory(ObjectType::GRAD, 45);

    ComplexNumber num6 = ComplexNum1->generate();
    ComplexNumber num7 = ComplexNum2->generate();

    // Проверка
    cout << "Действительные и мнимые части комплексных чисел, созданных с помощью фабрик" << endl;
    cout << num6.Re() << " " << num6.Im() << endl;
    cout << num7.Re() << " " << num7.Im() << endl;
    // Удаляем объекты
    delete ComplexNum1;
    delete ComplexNum2;
}
