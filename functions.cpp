/* @Author
 * Student Name: Ugurcan Polat
 */
 
#include "functions.h"

using namespace std;

void Functions::open_database() { 	
	// Opens the databasee file. See the close_database() function to
	// see why the file will be opened with "r" privilege.
	fp = fopen("database.txt", "r");
	
	// If it is unable to open the file give error.
	if(!fp) {
		cout << "Cannot open the database file" << endl;
		exit(0);
	}
	
	// Set the position indicator to the beginning of file.
	rewind(fp);
}

void Functions::write_into_structs(){	
	struct patient_record buffer;
	char line[100];
	int num_patients;
	
	// Calculates the number of patients in the file.
	for (num_patients = 0; !feof(fp); num_patients++) {
		// If the file has a empty line at the end of the file, break
		// the loop without counting that line.
		if(fgets(line, 255, fp) == NULL)
			break;
	}
	
	// Sets the position indicator to the beginning of the file.
	rewind(fp);
	
	// Initialize number of polyclinics.
	num_polyclinics = 0;
	
	// Initialize array of polyclinics to 0.
	for (int count = 0; count < 100; count++)
		polyclinics[count] = 0;
	
	for(int count = 0; count < num_patients; count++) {	
		// Reads the line. It reads informations until it sees the 
		// tab character '\t' and stores information in corresponding
		// variables in struct buffer. 							    
		fscanf(fp, "%[^\t]\t%[^\t]\t%[^\t]\t%d\t%d\t%s", buffer.name,
														 buffer.doctorName,
														 buffer.diagnosis,
														 &buffer.patientNumber,
														 &buffer.polyclinicNumber,
														 buffer.branchName);
		
		// fscanf reads carriage return '\r' and '\n' characters with
		// the information for name. Eliminate these characters from 
		// name, because there are unnecessary.	It does this operation
		// by shifting other characters to left and making null the 
		// shifted characters.					
		if (buffer.name[0] == '\r' && buffer.name[1] == '\n') {
			int check = strlen(buffer.name);
			for (int j = 0; j < (check - 1); j++) {
				buffer.name[j] = buffer.name[j+2];
				buffer.name[j+2] = '\0';
			}
		}
		
		// Call insertion_sort function to write the read informations
		// into the array of structs.
		insertion_sort(buffer);
	} 
}

