#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// ================= STUDENT CLASS =================
class Student {
public:
    string indexNumber;
    string fullName;
    string programme;
};

// ================= ATTENDANCE RECORD =================
class AttendanceRecord {
public:
    string indexNumber;
    string status;
};

// ================= SESSION CLASS =================
class AttendanceSession {
public:
    string courseCode;
    string date;
    string startTime;
    int duration;

    vector<AttendanceRecord> records;

    void saveSession() {
        string filename = "session_" + courseCode + "_" + date + ".txt";
        ofstream file(filename);

        file << courseCode << endl;
        file << date << endl;
        file << startTime << endl;
        file << duration << endl;

        for (auto &r : records) {
            file << r.indexNumber << "," << r.status << endl;
        }

        file.close();
        cout << "Session saved as: " << filename << endl;
    }

    void loadSession(string filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Session file not found.\n";
            return;
        }

        records.clear();

        getline(file, courseCode);
        getline(file, date);
        getline(file, startTime);
        file >> duration;
        file.ignore();

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            AttendanceRecord r;
            getline(ss, r.indexNumber, ',');
            getline(ss, r.status);
            records.push_back(r);
        }

        file.close();
    }

    void displayReport() {
        int present = 0, absent = 0, late = 0;

        cout << "\n--- Attendance Report ---\n";
        cout << "Course: " << courseCode << endl;
        cout << "Date: " << date << endl;
        cout << "Start Time: " << startTime << endl;
        cout << "Duration: " << duration << " minutes\n\n";

        for (auto &r : records) {
            cout << r.indexNumber << " - " << r.status << endl;

            if (r.status == "Present") present++;
            else if (r.status == "Absent") absent++;
            else if (r.status == "Late") late++;
        }

        cout << "\nSummary:\n";
        cout << "Present: " << present << endl;
        cout << "Absent: " << absent << endl;
        cout << "Late: " << late << endl;
    }
};

// ================= FILE FUNCTIONS =================
void saveStudents(const vector<Student>& students) {
    ofstream file("students.txt");
    for (auto &s : students) {
        file << s.indexNumber << ","
             << s.fullName << ","
             << s.programme << endl;
    }
    file.close();
}

void loadStudents(vector<Student>& students) {
    ifstream file("students.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Student s;

        getline(ss, s.indexNumber, ',');
        getline(ss, s.fullName, ',');
        getline(ss, s.programme);

        students.push_back(s);
    }

    file.close();
}

// ================= SHOW MENU =================
void showMenu() {
    cout << "\n===== DIGITAL ATTENDANCE SYSTEM =====\n";
    cout << "1. Register Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Create Attendance Session\n";
    cout << "5. View Existing Session Report\n";
    cout << "6. Exit\n";
    cout << "Choose option: ";
}

// ================= MAIN =================
int main() {
    vector<Student> students;
    loadStudents(students);

    AttendanceSession session;

    int choice;

    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {

        case 1: {
            Student s;
            cout << "Index Number: ";
            getline(cin, s.indexNumber);
            cout << "Full Name: ";
            getline(cin, s.fullName);
            cout << "Programme: ";
            getline(cin, s.programme);

            students.push_back(s);
            saveStudents(students);
            cout << "Student Registered.\n";
            break;
        }

        case 2:
            for (auto &s : students) {
                cout << s.indexNumber << " - "
                     << s.fullName << " - "
                     << s.programme << endl;
            }
            break;

        case 3: {
            string search;
            cout << "Enter Index Number: ";
            getline(cin, search);
            bool found = false;

            for (auto &s : students) {
                if (s.indexNumber == search) {
                    cout << "Found: "
                         << s.fullName << " - "
                         << s.programme << endl;
                    found = true;
                }
            }

            if (!found)
                cout << "Student not found.\n";
            break;
        }

        case 4: {
            session.records.clear();

            cout << "Course Code: ";
            getline(cin, session.courseCode);
            cout << "Date (YYYY_MM_DD): ";
            getline(cin, session.date);
            cout << "Start Time: ";
            getline(cin, session.startTime);
            cout << "Duration (minutes): ";
            cin >> session.duration;
            cin.ignore();

            for (auto &s : students) {
                AttendanceRecord r;
                r.indexNumber = s.indexNumber;

                cout << "Mark attendance for "
                     << s.fullName
                     << " (Present/Absent/Late): ";
                getline(cin, r.status);

                session.records.push_back(r);
            }

            session.saveSession();
            break;
        }

        case 5: {
            vector<string> sessionFiles;

            for (const auto &entry : fs::directory_iterator(".")) {
                if (entry.path().extension() == ".txt" &&
                    entry.path().filename().string().find("session_") != string::npos) {
                    sessionFiles.push_back(entry.path().filename().string());
                }
            }

            if (sessionFiles.empty()) {
                cout << "No session files found.\n";
                break;
            }

            cout << "\nAvailable Sessions:\n";
            for (size_t i = 0; i < sessionFiles.size(); i++) {
                cout << i + 1 << ". " << sessionFiles[i] << endl;
            }

            cout << "Select session number: ";
            int selection;
            cin >> selection;
            cin.ignore();

            if (selection < 1 || selection > sessionFiles.size()) {
                cout << "Invalid selection.\n";
                break;
            }

            session.loadSession(sessionFiles[selection - 1]);
            session.displayReport();
            break;
        }

        case 6:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid option.\n";
        }

    } while (choice != 6);

    return 0;
}
