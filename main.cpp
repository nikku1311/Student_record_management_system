#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <windows.h>
#include <sstream>
#include <fstream> // CSV export
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "nikku@13";
const char* DB = "mydb";

class University {
private:
	int Id;
	string Name, Subject;
	float CGPA;

public:
	University() : Id(0), Name(""), Subject(""), CGPA(0.0) {}

	void setId(int id) { Id = id; }
	void setName(string name) { Name = name; }
	void setSubject(string subject) { Subject = subject; }
	void setGrade(float cgpa) { CGPA = cgpa; }

	int getId() { return Id; }
	string getName() { return Name; }
	string getSubject() { return Subject; }
	float getGrade() { return CGPA; }
};

// Insert Function
void insertS(MYSQL* conn, University u) {
	int id;
	string name, subject;
	float cgpa;

	cout << "Enter Id: ";
	cin >> id;
	u.setId(id);
	cout << "Enter Name: ";
	cin >> name;
	u.setName(name);
	cout << "Enter Subject: ";
	cin >> subject;
	u.setSubject(subject);
	cout << "Enter CGPA: ";
	cin >> cgpa;
	u.setGrade(cgpa);

	stringstream ss, as;
	ss << u.getId();
	as << u.getGrade();

	string ins = "INSERT INTO student (Id, Name, Subject, CGPA) VALUES ('" + ss.str() + "', '" + u.getName() + "', '" + u.getSubject() + "', '" + as.str() + "')";

	if (mysql_query(conn, ins.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		cout << "Data Inserted Successfully." << endl;
	}
	Sleep(3000);
}

// Show Function
void showRecord(MYSQL* conn) {
	string show = "SELECT * FROM student";
	if (mysql_query(conn, show.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		MYSQL_RES* res = mysql_store_result(conn);
		if (res) {
			int num = mysql_num_fields(res);
			MYSQL_ROW row;
			while (row = mysql_fetch_row(res)) {
				for (int i = 0; i < num; i++) {
					cout << " " << row[i];
				}
				cout << endl;
			}
		}
	}
	Sleep(5000);
}

// Search Function
void searchS(MYSQL* conn) {
	int id;
	cout << "Enter Student ID: ";
	cin >> id;

	stringstream ss;
	ss << id;
	string show = "SELECT * FROM student WHERE Id ='" + ss.str() + "' ";

	if (mysql_query(conn, show.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		MYSQL_RES* res = mysql_store_result(conn);
		if (res) {
			MYSQL_ROW row = mysql_fetch_row(res);
			if (row) {
				int num = mysql_num_fields(res);
				for (int i = 0; i < num; i++) {
					cout << " " << row[i];
				}
				cout << endl;
			}
			mysql_free_result(res);
		}
	}
	Sleep(5000);
}

// Update Function
void updateS(MYSQL* conn, University u) {
	int id;
	cout << "Enter Id: ";
	cin >> id;

	stringstream ss;
	ss << id;

	string subject;
	cout << "Enter New Subject: ";
	cin >> subject;
	u.setSubject(subject);

	string upd = "UPDATE student SET Subject = '" + u.getSubject() + "' WHERE Id = '" + ss.str() + "'";
	if (mysql_query(conn, upd.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		cout << "Data Updated Successfully" << endl;
	}
	Sleep(3000);
}

// Delete Function
void deleteS(MYSQL* conn) {
	int id;
	cout << "Enter Student ID: ";
	cin >> id;

	stringstream ss;
	ss << id;

	string del = "DELETE FROM student WHERE Id ='" + ss.str() + "' ";
	if (mysql_query(conn, del.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
	} else {
		cout << "Data Deleted" << endl;
	}
	Sleep(3000);
}

// NEW: Export to Excel (CSV)
void exportToCSV(MYSQL* conn) {
	string query = "SELECT * FROM student";
	if (mysql_query(conn, query.c_str())) {
		cout << "Error: " << mysql_error(conn) << endl;
		return;
	}

	MYSQL_RES* res = mysql_store_result(conn);
	if (res) {
		ofstream file("students.csv");
		file << "ID,Name,Subject,CGPA\n";

		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res))) {
			file << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << "\n";
		}
		file.close();
		cout << "Student data exported to students.csv!" << endl;
		mysql_free_result(res);
	} else {
		cout << "No records found or error retrieving data." << endl;
	}
	Sleep(3000);
}

// MAIN
int main() {
	University u;
	MYSQL* conn;
	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
		cout << "Error: " << mysql_error(conn) << endl;
		return 1;
	} else {
		cout << "Logged in!" << endl;
	}
	Sleep(2000);

	bool exit = false;
	while (!exit) {
		system("cls");
		int val;
		cout << "Welcome to University Management System" << endl;
		cout << "***************************************" << endl;
		cout << "1. Insert Student Data" << endl;
		cout << "2. Show Student Data" << endl;
		cout << "3. Search Student Data" << endl;
		cout << "4. Update Student Data" << endl;
		cout << "5. Delete Student Data" << endl;
		cout << "6. Export Student Data to Excel (CSV)" << endl;
		cout << "0. Exit" << endl;
		cout << "Enter choice: ";
		cin >> val;

		cout << endl;

		if (val == 1) insertS(conn, u);
		else if (val == 2) showRecord(conn);
		else if (val == 3) searchS(conn);
		else if (val == 4) updateS(conn, u);
		else if (val == 5) deleteS(conn);
		else if (val == 6) exportToCSV(conn);
		else if (val == 0) {
			exit = true;
			cout << "Good Luck!" << endl;
			Sleep(2000);
		} else {
			cout << "Invalid Input" << endl;
			Sleep(1000);
		}
	}
	return 0;
}