int Functions::insertion_sort(struct patient_record buffer) {
	int is_new_polyclinic = 1;
	int i, j = 0, position;
	char null_string[100] = {'\0'};
	
	// Checks if the polyclinic of buffer already exist in the 
	// records. If it exist sets the flag to 0. 
	for (i = 0; i < num_polyclinics; i++) {
		if (buffer.polyclinicNumber == polyclinics[i]) {
			is_new_polyclinic = 0;
			break;
		}
	}
	
	// This loop compares the branch names to find the insertion index.
	// Each polyclinic has exactly 10 elements, hence the loop can
	// can jump 10 by 10 to speed up comparison.  
	for(i = 0; i < (num_polyclinics * 10); i += 10) {
		// If the buffer's branch name comes before from the current 
		// record's branch name the loop should stop there. 'i' will be
		// the position for insertion. 
		if (strcmp(buffer.branchName, (record+i)->branchName) < 0) 
			break;	
		
		if (strcmp(buffer.branchName, (record+i)->branchName) == 0) {
			// This loop finds the end index of the branch.
			for (j = i; j < (num_polyclinics * 10); j += 10) {
				// Find the end index of the branch by comparing buffer's
				// and following records branch names. When they does not
				// match j+10 should be the end index.
				if (strcmp(buffer.branchName, (record+j+10)->branchName) != 0) {
					j += 10;
					break;	
				}
			}
			break;
		} 		
	}
	
	// If buffer's branch already exist in records find the position
	// in the branch for insertion.
	if (strcmp(buffer.branchName, (record+i)->branchName) == 0) {
		// Find position between begin('i') and end('j') idexes of the branch.
		for (; i < j; i++) {
			// If buffer's polyclinic number already exists in records
			// check if the following record's polyclinic is same or not. 
			// The reason of doing that is to find the position for 
			// insertion in the polyclinic which should come after the
			// last record of the polyclinic. (If polyclinic has less
			// than 10 patients some of the elements would be null. 
			// Hence while comparin the buffer and the next position 
			// polyclinicNumbers should not match, and it shoul break
			// the loop.).
			if (buffer.polyclinicNumber == (record+i)->polyclinicNumber) {
				if (buffer.polyclinicNumber != (record+i+1)->polyclinicNumber) {
					i++;
					break;
				}
			}
			
			// If buffer's polyclinic number comes before the current
			// position break the loop at that position.
			else if (buffer.polyclinicNumber < (record+i)->polyclinicNumber) 
				break;
		}
	}
	
	// If buffer's polyclinic is already exist in records check if 
	// the polyclinic is full or not. If the polyclinic is full 
	// (10 patients) give error and return without inserting.
	// It checks this by looking the position of insertion is multiples
	// of 10 or not (if the position is multiples of 10, it is the first 
	// record of a polyclinic. Because each polyclinic has exactly 10
	// elements.).
	if (!is_new_polyclinic) {
		if (i % 10 == 0) {
			cout << endl << "No more than 10 patients can get "; 
			cout << "examined in one polyclinic!" << endl;
			return -1;
		}
	}
	
	// Shifting records is only necessary when the insert record's 
	// polyclinic does not exist in records. To open space for the 
	// new polyclinic other records should be shifted forward by 10.
	if (is_new_polyclinic) {
		// Struct must be realocated to handle 10 new polyclinic records.
		reallocate_struct();
		
		// Beginning from the last record shift forward records by 10.
		// Loop does its job until it reaches to the position where 
		// insertion will happen. To clarify: if there is 3 polyclinics,
		// position of the last record will be (num_polyclinics*10)-1=29.
		// Where it will be shifted is ((num_polyclinic+1)*10)-1= 39.
		for(position = ((num_polyclinics+1)*10)-1; position >= i+10; position--) {
			strcpy((record+position)->name, (record+position-10)->name);
			strcpy((record+position)->doctorName, (record+position-10)->doctorName);
			strcpy((record+position)->diagnosis, (record+position-10)->diagnosis);
			(record+position)->patientNumber = (record+position-10)->patientNumber;
			(record+position)->polyclinicNumber = (record+position-10)->polyclinicNumber;
			strcpy((record+position)->branchName, (record+position-10)->branchName);
		}	
		
		// Set the places of new polyclinic to null to make sure 
		// there is no leftover information after shifting records.
		for(position = i; position < i+10; position++) {
			strcpy((record+position)->name, null_string);
			strcpy((record+position)->doctorName, null_string);
			strcpy((record+position)->diagnosis, null_string);
			(record+position)->patientNumber = 0;
			(record+position)->polyclinicNumber = 0;
			strcpy((record+position)->branchName, null_string); 
		}	
	}
	
	// Insert the record to the position 'i'.
	strcpy((record+i)->name, buffer.name);
	strcpy((record+i)->doctorName, buffer.doctorName);
	strcpy((record+i)->diagnosis, buffer.diagnosis);
	(record+i)->patientNumber = buffer.patientNumber;
	(record+i)->polyclinicNumber = buffer.polyclinicNumber;
	strcpy((record+i)->branchName, buffer.branchName);	
	
	// If buffer's polyclinic does not exist in records write it 
	// into the array of polyclinics which is necessary to check
	// other records while insertion. Increment number of polyclinics.
	if (is_new_polyclinic) { 
		polyclinics[num_polyclinics] = buffer.polyclinicNumber;
		num_polyclinics++;
	}	
	
	return 0;
}

void Functions::reallocate_struct() {
	// Define temp struct to store records temporarily.
	struct patient_record *temp;
	temp = new struct patient_record[num_polyclinics*10];
	
	// Copy informations from records to temp struct.
	for (int count = 0; count < num_polyclinics*10; count++) {
		strcpy((temp+count)->name, (record+count)->name);
		strcpy((temp+count)->doctorName, (record+count)->doctorName);
		strcpy((temp+count)->diagnosis, (record+count)->diagnosis);
		(temp+count)->patientNumber = (record+count)->patientNumber;
		(temp+count)->polyclinicNumber = (record+count)->polyclinicNumber;		
		strcpy((temp+count)->branchName, (record+count)->branchName);
	}
	
	// Destroy and construct record struct with its new memory size.
	delete [] record;
	record = new struct patient_record[(num_polyclinics+1)*10];
	
	// Copy informations from temp to records.
	for (int count = 0; count < num_polyclinics*10; count++) {
		strcpy((record+count)->name, (temp+count)->name);
		strcpy((record+count)->doctorName, (temp+count)->doctorName);
		strcpy((record+count)->diagnosis, (temp+count)->diagnosis);
		(record+count)->patientNumber = (temp+count)->patientNumber;
		(record+count)->polyclinicNumber = (temp+count)->polyclinicNumber;		
		strcpy((record+count)->branchName, (temp+count)->branchName);
	}
	
	// Destroy temp.
	delete [] temp;
}

