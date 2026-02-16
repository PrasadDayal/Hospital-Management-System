#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

#define GST 0.18

class Patient {
private:
    int id;
    char name[50];
    int age;
    char disease[50];
    char doctor[50];
    char admissionDate[20];
    char roomType[20];

    float roomCharge;
    float doctorCharge;
    float medicineCharge;
    float gstAmount;
    float totalBill;

public:
    void addPatient();
    void displayPatient();
    void calculateBill();
    void updatePatient();
    void showFullBill();
    int getId();
};

/* ================= BILL LOGIC ================= */

void Patient::calculateBill() {

    if (strcmp(roomType, "General") == 0)
        roomCharge = 2000;
    else if (strcmp(roomType, "Private") == 0)
        roomCharge = 5000;
    else if (strcmp(roomType, "ICU") == 0)
        roomCharge = 10000;
    else
        roomCharge = 0;

    float subtotal = roomCharge + doctorCharge + medicineCharge;
    gstAmount = subtotal * GST;
    totalBill = subtotal + gstAmount;
}

/* ================= ADD ================= */

void Patient::addPatient() {

    cout << "\nEnter Patient ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Name: ";
    cin.getline(name, 50);

    cout << "Enter Age: ";
    cin >> age;
    cin.ignore();

    cout << "Enter Disease: ";
    cin.getline(disease, 50);

    cout << "Enter Doctor Name: ";
    cin.getline(doctor, 50);

    cout << "Enter Admission Date (DD/MM/YYYY): ";
    cin.getline(admissionDate, 20);

    cout << "Room Type (General/Private/ICU): ";
    cin.getline(roomType, 20);

    cout << "Enter Doctor Charge: ";
    cin >> doctorCharge;

    cout << "Enter Medicine Charge: ";
    cin >> medicineCharge;

    calculateBill();
}

/* ================= UPDATE ================= */

void Patient::updatePatient() {

    cin.ignore();

    cout << "\nUpdate Name: ";
    cin.getline(name, 50);

    cout << "Update Age: ";
    cin >> age;
    cin.ignore();

    cout << "Update Disease: ";
    cin.getline(disease, 50);

    cout << "Update Doctor: ";
    cin.getline(doctor, 50);

    cout << "Update Admission Date: ";
    cin.getline(admissionDate, 20);

    cout << "Update Room Type (General/Private/ICU): ";
    cin.getline(roomType, 20);

    cout << "Update Doctor Charge: ";
    cin >> doctorCharge;

    cout << "Update Medicine Charge: ";
    cin >> medicineCharge;

    calculateBill();
}

/* ================= DISPLAY ================= */

void Patient::displayPatient() {

    cout << left << setw(6) << id
         << setw(15) << name
         << setw(6) << age
         << setw(12) << roomType
         << setw(12) << totalBill << endl;
}

/* ================= FULL BILL ================= */

void Patient::showFullBill() {

    cout << "\n========== BILL DETAILS ==========\n";
    cout << "Patient ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Doctor: " << doctor << endl;
    cout << "Room Type: " << roomType << endl;
    cout << "Admission Date: " << admissionDate << endl;
    cout << "----------------------------------\n";
    cout << "Room Charge: " << roomCharge << endl;
    cout << "Doctor Charge: " << doctorCharge << endl;
    cout << "Medicine Charge: " << medicineCharge << endl;
    cout << "GST (18%): " << gstAmount << endl;
    cout << "----------------------------------\n";
    cout << "TOTAL BILL: " << totalBill << endl;
    cout << "==================================\n";
}

int Patient::getId() {
    return id;
}

/* ================= FILE FUNCTIONS ================= */

void addRecord() {
    Patient p;
    ofstream file("hospital.dat", ios::binary | ios::app);
    p.addPatient();
    file.write((char*)&p, sizeof(p));
    file.close();
    cout << "\nRecord Added Successfully!\n";
}

void displayAll() {

    Patient p;
    ifstream file("hospital.dat", ios::binary);

    cout << "\n-------------------------------------------------\n";
    cout << left << setw(6) << "ID"
         << setw(15) << "Name"
         << setw(6) << "Age"
         << setw(12) << "Room"
         << setw(12) << "Bill" << endl;
    cout << "-------------------------------------------------\n";

    while (file.read((char*)&p, sizeof(p))) {
        p.displayPatient();
    }

    file.close();
}

void searchRecord() {

    int id;
    bool found = false;
    Patient p;

    cout << "\nEnter Patient ID: ";
    cin >> id;

    ifstream file("hospital.dat", ios::binary);

    while (file.read((char*)&p, sizeof(p))) {
        if (p.getId() == id) {
            p.showFullBill();
            found = true;
            break;
        }
    }

    file.close();

    if (!found)
        cout << "\nPatient Not Found!\n";
}

void updateRecord() {

    int id;
    bool found = false;
    Patient p;

    cout << "\nEnter Patient ID to Update: ";
    cin >> id;

    fstream file("hospital.dat", ios::binary | ios::in | ios::out);

    while (file.read((char*)&p, sizeof(p))) {
        if (p.getId() == id) {

            p.updatePatient();

            file.seekp(-sizeof(p), ios::cur);
            file.write((char*)&p, sizeof(p));

            cout << "\nRecord Updated Successfully!\n";
            found = true;
            break;
        }
    }

    file.close();

    if (!found)
        cout << "\nPatient Not Found!\n";
}

void deleteRecord() {

    int id;
    bool found = false;
    Patient p;

    cout << "\nEnter Patient ID to Delete: ";
    cin >> id;

    ifstream file("hospital.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);

    while (file.read((char*)&p, sizeof(p))) {
        if (p.getId() != id)
            temp.write((char*)&p, sizeof(p));
        else
            found = true;
    }

    file.close();
    temp.close();

    remove("hospital.dat");
    rename("temp.dat", "hospital.dat");

    if (found)
        cout << "\nRecord Deleted Successfully!\n";
    else
        cout << "\nPatient Not Found!\n";
}

/* ================= MAIN ================= */

int main() {

    int choice;

    do {
        cout << "\n========= HOSPITAL MANAGEMENT SYSTEM =========\n";
        cout << "1. Add Patient\n";
        cout << "2. View All Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Update Patient\n";
        cout << "5. Delete Patient\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayAll(); break;
            case 3: searchRecord(); break;
            case 4: updateRecord(); break;
            case 5: deleteRecord(); break;
            case 6: cout << "\nExiting Program...\n"; break;
            default: cout << "\nInvalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}

