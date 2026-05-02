// Sajana pamuditha ( G21328035 )


#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// Constants for program configuration
const int months_In_Year = 12; 
const double no_Taxable_Income = 0; 
const int max_Employees = 100; 
const double taxable_Income_Threshold = 12570; 
const double tax_Rate = 0.20;

// Structure to store employee details
struct Employee {
    string id; 
    string name; 
    double hourlyRate;
};

// Structure to store payroll details
struct Payroll {
    string id; 
    double hoursWorked; 
    double grossPay; 
    double tax; 
    double netPay;
};

// Function declarations

// Displays the main menu for payroll operations
void displayMenu(Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount);

// Converts a string to uppercase
void toUpperCase(string &str);

// Reads employee data from file and stores it in an array
void readEmployeeData(string filename, Employee employees[max_Employees], int &employeeCount);

// Finds an employee index by their ID in the employee list
int findEmployeeIndex(string id, Employee employees[max_Employees], int employeeCount);

// Reads payroll data from a file and calculates payroll for a given month
void readPayrollData(string filename, int monthIndex, Employee employees[max_Employees], int employeeCount, Payroll monthlyPayroll[12][max_Employees]);

// Writes an error log to a file (for invalid or missing employee data)
void writeErrorLog(string filename);

// Writes payroll data to an output file
void writePayrollToFile(string filename, int monthIndex, Payroll monthlyPayroll[12][max_Employees], int employeeCount);

// Displays payroll details on the console
void displayPayroll(int monthIndex, Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount);

// Searches for a specific employee's payroll details
void searchPayroll(Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount);



// Main function
int main() {

    Employee employees[max_Employees]; // Array to store employee data
    Payroll monthlyPayroll[12][max_Employees]; // 2D array to store payroll data for each month
 
    bool processed = false; 

    int employeeCount = 0; 

    readEmployeeData("employees.txt", employees, employeeCount);
    
    displayMenu(monthlyPayroll, employees, employeeCount);
    
    int monthIndex = 0; 

    return 0; 
}

// Displays the main menu for payroll operations

void displayMenu(Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount) {
    string choice; 
    string filename;
    int monthIndex; 
    bool processed[4] = {false}; 

    while (true) {
    
       // Display menu options to the user
        cout << "\n 📌 Payroll System Menu:\n";
        cout << "1️⃣ Display Payroll\n";
        cout << "2️⃣ Search Employee Payroll\n";
        cout << "3️⃣ Review All Processed Payrolls\n";
   
        cout << "\n👉 Enter your choice (or type 'quit' to exit): ";
        cin >> choice; 


        // Check if the user wants to quit
        if (choice == "quit" || choice == "Quit") {
            cout << "👋 Exiting the program. Goodbye!\n";
         displayMenu(monthlyPayroll, employees, employeeCount);
        }

      

        switch (choice[0]) {
            case '1': {
                // Prompt user to enter payroll file name
                cout << "\nFile Names (Jan25.txt, Feb25.txt, Mar25.txt, Apr25.txt)\n";
                cout << "\n📂 Enter payroll file name (or type 'quit' to exit): ";
                cin >> filename;

                // Check if the user wants to quit
                if (filename == "quit" || filename == "Quit") {
                    displayMenu(monthlyPayroll, employees, employeeCount);
                }

                // Assign month index based on file name and mark payroll as processed
                if (filename == "Jan25.txt") {
                    monthIndex = 0;
                    processed[0] = true;
                } else if (filename == "Feb25.txt") {
                    monthIndex = 1;
                    processed[1] = true;
                } else if (filename == "Mar25.txt") {
                    monthIndex = 2;
                    processed[2] = true;
                } else if (filename == "Apr25.txt") {
                    monthIndex = 3;
                    processed[3] = true;
                } else {
                    cout << "❌ Invalid file name. Please try again.\n";
                    continue;
                }

                
                readPayrollData(filename, monthIndex, employees, employeeCount, monthlyPayroll);
                displayPayroll(monthIndex, monthlyPayroll, employees, employeeCount);
                
               
                writePayrollToFile(filename + "_output.txt", monthIndex, monthlyPayroll, employeeCount);
                break;
            }

            case '2':
                // Search for a specific employee's payroll details
                searchPayroll(monthlyPayroll, employees, employeeCount);
                break;

            case '3':
                // Display payroll data for only the processed months
                for (int i = 0; i < 4; i++) {
                    if (processed[i]) {
                        displayPayroll(monthIndex, monthlyPayroll, employees, employeeCount);
                    }
                    
                }
                break;

            default:
                cout << "❌ Invalid choice. Please try again.\n";
        }
    }
}


// Converts a given string to uppercase
void toUpperCase(string &str) {
    for (char &c : str) {
        if (c >= 'a' && c <= 'z') c -= ('a' - 'A'); 
    }
}