void Functions::print_records() {		
	// Prints header.
	cout << endl;			
	cout << setw(20) << left << "Name" << " | ";
	cout << setw(20) << left << "Doctor Name" << " | ";
	cout << setw(25) << left << "Diagnosis" << " | ";
	cout << setw(6) << left << "PatNum" << " | ";
	cout << setw(6) << left << "PolNum" << " | ";
	cout << setw(20) << left << "Branch" << endl;
	
	for (int count = 0; count < 100; count++) 
		cout << "-";
		
	cout << endl;

	// Prints all the records with information. It skips empty records.		
	for (int count = 0; count < num_polyclinics*10; count++){	
		if ((record+count)->patientNumber != 0) {
			cout << setw(20) << left << (record+count)->name << " | ";
			cout << setw(20) << left << (record+count)->doctorName << " | ";
			cout << setw(25) << left << (record+count)->diagnosis << " | ";
			cout << setw(6) << right << (record+count)->patientNumber << " | ";
			cout << setw(6) << right << (record+count)->polyclinicNumber << " | ";
			cout << setw(20) << left << (record+count)->branchName << endl; 
		}
    } 
    cout << endl;
}

void Functions::search_by_branch() {
	char search[20];
	int found_flag = 0;
	cout << endl << "Please enter branch name to search: ";
	cin >> search;
	cout << endl;
	
	// Prints header.		
	cout << setw(20) << left << "Name" << " | ";
	cout << setw(20) << left << "Doctor Name" << " | ";
	cout << setw(25) << left << "Diagnosis" << " | ";
	cout << setw(6) << left << "PatNum" << " | ";
	cout << setw(6) << left << "PolNum" << " | ";
	cout << setw(20) << left << "Branch" << endl;
	
	for (int count = 0; count < 100; count++) 
		cout << "-";
		
	cout << endl;
	
	// Checks all records.
	for (int i = 0; i < num_polyclinics*10; i++) {
		if (strcmp(search, (record+i)->branchName) == 0) {
			// Set found_flag to 1, it will be used to check if any
			// record has been found or not.			
			found_flag = 1;
			// Prints the found record.
			cout << setw(20) << left << (record+i)->name << " | ";
			cout << setw(20) << left << (record+i)->doctorName << " | ";
			cout << setw(25) << left << (record+i)->diagnosis << " | ";
			cout << setw(6) << right << (record+i)->patientNumber << " | ";
			cout << setw(6) << right << (record+i)->polyclinicNumber << " | ";
			cout << setw(20) << left << (record+i)->branchName << endl; 
		}
	}
	
	// If there is no record with the entered branch name print
	// the result.	
	if (!found_flag)
		cout << "No record has been found in \"" << search << "\" branch!";
	
	cout << endl;
}

void Functions::search_by_polyclinic() {
	int search, found_flag = 0;
	cout << endl << "Please enter polyclinc number to search: ";
	cin >> search;
	cout << endl;
	
	// Prints header.		
	cout << setw(20) << left << "Name" << " | ";
	cout << setw(20) << left << "Doctor Name" << " | ";
	cout << setw(25) << left << "Diagnosis" << " | ";
	cout << setw(6) << left << "PatNum" << " | ";
	cout << setw(6) << left << "PolNum" << " | ";
	cout << setw(20) << left << "Branch" << endl;

	for (int count = 0; count < 100; count++) 
		cout << "-";
		
	cout << endl;
	
	// Checks all records.
	for (int i = 0; i < num_polyclinics*10; i++) {
		if ((record+i)->polyclinicNumber == search) {
			// Set found_flag to 1, it will be used to check if any
			// record has been found or not.
			found_flag = 1;
			// Prints the found record.
			cout << setw(20) << left << (record+i)->name << " | ";
			cout << setw(20) << left << (record+i)->doctorName << " | ";
			cout << setw(25) << left << (record+i)->diagnosis << " | ";
			cout << setw(6) << right << (record+i)->patientNumber << " | ";
			cout << setw(6) << right << (record+i)->polyclinicNumber << " | ";
			cout << setw(20) << left << (record+i)->branchName << endl; 
		}
	}
	
	// If there is no record with the entered polyclinic number print
	// the result.
	if (!found_flag) {
		cout << "No record has been found with polyclinic number ";
		cout << "'"<< search << "'" << endl;
	}
	
	cout << endl;	
}

