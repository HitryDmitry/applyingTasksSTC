#include "include/complexNumbers/complexNumbers.h"
#include <gtest/gtest.h>

/**
 * @brief если этот тест проходит успешно, то конструкторы этого класса реализованны и с созданием объекта всё ок.
 */
TEST(ComplexNumberTest, Creating) {
	ComplexNumber<int> n1;
    ComplexNumber<float> n2(1.5);
    ComplexNumber<double> n3(3.55, 7.96);
}
/**
 * @brief тестируем методы Re и Im
 */
TEST(ComplexNumberTest, CheckCountCreatedObjects_nothing) {
    ComplexNumber<int> n1;
    ComplexNumber<float> n2(1.5);
    ComplexNumber<double> n3(3.55, 7.96);
	ASSERT_EQ(n1.Re(), 0);
    ASSERT_EQ(n2.Re(), 0);
    ASSERT_EQ(n2.Im(), 0);
    ASSERT_EQ(n3.Im(), 7.96);
}
///**
// * @brief тестируем метод
// */
//TEST(ComplexNumberTest, CheckCountCreatedObjects) {
//	Printing::Printer testedObject;
//	Printing::Printer testedObject1;
//	Printing::Printer testedObject2;
//
//	ASSERT_EQ(Printing::Printer::getCountCreatedPrinters(), 3);
//}
///**
// * @brief TEST создаётся три подряд объекта в куче. далее по одному объекты начинают удаляться.
// * после каждого последующего удаления количество созданных принеторов декрементируется.
// * и достигает нуля, когда каждый из созданных обектов уничтожился.
// *
// */
//TEST(ComplexNumberTest, CheckCountCreatedObjectsWithDelete) {
//	Printing::Printer* testedObject = new Printing::Printer();
//	Printing::Printer* testedObject1 = new Printing::Printer();
//	Printing::Printer* testedObject2 = new Printing::Printer();
//
//	delete  testedObject;
//	ASSERT_EQ(Printing::Printer::getCountCreatedPrinters(), 2);
//	delete  testedObject1;
//	ASSERT_EQ(Printing::Printer::getCountCreatedPrinters(), 1);
//	delete  testedObject2;
//	ASSERT_EQ(Printing::Printer::getCountCreatedPrinters(), 0);
//}
///**
// * @brief этот тест проверяет печатает ли принтер вообще что-то, результат виден только в виде вывода в консоль
// */
//TEST(ComplexNumberTest, printing_stock) {
//	Printing::Printer testedObject;
//	testedObject.print(" Hello Word! ");
//}
///**
// * @brief этот тест проверяет печатает ли принтер путём подстановки собственного потока вывода.
// * в данном случае это объект класса std::stringstream
// * результат печати должен находиться в объекте printString, и должен представлять из себя сторку для печати + escape последовательность '\n'
// * '\n' переводит каретку в начало следующей строки
// */
//TEST(ComplexNumberTest, printing_custom) {
//	std::stringstream printString;
//
//	Printing::Printer testedObject(printString);
//
//	testedObject.print(" Hello Word! ");
//
//	auto printerResult = printString.str();
//	ASSERT_EQ(printerResult, (" Hello Word! \n"));
//}
////TODO: добавить тесты для демонстрации многопоточного использования, демонстрации контейнеров и их возможностей
