#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits> // for numeric_limits
using namespace std;

class Student {
public:
    string name;
    string index;

    Student(string n, string i) {
        name = n;
        index = i;
    }
};

class AttendanceSession {
public:
    string courseCode;
    string date;
    string startTime;
    int duration;
    vector<pair<string, string>> attendance; // index, status

    AttendanceSession(string c, string d, string s, int dur) {
        courseCode = c;
        date = d;
        startTime = s;
        duration = dur;
    }

    void markAttendance(string index, string status) {
        attendance.push_back({ index, status });
    }

    void displayReport() {
        int present = 0, absent = 0, late = 0;
        cout << "\nAttendance Report for " << courseCode << " on " << date << endl;
        for (auto& record : attendance) {
            cout << "Student " << record.first << " - " << record.second << endl;
            if (record.second == "Present") present++;
            else if (record.second == "Absent") absent++;
            else if (record.second == "Late") late++;
        }
        cout << "\nSummary: Present=" << present << ", Absent=" << absent << ", Late=" << late << endl;
    }
};

// Global storage
vector<Student> students;
vector<AttendanceSession> sessions;

// File handling
void saveStudents() {
    ofstream file("students.txt");
    for (auto& s : students) {
        file << s.name << "," << s.index << endl;
    }
    file.close();
}

void loadStudents() {
    ifstream file("students.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            string name = line.substr(0, pos);
            string index = line.substr(pos + 1);
            students.push_back(Student(name, index));
        }
    }
    file.close();
}

// Menu functions
void addStudent() {
    string name, index;
    cout << "Enter student name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover input
    getline(cin, name); // allows full names with spaces

    cout << "Enter index number: ";
    getline(cin, index); // always waits for input

    students.push_back(Student(name, index));
    saveStudents();
    cout << "Student added successfully!\n";
}

void viewStudents() {
    cout << "\nRegistered Students:\n";
    for (auto& s : students) {
        cout << s.name << " (" << s.index << ")\n";
    }
}

void searchStudent() {
    string index;
    cout << "Enter index number to search: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, index);
    for (auto& s : students) {
        if (s.index == index) {
            cout << "Found: " << s.name << " (" << s.index << ")\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void createSession() {
    string course, date, time;
    int duration;
    cout << "Enter course code: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, course);
    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, date);
    cout << "Enter start time (HH:MM): ";
    getline(cin, time);
    cout << "Enter duration (hours): ";
    cin >> duration;
    sessions.push_back(AttendanceSession(course, date, time, duration));
    cout << "Session created successfully!\n";
}

void markAttendance() {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }
    int choice;
    cout << "Select session (0-" << sessions.size() - 1 << "): ";
    cin >> choice;
    if (choice < 0 || choice >= sessions.size()) {
        cout << "Invalid choice.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
    for (auto& s : students) {
        string status;
        cout << "Mark attendance for " << s.name << " (" << s.index << ") [Present/Absent/Late]: ";
        getline(cin, status);
        sessions[choice].markAttendance(s.index, status);
    }
}

void viewReport() {
    if (sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }
    int choice;
    cout << "Select session (0-" << sessions.size() - 1 << "): ";
    cin >> choice;
    if (choice < 0 || choice >= sessions.size()) {
        cout << "Invalid choice.\n";
        return;
    }
    sessions[choice].displayReport();
}

void clearStudents() {
    students.clear(); // remove from memory
    ofstream file("students.txt", ios::trunc); // empty the file
    file.close();
    cout << "All student records have been deleted!\n";
}

int main() {
    loadStudents();
    int option;
    do {
        cout << "\n--- Digital Attendance System ---\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Create Session\n";
        cout << "5. Mark Attendance\n";
        cout << "6. View Report\n";
        cout << "7. Clear All Students\n";
        cout << "0. Exit\n";
        cout << "Choose option: ";
        cin >> option;

        switch (option) {
        case 1: addStudent(); break;
        case 2: viewStudents(); break;
        case 3: searchStudent(); break;
        case 4: createSession(); break;
        case 5: markAttendance(); break;
        case 6: viewReport(); break;
        case 7: clearStudents(); break;
        case 0: cout << "Exiting program...\n"; break;
        default: cout << "Invalid option.\n";
        }
    } while (option != 0);

    return 0;
}

