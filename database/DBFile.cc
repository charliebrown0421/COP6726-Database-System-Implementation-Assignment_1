#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"


DBFile::DBFile () {
off_t currentPage=0;
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
    if (f_type== heap){
     currentPage = 0;
    dataFile.Open(0,f_path);
    return 1;
    }
    else return 0;
}


void DBFile::Load (Schema &f_schema, char *loadpath) {
     FILE * filetoload = fopen(loadpath,"r");
	if(!filetoload){
		cerr << "Unable to open file: " << loadpath <<endl;
		exit(1);
	}
    //For testing
    /*
    if(dataRecord.SuckNextRecord(&f_schema,filetoload)){
    dataRecord.Print(&f_schema);
    }*/
    // Load Records from the dbfile
    int  count_records=0;
	while(dataRecord.SuckNextRecord(&f_schema, filetoload)){
		count_records++;
		if(!dataPage.Append(&dataRecord)){
			dataFile.AddPage(&dataPage,currentPage++);
			dataPage.EmptyItOut();
			dataPage.Append(&dataRecord);
		}
	}
	//Add the last page into the file;
	dataFile.AddPage(&dataPage,currentPage++);
	//For testing
	cout<<" The total records have been loaded are "<<count_records<<endl;
	fclose(filetoload);
	return;
}

int DBFile::Open (char *f_path) {
    dataFile.Open(1,f_path);
	currentPage = 0;
	cout << "total pages in File: " << dataFile.GetLength() << ";" <<endl;
	return 1;
}

void DBFile::MoveFirst () {
//For safety
 dataPage.EmptyItOut();
 //move to the first one
 currentPage = 0;
}

int DBFile::Close () {
    //For safety
   dataPage.EmptyItOut();
  cout << " The number of total pages in File are: " << dataFile.GetLength() << ";" <<endl;
  //Reset
  currentPage = 0;
  return dataFile.Close();
}

void DBFile::Add (Record &rec) {
    if(!dataPage.Append(&rec)){
    dataPage.EmptyItOut();
    dataPage.Append(&rec);
    }
    return;
}
//Version 1
/*int DBFile::GetNext (Record &fetchme) {
    int hasRecord=dataPage.GetFirst(&fetchme);
    if (hasRecord==0&&currentPage+1>=dataFile.GetLength())
    return 0;
    else if (hasRecord==0)  {
    dataFile.GetPage(&dataPage,currentPage++);
    dataPage.GetFirst(&fetchme);
    }
    return 1;
}*/
//Version 2
/*int DBFile::GetNext (Record &fetchme) {
    int hasRecord=dataPage.GetFirst(&fetchme);
    if(hasRecord==0){
    if (hasRecord==0&&currentPage+1>=dataFile.GetLength())
    return 0;
    dataFile.GetPage(&dataPage,currentPage++);
    dataPage.GetFirst(&fetchme);
    }
    return 1;
    }*/

//Version 3 Fastest one
int DBFile::GetNext (Record &fetchme) {
   if(!dataPage.GetFirst(&fetchme)){
    if (currentPage+1>=dataFile.GetLength())
       return 0;
    dataFile.GetPage(&dataPage,currentPage++);
    dataPage.GetFirst(&fetchme);
    }
    return 1;
    }

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

    int find_flag = 0;
	ComparisonEngine comp;
	while(GetNext(fetchme)){
	    if (comp.Compare (&fetchme, &literal, &cnf)){
		//fetchme.Print(&mySchema);
        	find_flag = 1;
        	break;
	    }
	}

	if(find_flag == 0){
	    //no records found
	    return 0;
	}
	return 1;
}