// Reads employee data from a file and stores it in the employees array
void readEmployeeData(string filename, Employee employees[max_Employees], int &employeeCount) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open " << filename << endl; 
        return;
    }
    
    // Read employee data (ID, name, hourly rate) and store it in the array
    while (file >> employees[employeeCount].id >> employees[employeeCount].name >> employees[employeeCount].hourlyRate) {
        employeeCount++; 
    }
    
    file.close(); 
}

// Searches for an employee by their ID in the employee list
int findEmployeeIndex(string id, Employee employees[max_Employees], int employeeCount) {
    for (int i = 0; i < employeeCount; ++i) {
        if (employees[i].id == id) return i; 
    }
    return -1;
}


void readPayrollData(string filename, int monthIndex, Employee employees[max_Employees], int employeeCount, Payroll monthlyPayroll[12][max_Employees]) {
    ifstream file(filename);
    if (!file) { 
        cout << "❌ Error: Could not open " << filename << ". Please check the file and try again.\n";
        return;
    }

    ofstream errorLog("errors.txt", ios::app); 
    string empId;
    double hoursWorked;

    // Write the error header to the error log file if it's empty
    if (errorLog.tellp() == 0) { // Check if the file is empty
        errorLog << left << setw(50) << "Name of pay file where error occurred"
                 << "Error description" << endl;
    }
    
    // Read employee IDs and hours worked from the file
    while (file >> empId) {
        toUpperCase(empId);
        
        // Attempt to read hours worked; check for valid input
        if (!(file >> hoursWorked)) { // If hoursWorked is missing or invalid
            errorLog << left << setw(50) << filename  
                     << "Pay entry for " << empId << " is incomplete." << endl;
            file.clear(); // Clear error state to continue reading
            continue; 
        }

        // Find the index of the employee in the employees array
        int empIndex = findEmployeeIndex(empId, employees, employeeCount);
        if (empIndex == -1) { // If employee ID is not found
            errorLog << left << setw(50) << filename 
                     << empId  << " is not a valid employee ID number"  << endl;
            continue; 
        }

        // Check if the recorded hours are negative (invalid data)
        if (hoursWorked < 0) {
            errorLog << left << setw(50) << filename  
                     << "  Incomplete entry for " << empId << endl;
            continue; 
        }

        // Calculate payroll details
        double grossPay = hoursWorked * employees[empIndex].hourlyRate;
        double taxableIncome = max(no_Taxable_Income, (grossPay * months_In_Year) - taxable_Income_Threshold);
        double tax = (taxableIncome * tax_Rate) / months_In_Year; 
        double netPay = grossPay - tax; 
        
        // Store payroll data in the payroll array
        monthlyPayroll[monthIndex][empIndex] = {empId, hoursWorked, grossPay, tax, netPay};
    }

    file.close(); 
    errorLog.close(); 
}

// Writes an error log to a file (for invalid or missing employee data)

void writePayrollToFile(string filename, int monthIndex, Payroll monthlyPayroll[12][max_Employees], int employeeCount) {
    ofstream file(filename);
    ofstream errorLog("errors.txt", ios::app);

    // Check if the payroll file opened successfully
    if (!file) {
        cout << left << setw(50) << filename << "Error: Could not open payroll file." << endl;
        return; 
    }
    
    // Write the header for the payroll data
    file << left << setw(15) << "Employee_ID"
         << setw(30) << "Monthly Pay (Before)"
         << setw(30) << "Monthly Pay (After)" << endl;
    file << string(75, '-') << endl; 

    // Loop through the employees and write payroll data to the file
    for (int i = 0; i < employeeCount; ++i) {
        
        
        if (monthlyPayroll[monthIndex][i].id != "") { 
            file << left << setw(15) << monthlyPayroll[monthIndex][i].id 
                 << setw(30) << monthlyPayroll[monthIndex][i].grossPay 
                 << setw(30) << monthlyPayroll[monthIndex][i].netPay << endl;
        }
    }
    file.close(); 
}

