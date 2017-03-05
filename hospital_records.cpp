/* @Author
 * Student Name: Ugurcan Polat
 * Student ID : 150140117
 * Date: 18.10.2016
 */

#include "functions.cpp"

struct Functions operations;

void menu(void);

int main(void) {
	char selection;
	
	// Opens the input file.
	operations.open_database();
	// Initially allocate record with 10 record (1 polyclinic).
	operations.record = new struct patient_record[10];
	// Reads from the input file and writes records into array of structs.
	operations.write_into_structs();
	
	do {
		menu();
		cout << "Your selection is: ";
		cin >> selection;
		switch(selection) {
			case 'P': operations.print_records(); break;
			case 'B': operations.search_by_branch(); break;
			case 'C': operations.search_by_polyclinic(); break;
			case 'I': operations.insert_record(); break;
			case 'R': operations.remove_record(); break;
			case 'E': cout << endl << "Goodbye!"; break;
			default: cout << endl << "Wrong input!" << endl; break;		
		}
	
	} while(selection != 'E');
	
	// Writes latest records into the database file, and closes it.
	operations.close_database();
	
	// Destroy record.
	delete [] operations.record;
	
	return 0;
}

void menu(void) {
	cout << "\nPlease select the operation to perform and enter the ";
	cout << "operation code" << endl;
	cout << "(P) Print all of the patient records," << endl;
	cout << "(B) Search the data by the branch name," << endl;
	cout << "(C) Search the data by the polyclinic number," << endl;
	cout << "(I) Insert a new patient record," << endl;
	cout << "(R) Remove the patient record," << endl;
	cout << "(E) Exit the program," << endl;
}
