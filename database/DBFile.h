#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h

class DBFile {

public:
	DBFile ();

	int Create (char *fpath, fType file_type, void *startup);
	int Open (char *fpath);
	int Close ();

	void Load (Schema &myschema, char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

	private:
	//The Binary File
	File dataFile;
	//Buffer
	Page dataPage;
	//Records in database
	Record dataRecord;
    //current page in reading
	off_t currentPage; 		//start from 0, 1st page's index is 0

};
#endif
