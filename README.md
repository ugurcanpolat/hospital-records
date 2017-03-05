--- Program Work Flow ---

First, read the records from the file named “database.txt”.

In the database file, each row corresponds to a different record. 
The parameters of each record are separated by a single tab character (“\t”). The columns correspond to the following data:

1) name: the patient’s name
2) doctorName: the doctor’s name
3) diagnosis: the diagnosed disease
4) patientNumber: the patient’s ID
5) polyclinicNumber: the polyclinic’s ID
6) branchName: the name of the hospital branch

Write the records into an array of patient_record structs. While doing so, the records will be ordered by branchName. 
Records with the same branchName must be ordered by polyclinicNumber.


Commands:
P: Prints all the patient records registered in the hospital.
B: Searches patients by branch. (In this command, the program should ask for the
branch name, then print all matching records.)
C: Searches patients by polyclinic. (In this command, the program should ask for the
polyclinic number, then print all matching records.)
I: Inserts the record as entered by the user. (In this command, the program should
ask for each parameter of the record one by one.)
R: Removes the record specified by the user. (In this command, the program should
ask for the patient number, then delete the corresponding record.) E: Exits the program.

Before your program terminates, “database.txt” will be overwritten with the most recent versions of the records.

-- Notes ---

a. Each branch can have at most 2 polyclinics.
b. Each polyclinic is used for only one branch.
c. No more than 10 patients can get examined in one polyclinic.
