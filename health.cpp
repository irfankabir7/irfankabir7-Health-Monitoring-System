/// Irfanul Hoque Kabir
/// Id:C231221
/*
Blood Pressure
Normal Blood Pressure: Systolic < 120 mmHg and Diastolic < 80 mmHg
High Blood Pressure (Hypertension):
Stage 1: Systolic 130-139 mmHg or Diastolic 80-89 mmHg
Stage 2: Systolic ≥ 140 mmHg or Diastolic ≥ 90 mmHg
Hypertensive Crisis: Systolic > 180 mmHg or Diastolic > 120 mmHg (requires immediate medical attention)
Heart Rate
Normal Resting Heart Rate: 60-100 beats per minute (bpm)
Bradycardia (Low Heart Rate): < 60 bpm (except in physically fit individuals)
Tachycardia (High Heart Rate): > 100 bpm


*/
#include <iostream>
#include <string>
#include <limits>

using namespace std;

struct HealthData {
    string date;
    float systolicBP;
    float diastolicBP;
    float heartRate;
    HealthData* left;
    HealthData* right;

    HealthData(const string& date, float sbp, float dbp, float hr)
        : date(date), systolicBP(sbp), diastolicBP(dbp), heartRate(hr), left(nullptr), right(nullptr) {}
};

struct Patient {
    int id;
    string name;
    Patient* next;
    HealthData* healthData;

    Patient(int id, const string& name)
        : id(id), name(name), next(nullptr), healthData(nullptr) {}
};

void displayMenu() {
    cout << "Health Monitoring System Menu\n";
    cout << "1. Add Patient\n";
    cout << "2. Add Health Data\n";
    cout << "3. Check Patient Alerts\n";
    cout << "4. View Patient Details\n";
    cout << "5. Delete Patient\n";
    cout << "6. Update Patient Information\n";
    cout << "7. View All Patients\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int getValidChoice() {
    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 8) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Enter again: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return choice;
}

