#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

enum Majors { ComputerScience = 1, Informatics, MathematicsAndEconomics, PhysicsAndInformatics, LaborTraining };
enum Subjects { Physics = 1, Math, Prog };

#pragma pack(push, 1)
struct Student {
    char surname[50];
    unsigned course;
    Majors major;
    int physicsGrade;
    int mathGrade;
    union {
        int programmingGrade;
        int numericalMethodsGrade;
        int pedagogyGrade;
    };

    string return_major() {
        // Повертає назву спеціальності як рядок
        switch (major) {
        case ComputerScience: return "Комп'ютерні науки";
        case Informatics: return "Інформатика";
        case MathematicsAndEconomics: return "Математика & Економіка";
        case PhysicsAndInformatics: return "Фізика & Інформатика";
        case LaborTraining: return "Трудове навчання";
        default: return "Невідомо";
        }
    }

    void fill_struct(int i) {
        // Заповнює структуру даними про студента
        int temp;
        cout << "Студент №" << i + 1 << endl;
        cout << "Прізвище: "; cin.ignore(); cin.getline(surname, 50);
        cout << "Курс: "; cin >> course;
        cout << "Спеціальність (Комп'ютерні науки = 1, Інформатика = 2, Математика & Економіка = 3, Фізика & Інформатика = 4, Трудове навчання = 5): ";
        cin >> temp;
        major = static_cast<Majors>(temp);
        cout << "Оцінка з фізики: "; cin >> physicsGrade;
        cout << "Оцінка з математики: "; cin >> mathGrade;

        switch (major) {
        case ComputerScience:
            cout << "Оцінка з програмування: "; cin >> programmingGrade;
            break;
        case Informatics:
            cout << "Оцінка з чисельних методів: "; cin >> numericalMethodsGrade;
            break;
        default:
            cout << "Оцінка з педагогіки: "; cin >> pedagogyGrade;
            break;
        }
    }
};
#pragma pack(pop)

// Заповнює масив студентів і записує їх у файл
void fill_arr(Student* arr, int n, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для запису" << endl;
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        arr[i].fill_struct(i);
        file.write(reinterpret_cast<char*>(&arr[i]), sizeof(Student));
    }
    file.close();
}

// Рахує кількість цифр у числі
int calc_n(int n) {
    int res = 0;
    while (n != 0) {
        n /= 10;
        res++;
    }
    return res;
}

// Визначає довжину найдовшого прізвища серед студентів у файлі
int lenOfLongestSurname(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }
    Student s;
    int res = -1;
    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        int currentLen = strlen(s.surname);
        res = max(res, currentLen);
    }
    file.close();
    return res;
}

// Виводить таблицю з даними про студентів у вигляді красивого заголовка
void printHeader(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }
    Student s;
    int width = lenOfLongestSurname(filename, n);
    width = max(width, 8);
    int maxDigits = calc_n(n);

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 111; i++) {
        cout << "=";
    }
    cout << endl;

    cout << "|" << setw(maxDigits) << right << "№";
    cout << "|" << setw(width) << left << "Прізвище" << "| Курс |     Спеціальність    | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |" << endl;

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 111; i++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "|" << setw(maxDigits) << right << i + 1;
        cout << "|" << setw(width) << left << s.surname;
        cout << "|" << setw(6) << left << s.course;
        cout << "|" << setw(22) << left << s.return_major();
        cout << "|" << setw(8) << left << s.physicsGrade;
        cout << "|" << setw(12) << left << s.mathGrade;

        switch (s.major) {
        case ComputerScience:
            cout << "|" << setw(15) << s.programmingGrade;
            cout << "|" << setw(17) << "-" << "|" << setw(12) << "-" << "|";
            break;
        case Informatics:
            cout << "|" << setw(15) << "-";
            cout << "|" << setw(17) << s.numericalMethodsGrade << "|" << setw(12) << "-" << "|";
            break;
        default:
            cout << "|" << setw(15) << "-";
            cout << "|" << setw(17) << "-" << "|" << setw(12) << s.pedagogyGrade << "|";
            break;
        }
        cout << endl;
    }

    for (int i = 0; i < (maxDigits - 1) + (width - 8) + 111; i++) {
        cout << "-";
    }
    cout << endl;
    file.close();
}

// Рахує кількість студентів, які отримали оцінки 4 або 5 з фізики
int counter(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }
    Student s;
    int result = 0;
    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        if (s.physicsGrade == 4 || s.physicsGrade == 5) {
            result++;
        }
    }
    file.close();
    return result;
}

// Рахує студентів, які отримують усі оцінки "відмінно"
int countExcellentStudents(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }
    Student s;
    int excellentCount = 0;
    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        if (s.physicsGrade == 5 && s.mathGrade == 5) {
            bool isExcellent = false;
            if (s.major == ComputerScience && s.programmingGrade == 5) isExcellent = true;
            else if (s.major == Informatics && s.numericalMethodsGrade == 5) isExcellent = true;
            else if (s.major != ComputerScience && s.major != Informatics && s.pedagogyGrade == 5) isExcellent = true;
            if (isExcellent) excellentCount++;
        }
    }
    file.close();
    return excellentCount;
}

// Обчислює середню оцінку з вказаного предмета
float average(const char* filename, int n, Subjects subject) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }
    Student s;
    int sum = 0;
    int count = 0;

    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        switch (subject) {
        case Physics:
            sum += s.physicsGrade;
            count++;
            break;
        case Math:
            sum += s.mathGrade;
            count++;
            break;
        case Prog:
            if (s.major == ComputerScience) {
                sum += s.programmingGrade;
                count++;
            }
            break;
        }
    }
    file.close();
    return count ? static_cast<float>(sum) / count : 0;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n;
    cout << "Введіть кількість студентів: "; cin >> n;

    Student* arr = new Student[n];

    char filename[100];
    cout << "Введіть назву файлу для збереження: ";
    cin >> filename;

    fill_arr(arr, n, filename);

    printHeader(filename, n);

    cout << "Середнє арифметичне з фізики: " << average(filename, n, Physics) << endl;
    cout << "Середнє арифметичне з математики: " << average(filename, n, Math) << endl;
    cout << "Середнє арифметичне з програмування: " << average(filename, n, Prog) << endl;

    int excellentStudents = countExcellentStudents(filename, n);
    cout << "Процент студентів, які вчаться на 'відмінно': "
        << (static_cast<float>(excellentStudents) / n) * 100 << "%" << endl;
    cout << "Кількість студентів, які отримали 4 або 5 з фізики: "
        << counter(filename, n) << endl;

    delete[] arr;
    return 0;
}
