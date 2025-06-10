#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const int MAX_STUDENTS = 50;

struct Student {
    string name;
    string roll;
    int quizzes[6];
    int assignments[3];
    int mid, final, project;
    float total;
    string grade;
};

// Manual string to int conversion
int toInt(string str) {
    stringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}

// Grade assignment based on average
string getGrade(float total, float avg) {
    int center = (int)(avg + 0.5);
    if (total < center - 22) return "F";
    else if (total < center - 18) return "D";
    else if (total < center - 14) return "C-";
    else if (total < center - 10) return "C";
    else if (total < center - 6) return "C+";
    else if (total < center - 2) return "B-";
    else if (total <= center + 2) return "B";
    else if (total <= center + 6) return "B+";
    else if (total <= center + 10) return "A-";
    else return "A";
}

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;

    ifstream file("NCA.csv");
    if (!file) {
        cout << " File not found. Please ensure NCA.csv is in the correct folder.\n";
        return 1;
    }

    string header;
    getline(file, header); // skip header

    string line;
    while (getline(file, line) && studentCount < MAX_STUDENTS) {
        istringstream ss(line);
        Student s;
        string temp;

        getline(ss, s.name, ',');
        getline(ss, s.roll, ',');

        for (int i = 0; i < 6; i++) {
            getline(ss, temp, ',');
            s.quizzes[i] = toInt(temp);
        }

        for (int i = 0; i < 3; i++) {
            getline(ss, temp, ',');
            s.assignments[i] = toInt(temp);
        }

        getline(ss, temp, ','); s.mid = toInt(temp);
        getline(ss, temp, ','); s.final = toInt(temp);
        getline(ss, temp);      s.project = toInt(temp);

        students[studentCount++] = s;
    }
    file.close();

    float wQuiz, wAssign, wMid, wFinal, wProj;
    cout << "\n Enter component weightages (total should be 100%)\n";
    cout << "Quiz Weightage: "; cin >> wQuiz;
    cout << "Assignment Weightage: "; cin >> wAssign;
    cout << "Midterm Weightage: "; cin >> wMid;
    cout << "Final Weightage: "; cin >> wFinal;
    cout << "Project Weightage: "; cin >> wProj;

    if (wQuiz + wAssign + wMid + wFinal + wProj != 100) {
        cout << " Error: Total weightage must be 100.\n";
        return 1;
    }

    // Calculate total marks
    float totalSum = 0;
    for (int i = 0; i < studentCount; i++) {
        int quizSum = 0, assignSum = 0;
        for (int j = 0; j < 6; j++) quizSum += students[i].quizzes[j];
        for (int j = 0; j < 3; j++) assignSum += students[i].assignments[j];

        float qPart = (quizSum / 60.0f) * wQuiz;
        float aPart = (assignSum / 30.0f) * wAssign;
        float mPart = (students[i].mid / 100.0f) * wMid;
        float fPart = (students[i].final / 100.0f) * wFinal;
        float pPart = (students[i].project / 40.0f) * wProj;

        students[i].total = qPart + aPart + mPart + fPart + pPart;
        totalSum += students[i].total;
    }

    float average = totalSum / studentCount;

    // Assign grades
    for (int i = 0; i < studentCount; i++) {
        students[i].grade = getGrade(students[i].total, average);
    }

    // Menu
    int option;
    do {
        cout << "\n Menu:\n";
        cout << "1. Display All Student Results\n";
        cout << "2. Save Results to File (result.csv)\n";
        cout << "3. Search Student by Roll Number\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> option;

        if (option == 1) {
            cout << "\n Student Results:\n";
            for (int i = 0; i < studentCount; i++) {
                cout << "Roll: " << students[i].roll
                     << ", Name: " << students[i].name
                     << ", Total: " << students[i].total
                     << ", Grade: " << students[i].grade << "\n";
            }
        } else if (option == 2) {
            ofstream out("result.csv");
            out << "Roll,Name,Total,Grade\n";
            for (int i = 0; i < studentCount; i++) {
                out << students[i].roll << "," << students[i].name << ","
                    << students[i].total << "," << students[i].grade << "\n";
            }
            out.close();
            cout << "Results saved to result.csv\n";
        } else if (option == 3) {
            string searchRoll;
            cout << "Enter Roll Number: ";
            cin >> searchRoll;
            bool found = false;
            for (int i = 0; i < studentCount; i++) {
                if (students[i].roll == searchRoll) {
                    cout << "\n Student Found:\n";
                    cout << "Name: " << students[i].name << "\n";
                    cout << "Total: " << students[i].total << "\n";
                    cout << "Grade: " << students[i].grade << "\n";
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << " No student found with roll " << searchRoll << "\n";
            }
        } else if (option == 4) {
            cout << "Exiting. Goodbye!\n";
        } else {
            cout << " Invalid option. Try again.\n";
        }

    } while (option != 4);

    return 0;
}