// Function to display payroll data for a given month
void displayPayroll(int monthIndex, Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount) {
  
    string months[] = {"January", "February", "March", "April"};

    // Display the payroll data header
    cout << "\n📊 Payroll Data for Month: " << months[monthIndex] << "\n";
    cout << "Employee_ID          Name          Hours Worked   Gross Pay     Tax Deduction      Net Pay\n";
    cout << "------------------------------------------------------------------------------------------\n";

    // Loop through all employees to display their payroll data
    for (int i = 0; i < employeeCount; i++) {
        // Skip if the employee ID is empty
        if (monthlyPayroll[monthIndex][i].id.empty()) {
            continue; 
        }
        
        // Display employee payroll details
        cout << left << setw(20) << monthlyPayroll[monthIndex][i].id 
             << setw(15) << employees[findEmployeeIndex(monthlyPayroll[monthIndex][i].id, employees, employeeCount)].name 
             << setw(15) << fixed << setprecision(2) << monthlyPayroll[monthIndex][i].hoursWorked 
             << setw(15) << fixed << setprecision(2) << monthlyPayroll[monthIndex][i].grossPay 
             << setw(15) << fixed << setprecision(2) << monthlyPayroll[monthIndex][i].tax 
             << setw(15) << fixed << setprecision(2) << monthlyPayroll[monthIndex][i].netPay << endl; 
    }
}
// Function to search for an employee's payroll details
void searchPayroll(Payroll monthlyPayroll[12][max_Employees], Employee employees[max_Employees], int employeeCount) {
    string searchId; 
   
    
    cout << "Enter Employee ID to search (or type 'quit' to exit): ";
    cin >> searchId;

    // Check if the user wants to exit the search function
    if (searchId == "quit" || searchId == "Quit") {
        displayMenu(monthlyPayroll, employees, employeeCount);  
        return;
    }

    toUpperCase(searchId);
    
    bool idFound = false; // Flag to track if the ID exists in payroll records

    // Loop through payroll records of the first four months to find the employee ID
    for (int m = 0; m < 4; m++) {
        for (int i = 0; i < employeeCount; i++) {
            if (monthlyPayroll[m][i].id == searchId) {
                idFound = true; 
            }
        }
    }
    
    if (!idFound) {
        cout << "Error: Employee ID " << searchId << " not found in records.\n";
        return;
    }

    // Loop for selecting a month to display payroll details
    while (true) {  
        cout << "\n 📅 Select the month to view payroll:\n";
        cout << "1. January\n";
        cout << "2. February\n";
        cout << "3. March\n";
        cout << "4. April\n";
        cout << "5. All Months\n";
        cout << "Enter your choice (or type 'quit' to go back): ";

        string choiceInput; 
        cin >> choiceInput;  

        // Check if the user wants to go back to the menu
        if (choiceInput == "quit" || choiceInput == "Quit") {
            displayMenu(monthlyPayroll, employees, employeeCount); // Return to menu
            return; 
        }

        // Validate input choice (should be between 1 and 5)
        if (choiceInput.length() == 1 && choiceInput[0] >= '1' && choiceInput[0] <= '5') {
            int monthIndex = choiceInput[0] - '1'; 
            bool found = false; 
            
            // If option 5 is selected, display payroll details for all months
            if (monthIndex == 4) {  
                for (int m = 0; m < 4; m++) {
                    for (int i = 0; i < employeeCount; i++) { 
                        if (monthlyPayroll[m][i].id == searchId) { 
                            // Display payroll details for the current month
                            cout << "\n 📅 Month: " 
                                 << (m == 0 ? "January" : m == 1 ? "February" : m == 2 ? "March" : "April" ) << endl;
                            cout << "\nEmployee ID: " << monthlyPayroll[m][i].id << endl;
                            cout << "Name: " << employees[findEmployeeIndex(monthlyPayroll[m][i].id, employees, employeeCount)].name << endl;
                            cout << "Hours Worked: " << monthlyPayroll[m][i].hoursWorked << endl;
                            cout << "Gross Pay: " << monthlyPayroll[m][i].grossPay << endl;
                            cout << "Tax Deduction: " << monthlyPayroll[m][i].tax << endl;
                            cout << "Net Pay: " << monthlyPayroll[m][i].netPay << endl;
                            found = true; 
                        }
                    }
                }
                if (!found) {
                    cout << "Employee ID " << searchId << " not found for any month.\n";
                } break;
            } else {
                // Display payroll details for the selected month
                for (int i = 0; i < employeeCount; i++) { 
                    if (monthlyPayroll[monthIndex][i].id == searchId) { 
                        cout << "\n 📅 Month: " 
                             << (monthIndex == 0 ? "January" : monthIndex == 1 ? "February" : monthIndex == 2 ? "March" : "April" ) << endl;
                        cout << "\nEmployee ID: " << monthlyPayroll[monthIndex][i].id << endl; 
                        cout << "Name: " << employees[findEmployeeIndex(monthlyPayroll[monthIndex][i].id, employees, employeeCount)].name << endl;
                        cout << "Hours Worked: " << monthlyPayroll[monthIndex][i].hoursWorked << endl;
                        cout << "Gross Pay: " << monthlyPayroll[monthIndex][i].grossPay << endl;
                        cout << "Tax Deduction: " << monthlyPayroll[monthIndex][i].tax << endl;
                        cout << "Net Pay: " << monthlyPayroll[monthIndex][i].netPay << endl;
                        return; 
                    }
                }  
                // If employee ID is not found for the selected month, show an error message
                cout << "Employee ID " << searchId << " not found for the selected month.\n";
            }
        } else {
            // Invalid choice message
            cout << "Invalid choice. Try again.\n";
        }
    }
}