Patient* findPatient(Patient* head, int id) {
    Patient* current = head;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void addPatient(Patient*& head, int id, const string& name) {
    if (findPatient(head, id)) {
        cout << "Patient with ID " << id << " already exists.\n";
        return;
    }

    Patient* newPatient = new Patient(id, name);
    newPatient->next = head;
    head = newPatient;
    cout << "Patient added successfully.\n";
}

HealthData* addHealthData(HealthData* root, const string& date, float systolicBP, float diastolicBP, float heartRate) {
    if (!root) {
        return new HealthData(date, systolicBP, diastolicBP, heartRate);
    }

    if (date < root->date) {
        root->left = addHealthData(root->left, date, systolicBP, diastolicBP, heartRate);
    } else if (date > root->date) {
        root->right = addHealthData(root->right, date, systolicBP, diastolicBP, heartRate);
    }

    return root;
}

bool addHealthDataToPatient(Patient* head, int id, const string& date, float systolicBP, float diastolicBP, float heartRate) {
    Patient* patient = findPatient(head, id);
    if (!patient) {
        return false;
    }

    patient->healthData = addHealthData(patient->healthData, date, systolicBP, diastolicBP, heartRate);
    return true;
}

void checkAndDisplayAlert(HealthData* node, const string& patientName, int patientID, bool& anyAlert) {
    if (!node) return;

    checkAndDisplayAlert(node->left, patientName, patientID, anyAlert);

    bool hasAlert = false;
    string alertMessage;

    if (node->systolicBP >= 140 || node->diastolicBP >= 90) {
        alertMessage = "High Blood Pressure";
        hasAlert = true;
    } else if (node->systolicBP > 180 || node->diastolicBP > 120) {
        alertMessage = "Hypertensive Crisis";
        hasAlert = true;
    } else if (node->heartRate < 60) {
        alertMessage = "Low Heart Rate (Bradycardia)";
        hasAlert = true;
    } else if (node->heartRate > 100) {
        alertMessage = "High Heart Rate (Tachycardia)";
        hasAlert = true;
    }

    if (hasAlert) {
        cout << "Patient ID: " << patientID << ", Name: " << patientName << ", Alert: " << alertMessage << " on " << node->date << "\n";
        anyAlert = true;
    }

    checkAndDisplayAlert(node->right, patientName, patientID, anyAlert);
}

void checkAllPatientAlerts(Patient* head) {
    Patient* current = head;
    bool anyAlerts = false;

    while (current) {
        bool patientHasAlert = false;
        checkAndDisplayAlert(current->healthData, current->name, current->id, patientHasAlert);
        if (patientHasAlert) {
            anyAlerts = true;
        }
        current = current->next;
    }

    if (!anyAlerts) {
        cout << "No alerts for any patients.\n";
    }
}

void viewHealthData(HealthData* root) {
    if (!root) return;
    viewHealthData(root->left);
    cout << "Date: " << root->date << ", Systolic BP: " << root->systolicBP << ", Diastolic BP: " << root->diastolicBP << ", HR: " << root->heartRate << "\n";
    viewHealthData(root->right);
}

void viewPatientDetails(Patient* head, int id) {
    Patient* patient = findPatient(head, id);
    if (!patient) {
        cout << "Patient with ID " << id << " not found.\n";
        return;
    }

    cout << "Patient ID: " << patient->id << ", Name: " << patient->name << "\n";
    viewHealthData(patient->healthData);
}

void deleteHealthData(HealthData* node) {
    if (!node) return;
    deleteHealthData(node->left);
    deleteHealthData(node->right);
    delete node;
}

void deletePatient(Patient*& head, int id) {
    Patient* current = head;
    Patient* previous = nullptr;

    while (current && current->id != id) {
        previous = current;
        current = current->next;
    }

    if (!current) {
        cout << "Patient with ID " << id << " not found.\n";
        return;
    }

    if (previous) {
        previous->next = current->next;
    } else {
        head = current->next;
    }

    deleteHealthData(current->healthData);
    delete current;
    cout << "Patient deleted successfully.\n";
}

void updatePatient(Patient* head, int id, const string& newName) {
    Patient* patient = findPatient(head, id);
    if (!patient) {
        cout << "Patient with ID " << id << " not found.\n";
        return;
    }

    patient->name = newName;
    cout << "Patient information updated successfully.\n";
}

void viewAllPatients(Patient* head) {
    if (!head) {
        cout << "No patients found.\n";
        return;
    }

    Patient* current = head;
    while (current) {
        cout << "Patient ID: " << current->id << ", Name: " << current->name << "\n";
        current = current->next;
    }
}

int getValidID() {
    int id;
    while (true) {
        cin >> id;
        if (cin.fail() || id <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid ID. Enter again: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return id;
}

float getValidFloat(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

int main() {
    Patient* patientList = nullptr;
    int choice;

    do {
        displayMenu();
        choice = getValidChoice();

        if (choice == 1) {
            int id;
            string name;
            cout << "Enter Patient ID: ";
            id = getValidID();
            cout << "Enter Patient Name: ";
            getline(cin, name);
            addPatient(patientList, id, name);
        } else if (choice == 2) {
            int id;
            string date;
            float systolicBP, diastolicBP, heartRate;

            cout << "Enter Patient ID: ";
            id = getValidID();

            cout << "Enter Date (YYYY-MM-DD): ";
            getline(cin, date);

            systolicBP = getValidFloat
                        ("Enter Systolic Blood Pressure: ");
            diastolicBP = getValidFloat("Enter Diastolic Blood Pressure: ");
            heartRate = getValidFloat("Enter Heart Rate: ");

            if (addHealthDataToPatient(patientList, id, date, systolicBP, diastolicBP, heartRate)) {
                cout << "Health data added successfully.\n";
            } else {
                cout << "Patient with ID " << id << " not found.\n";
            }
        } else if (choice == 3) {
            cout << "Checking alerts for all patients...\n";
            checkAllPatientAlerts(patientList);
        } else if (choice == 4) {
            int id;
            cout << "Enter Patient ID: ";
            id = getValidID();
            viewPatientDetails(patientList, id);
        } else if (choice == 5) {
            int id;
            cout << "Enter Patient ID: ";
            id = getValidID();
            deletePatient(patientList, id);
        } else if (choice == 6) {
            int id;
            string newName;
            cout << "Enter Patient ID: ";
            id = getValidID();
            cout << "Enter New Name: ";
            getline(cin, newName);
            updatePatient(patientList, id, newName);
        } else if (choice == 7) {
            viewAllPatients(patientList);
        }
    } while (choice != 8);

    cout << "Exiting Health Monitoring System. Goodbye!\n";

    // Clean up allocated memory before exiting
    while (patientList) {
        Patient* temp = patientList;
        patientList = patientList->next;
        deleteHealthData(temp->healthData);
        delete temp;
    }

    return 0;
}

