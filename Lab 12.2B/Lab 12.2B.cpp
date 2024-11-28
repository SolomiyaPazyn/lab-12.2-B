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
        // ������� ����� ������������ �� �����
        switch (major) {
        case ComputerScience: return "����'����� �����";
        case Informatics: return "�����������";
        case MathematicsAndEconomics: return "���������� & ��������";
        case PhysicsAndInformatics: return "Գ���� & �����������";
        case LaborTraining: return "������� ��������";
        default: return "�������";
        }
    }

    void fill_struct(int i) {
        // �������� ��������� ������ ��� ��������
        int temp;
        cout << "������� �" << i + 1 << endl;
        cout << "�������: "; cin.ignore(); cin.getline(surname, 50);
        cout << "����: "; cin >> course;
        cout << "������������ (����'����� ����� = 1, ����������� = 2, ���������� & �������� = 3, Գ���� & ����������� = 4, ������� �������� = 5): ";
        cin >> temp;
        major = static_cast<Majors>(temp);
        cout << "������ � ������: "; cin >> physicsGrade;
        cout << "������ � ����������: "; cin >> mathGrade;

        switch (major) {
        case ComputerScience:
            cout << "������ � �������������: "; cin >> programmingGrade;
            break;
        case Informatics:
            cout << "������ � ��������� ������: "; cin >> numericalMethodsGrade;
            break;
        default:
            cout << "������ � ���������: "; cin >> pedagogyGrade;
            break;
        }
    }
};
#pragma pack(pop)

// �������� ����� �������� � ������ �� � ����
void fill_arr(Student* arr, int n, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� ������" << endl;
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        arr[i].fill_struct(i);
        file.write(reinterpret_cast<char*>(&arr[i]), sizeof(Student));
    }
    file.close();
}

// ���� ������� ���� � ����
int calc_n(int n) {
    int res = 0;
    while (n != 0) {
        n /= 10;
        res++;
    }
    return res;
}

// ������� ������� ���������� ������� ����� �������� � ����
int lenOfLongestSurname(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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

// �������� ������� � ������ ��� �������� � ������ ��������� ���������
void printHeader(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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

    cout << "|" << setw(maxDigits) << right << "�";
    cout << "|" << setw(width) << left << "�������" << "| ���� |     ������������    | Գ���� | ���������� | ������������� | ������� ������ | ��������� |" << endl;

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

// ���� ������� ��������, �� �������� ������ 4 ��� 5 � ������
int counter(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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

// ���� ��������, �� ��������� �� ������ "������"
int countExcellentStudents(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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

// �������� ������� ������ � ��������� ��������
float average(const char* filename, int n, Subjects subject) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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
    cout << "������ ������� ��������: "; cin >> n;

    Student* arr = new Student[n];

    char filename[100];
    cout << "������ ����� ����� ��� ����������: ";
    cin >> filename;

    fill_arr(arr, n, filename);

    printHeader(filename, n);

    cout << "������ ����������� � ������: " << average(filename, n, Physics) << endl;
    cout << "������ ����������� � ����������: " << average(filename, n, Math) << endl;
    cout << "������ ����������� � �������������: " << average(filename, n, Prog) << endl;

    int excellentStudents = countExcellentStudents(filename, n);
    cout << "������� ��������, �� ������� �� '������': "
        << (static_cast<float>(excellentStudents) / n) * 100 << "%" << endl;
    cout << "ʳ������ ��������, �� �������� 4 ��� 5 � ������: "
        << counter(filename, n) << endl;

    delete[] arr;
    return 0;
}