void Functions::insert_record() {
	struct patient_record insert;
	int polyclinics_in_branch = 0, temp_polyclinic = 0, is_new_poly = 1;
	
	// Gets information of the record that will be inserted. It asks
	// information of 'string' with scanf to read until user 'enters'.
	// Before that getchar function removes '\n' character from input
	// buffer.
	cout << "\nEnter name: ";
	getchar();
	scanf("%[^\n]s", insert.name);
	cout << "Enter name of doctor: ";
	getchar();
	scanf("%[^\n]s", insert.doctorName);
	cout << "Enter diagnosis: ";
	getchar();
	scanf("%[^\n]s", insert.diagnosis);
	cout << "Enter patient number: ";
	cin >> insert.patientNumber;
	
	// If the entered patient number is not a 4-digit integer,
	// gives error and returns without inserting.
	if (insert.patientNumber < 1000 || insert.patientNumber > 9999) {
		cout << "\nPatient number must an integer with 4 digits!\n";
		return;
	}
	
	cout << "Enter polyclinic number: ";
	cin >> insert.polyclinicNumber;
	
	cout << "Enter branch name: ";
	getchar();
	scanf("%[^\n]s", insert.branchName);
	
	for (int i = 0; i < num_polyclinics*10; i += 10) {
		// Checks if the entered branch is already in records.
		if (strcmp((record+i)->branchName, insert.branchName) == 0) {
			// Checks if the current record's polyclinic number 
			// does not match with the entered one.
			if ((record+i)->polyclinicNumber != insert.polyclinicNumber) {
				// If the current record's polyclinic number is different
				// from the backed up one, increment number of polyclinics
				// in the branch. This variable will be used to check
				// If user is trying to insert more than 2 polyclinics
				// in one branch. 
				if ((record+i)->polyclinicNumber != temp_polyclinic)
					polyclinics_in_branch++;
				
				// Backup the polyclinicNumber to compare it next time.	
				temp_polyclinic = (record+i)->polyclinicNumber;
			}
			
			// If the entered polyclinicNumver is already exist
			// Set the flag to zero and break the loop.
			else {
				is_new_poly = 0;
				break;
			}
		}
	}
	
	// If the entered polyclinic is a new one and entered branchName
	// already has 2 polyclinics, gives error and returns without inserting.
	if (is_new_poly && polyclinics_in_branch == 2) {
		cout << "\nEach branch can have at most 2 polyclinics!\n";
		return;
	}
	
	// Checks if entered polyclinicNumber and branchName matches the 
	// ones in the record's array of structs. If entered 
	// polyclinicNumber is found in the array, and its branchName does 
	// not match with the one in array, gives error and returns 
	// without inserting.
	for (int i = 0; i < num_polyclinics*10; i += 10) {
		if ((record+i)->polyclinicNumber == insert.polyclinicNumber) {
			if (strcmp((record+i)->branchName, insert.branchName) != 0) {
				cout << "\nEach polyclinic can be used for only one branch!\n";
				return;
			}
		}
	} 
	
	// Calls insertion_sort function to insert the record and stores
	// insertion status.
	int status = insertion_sort(insert);
	
	if (status != -1)
		cout << endl << "Record has been inserted" << endl;
}

