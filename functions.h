/* @Author
 * Student Name: Ugurcan Polat
 * Student ID : 150140117
 * Date: 18.10.2016
 */

#ifndef functions_h
#define functions_h

#include <iomanip>  // setw
#include <iostream> // cin, cout
#include <stdio.h>  // fopen, fclose, feof, fprintf, scanf, fscanf, getchar 
#include <string.h> // strcpy, strcmp, strlen

struct patient_record {
	char name[100], doctorName[100], diagnosis[100]; 
	int  patientNumber, polyclinicNumber;
	char branchName[100];
};

struct Functions {                        
	FILE *fp;
	struct patient_record *record;
	int  num_polyclinics;
	int  polyclinics[100];
	void open_database();
	void write_into_structs();
	int  insertion_sort(struct patient_record buffer);
	void reallocate_struct();
	void print_records();
	void search_by_branch();
	void search_by_polyclinic();
	void insert_record();
	void remove_record();
	void close_database();
};

#endif 
