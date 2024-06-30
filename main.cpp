#include "TestTask/libs/complexNumbers/include/complexNumbers/complexNumbers.h"

int main()
{
// Создаем 3 различных комплексных числа (задействованы различные конструкторы)
    ComplexNumber<int> num1;
    ComplexNumber<float> num2(1.5);
    ComplexNumber<double> num3(3.24, 4.15);

    // Получаем значения Re и Im для каждого числа
    std::cout << num1.Re() << " " << num1.Im() << std::endl;
    std::cout << num2.Re() << " " << num2.Im() << std::endl;
    std::cout << num3.Re() << " " << num3.Im() << std::endl;

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
    std::cout << num3.Re() << " " << num3.Im() << std::endl;


    ComplexNumber<double> num4(1, 1);
    // Метод Abs
    std::cout << num4.Abs() << std::endl;
    // Метод Arg
    std::cout << num4.Arg() << std::endl;
    // Метод ArgDeg
    std::cout << num4.ArgDeg() << std::endl;
    // Метод Conj
    ComplexNumber<double> num5 = num4.Conj();
    std::cout << num5.Re() << " " << num5.Im() << std::endl;


    // Тестируем фабрики
    Interface* ComplexNum1 = MakeObjectFactory(ObjectType::RAD, M_PI/3);
    Interface* ComplexNum2 = MakeObjectFactory(ObjectType::GRAD, 45);

    ComplexNumber num6 = ComplexNum1->generate();
    ComplexNumber num7 = ComplexNum2->generate();

    // Проверка
    std::cout << "Действительные и мнимые части комплексных чисел, созданных с помощью фабрик" << std::endl;
    std::cout << num6.Re() << " " << num6.Im() << std::endl;
    std::cout << num7.Re() << " " << num7.Im() << std::endl;
    // Удаляем объекты
    delete ComplexNum1;
    delete ComplexNum2;
}


//#include "printer/printer.h"
//
//int main()
//{
//    Printing::Printer printer;
//
//    printer.print(" Hello Word! ");
//    std::cerr << " принтеров создано: " << Printing::Printer::getCountCreatedPrinters() << std::endl;
//
//    Printing::Printer* ptrToprinter = nullptr;
//
//    ptrToprinter = new Printing::Printer();
//
//    std::cerr << " принтеров создано: " << Printing::Printer::getCountCreatedPrinters() << std::endl;
//
//    delete ptrToprinter;
//
//    std::cerr << " принтеров создано: " << Printing::Printer::getCountCreatedPrinters() << std::endl;
//
//    return 0;
//}