void Functions::remove_record() {
	int remove, is_found = 0, is_only_pol = 1;
	int i, j, d, copy_i;
	char null_string[100] = {'\0'};
	
	
	// Asks for the patient number of the record that will be removed.
	cout << "\nEnter patient number of the record to be removed: ";
	cin >> remove;
	
	// If entered patient number is not a 4-digit integer, give error
	// and return without doing any operations.
	if (remove < 1000 || remove > 9999) {
		cout << endl << "The patient number must be an integer ";
		cout << "with 4 digits!" << endl;
		return;
	}
	
	// Searches for the entered patientNumber. If it is found,
	// is_found flag must be 1 and 'i' must be the position of 
	// the record. 
	for (i = 0; i < num_polyclinics*10; i++) {
		if ((record+i)->patientNumber == remove) {
			is_found = 1; 
			break;
		}
	}
	
	// If there is no record with the entered patient number, 
	// give error and return without doing any operations.
	if (!is_found) {
		cout << endl << "There is no record with patient number '";
		cout << remove << "'!" << endl;
		return; 
	}
	
	// Search for polyclinicNumber of the record that will be removed to
	// check if it is the only record in its polyclinic or not.
	for (j = 0; j < num_polyclinics*10; j++) {
		if ((record+j)->polyclinicNumber == (record+i)->polyclinicNumber
															&& j != i) {
			is_only_pol = 0;
			break;
		}
	}
	
	// If the record that will be removed is the only record in its 
	// polyclinic, following loop must end when it reaches to last 
	// record of array of structs. And it should shift the following 
	// polyclinic's record (the distance between it and following
	// polyclinic is 10.).
	if (is_only_pol) {
		j = num_polyclinics*10;
		d = 10;
	}
	
	// If it is not the only record in its polyclinic, then loop must
	// end when it reaches to end index of its polyclinic. To determine
	// the end index; divide its position by 10 and increment it by 1,
	// then multiply it by 10 (For the position 22: 
	// j = (22 / 10) + 1 = 3 (integer). j = 3 * 10 = 30. 
	// Hence the record is not the only record in its polyclinic, 
	// the distance between it and following record is 1.
	else {
		j = (i / 10) + 1;
		j *= 10;
		j--;
		d = 1;
	}
	
	// Backup polyclinicNumber of the record that will be removed to 
	// use it to remove its polclinicNumber from array of polyclinics.  
	copy_i = (record+i)->polyclinicNumber;
	for (; i < j; i++) {
		// Shifts record to remove record by overwriting.
		strcpy((record+i)->name, (record+i+d)->name);
		strcpy((record+i)->doctorName, (record+i+d)->doctorName);
		strcpy((record+i)->diagnosis, (record+i+d)->diagnosis);
		(record+i)->patientNumber = (record+i+d)->patientNumber;
		(record+i)->polyclinicNumber = (record+i+d)->polyclinicNumber;
		strcpy((record+i)->branchName, (record+i+d)->branchName);
		
		// Removes the shifted record.
		strcpy((record+i+d)->name, null_string);
		strcpy((record+i+d)->doctorName, null_string);
		strcpy((record+i+d)->diagnosis, null_string);
		(record+i+d)->patientNumber = 0;
		(record+i+d)->polyclinicNumber = 0;
		strcpy((record+i+d)->branchName, null_string);
		
	}
	
	// If the removed record is only record in its polyclinic, 
	// remove that polyclinic from array of polyclinics, too.
	if (is_only_pol) {
		for (i = 0; i < num_polyclinics; i++) {
			if (polyclinics[i] == copy_i) {
				polyclinics[i] = 0;
				num_polyclinics--;
			}
		}
	}
	 
	cout << endl << "Record with patient number '" << remove;
	cout << "' has been removed!" << endl;
}

void Functions::close_database() {
	// To overwrite the file close the file which is opened with 
	// "r" privilege, then open it again with "w" privilege.
	// This will allow program to overwrite existing informations.
	// Opening the file with "r+" privilege causes some bugs such as:
	// if user removes a patient, the number of patients will decrease
	// by 1. The original file has "number of patients" records, and  
	// the latest records has "number of patients-1" records. This will 
	// cause the last line of the original file to remain.
	fclose(fp);
	fopen("database.txt", "w");
	
	// Writes into the file
	for (int count = 0; count < num_polyclinics*10; count++) {
		// Do not write empty records (by looking at patientNumber,
		// because patientNumber must be a 4-digit integer.
		if ((record+count)->patientNumber != 0) {
			fprintf(fp, "%s\t%s\t%s\t%d\t%d\t%s\r\n",(record+count)->name,
													 (record+count)->doctorName,
													 (record+count)->diagnosis,
													 (record+count)->patientNumber,
													 (record+count)->polyclinicNumber,
													 (record+count)->branchName);
		}
	}
	
	// Close the file
	fclose(fp);
}
