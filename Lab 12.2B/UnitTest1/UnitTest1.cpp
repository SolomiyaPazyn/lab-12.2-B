#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include "../Lab 12.2B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Ініціалізуємо масив студентів
			Student students[3] = {
				{"Ivanov", 1, ComputerScience, 5, 4, 5}, // programmingGrade = 5
				{"Petrov", 2, Informatics, 4, 3, 4},     // numericalMethodsGrade = 4
				{"Sidorov", 3, LaborTraining, 3, 5, 5}   // pedagogyGrade = 5
			};

			// Зберігаємо у файл
			const char* filename = "test_students.dat";
			ofstream file(filename, ios::binary);
			for (int i = 0; i < 3; i++) {
				file.write(reinterpret_cast<char*>(&students[i]), sizeof(Student));
			}
			file.close();

			// Перевіряємо середню оцінку з фізики
			float avgPhysics = average(filename, 3, Physics);
			Assert::AreEqual(4.0f, avgPhysics, L"Середнє з фізики обчислено неправильно");

			// Перевіряємо середню оцінку з математики
			float avgMath = average(filename, 3, Math);
			Assert::AreEqual(4.0f, avgMath, L"Середнє з математики обчислено неправильно");

			// Перевіряємо середню оцінку з програмування
			float avgProg = average(filename, 3, Prog);
			Assert::AreEqual(5.0f, avgProg, L"Середнє з програмування обчислено неправильно");

		}
	};
}
