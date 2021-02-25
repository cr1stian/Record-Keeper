#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

#define p(x) std::cout << "\n"<< #x << " = " << x << std::endl; // For debugging, prints variable name & value

const int MAX_RECORDS = 100;

enum status{
	Active, // 0
	Discontinued, // 1
	Recall // 2
};

class Record{
private:
	std::string id;
	std::string name;
	int quantity;
	double price;
	status curStatus;
public:
	Record(); // Default Constructor
	Record(std::string id, std::string name, int quantity, double price); // Multiple parameter constructor
	std::string RecordStr(); // Returns Record as string
	void SetRecord(std::string id, std::string name, int quantity, double price);
	void SetId(std::string id);  // Sets given value as Record's ID
	void SetName(std::string name); // Sets given value as Record' Name
	void SetQuantity(int quantity); // Set givn value as Record's Quantity
	void SetPrice(double price);   // Sets given value as Record's Price
	double GetPrice(); // Returns Record's Price
	int GetQuantity(); // Returns Record's Quantity
	std::string GetId(); // Returns Record's ID
	std::string GetName();// Returns Record's Name

	~Record(){ std::cout<<"\nDeleted: "+this->RecordStr(); }; // Destructor
};


void Menu(int& selection); // Displays Menu
void GetData(Record* recordBookPtr[], Record recordBook[], std::string fileName, int& curCount); // Grabs data from file and store to database
void WriteError(std::string input); // Writes to ErrorFile
bool IsValidRec(Record& curRecord); // Checks if Record is Valid
void DisplayArray(Record recordBook[], int count); // Display's Record's Array
int SearchRecords(Record recordBook[], int count, std::string searchKey);
std::string CapitalizeStr(std::string lowerCase); // Capitalizes string

//
/* MAIN PROGRAM HURRRR */
//
int main(){

	int recordCount = 0;
	std::string fileName = "data.txt";

	Record* recordBookPtr[MAX_RECORDS];
	Record recordBook[MAX_RECORDS];



	GetData(recordBookPtr, recordBook, fileName, recordCount);
	std::cout<<"\nIn Main: ";
	DisplayArray(recordBook, recordCount);
	int result = SearchRecords(recordBook, recordCount, "WAX");
	p(recordBook[result].RecordStr());


	return 0;
}

void Menu(int& selection){
	/*
• print all items in the inventory unsorted
• print invalid records from an error file
• you should allow a user to sort by any field: item ID, item name (one word), quantity on hand, or price. 
• print report - Number of unique item + the total worth all inventory (Qty * price of everytype)
*/



//switch( selection ){

//}
}


// Impelement this to write error message along with Record 

// Writes to Error File
void WriteError(std::string errorStr){
	std::ofstream outFile;

	outFile.open("errorFile.txt", std::ios::out|std::ios::app);

	outFile<<errorStr;

	outFile.close();
}




// ** Get Data **
void GetData(Record* recordBookPtr[], Record recordBook[], std::string fileName, int& curCount){
	bool goodFile = false;


	std::ifstream inFile;
	std::ofstream outFile;

	inFile.open(fileName, std::ios::in);
	outFile.open("errorFile.txt", std::ios::out|std::ios::app);


	std::cout<<"\n"+fileName;

	if( !inFile ){ // Tries to open file
		std::cout<<"\nError: Could Not Open Input File";
		exit(0);
	}
	if( inFile.peek()==EOF ){// Checking if file is empty
		std::cout<<"\nError: Input File is Empty";
		exit(0);
	}


	std::string id;
	std::string name;
	int quantity;
	double price;


	while( !inFile.eof()&&curCount<MAX_RECORDS ){
		inFile>>id>>name>>quantity>>price;

		Record curRecord = Record(id, name, quantity, price);
		// Build the record here then check for validation
		// We shouldn't test for a bad record without first building the record itself
		if( IsValidRec(curRecord) ){
			// Debugging here
			std::cout<<"\nTo Array: "<<id<<" "<<name<<" "<<quantity<<" "<<price;

			recordBook[curCount] = curRecord; // Assigning Value to Dynamically Created object
			recordBookPtr[curCount] = &recordBook[curCount]; // Assign Address of object to pointer 
			curCount++;
		} else{
			// Debugging here
			std::cout<<"\nErrorFile: "<<id<<" "<<name<<" "<<quantity<<" "<<price;

			outFile<<id<<" "<<name<<" "<<quantity<<" "<<price<<"\n";
		}
	};

	if( (curCount+1)==MAX_RECORDS ){
		std::cout<<"\nProgram Storage Full. Using values grabbed. . . "<<std::endl;
	}

	outFile.close();
	inFile.close();
}



// Check Record for Validity, Writing to ErrorFile when not true
bool IsValidRec(Record& curRecord){
	bool valid = true;
	std::string errStr;

	if( curRecord.GetPrice()<=0.0 ){
		errStr += "Invalid Price - ID: "+curRecord.GetId()+"\n";
		valid = false;
	}

	if( curRecord.GetQuantity()<=0 ){
		errStr += "Invalid Quantity - ID: "+curRecord.GetId()+"\n";
		valid = false;
	}

	for( unsigned int pos = 0; pos<curRecord.GetName().length(); pos++ ){
		if( !(curRecord.GetName()[pos]>='A'&&curRecord.GetName()[pos]<='Z') ){
			errStr += "Invalid Character In Name - ID: "+curRecord.GetId()+"\n";
			valid = false;
		}
	}

	if( !valid ){
		WriteError(errStr);
	}
	return valid;
}

// Record Methods

Record::Record(){ // Record Default Constructor
	this->id = "";
	this->name = "";
	this->quantity = 0;
	this->price = 0.0;
	this->curStatus = Active;
}

// Multiple Parameter Constructor
Record::Record(std::string newId, std::string newName, int newQuantity, double newPrice){
	this->SetRecord(newId, newName, newQuantity, newPrice);
	this->curStatus = Active;
}


// Pre: Object must exist first
// Pos: Display the record
std::string Record::RecordStr(){
	return (this->id+" "+this->name+" "+std::to_string(this->quantity)+" "+std::to_string(this->price));
}


// Pre: Object must exist first
// Pos: Retrieves the Quantity
int Record::GetQuantity(){
	return this->quantity;
}


// Pre: Object must exist first
// Pos: Retrieves the Price
double Record::GetPrice(){
	return this->price;
}


// Pre: Object must exist first
// Pos: Retrieves the Name
std::string Record::GetName(){
	return this->name;
}


// Pre: Object must exist first
// Pos: Retrieves the Name
std::string Record::GetId(){
	return this->id;
}

// Pre: Must have Valid inputs
// Pos: Set input values as Record
void Record::SetRecord(std::string id, std::string name, int quantity, double price){

	this->SetId(id);
	this->SetName(name);
	this->SetQuantity(quantity);
	this->SetPrice(price);
}

// Pre: Object Must be Allocated
// Pos: Sets input value as ID
void Record::SetId(std::string newId){
	int idLength = newId.length();
	std::string checkedStr = "";

	for( int pos = 0; pos<idLength; pos++ ){
		if( (newId[pos]>='A'&&newId[pos]<='Z')||(newId[pos]>='a'&&newId[pos]<='z')||(newId[pos]>='0'&&newId[pos]<='9') ){
			checkedStr += newId[pos];
		} else{
			checkedStr += newId[pos];
		}
	}
	this->id = checkedStr;
}


// Pre: Object Must be Allocated
// Pos: Sets input value as Name
void Record::SetName(std::string name){
	std::string checkedName = "";
	int nameLength = name.length();

	for( int pos = 0; pos<nameLength; pos++ ){
		if( (name[pos]>='A'&&name[pos]<='Z')||name[pos]==' ' ){
			checkedName += name[pos];
		} else if( name[pos]>='a'&&name[pos]<='z' ){
			checkedName += (name[pos]-32);
		} else{
			checkedName += name[pos];
		}
	}
	this->name = checkedName;
}


// Pre: Object Must be Allocated
// Pos: Sets input value as Quantity
void Record::SetQuantity(int newQuantity){
	this->quantity = newQuantity;
}


// Pre: Object Must be Allocated
// Pos: Sets input value as Price
void Record::SetPrice(double newPrice){
	this->price = newPrice;
}

// Function to display all Records in RecordArray
void DisplayArray(Record recordBook[], int count){
	std::cout<<"\nOutside Loop: ";
	for( int i = 0; i<count; i++ ){
		std::cout<<"\nHere: "<<count<<recordBook[i].RecordStr();
	}
}

int SearchRecords(Record recordBook[], int count, std::string searchKey){

	for( int pos = 0; pos<count; pos++ ){
		if( recordBook[pos].GetName()==searchKey||recordBook[pos].GetId()==searchKey )
			return pos;
	}
	return MAX_RECORDS+1;
}

std::string CapitalizeStr(std::string lowerCase){
	std::string upperStr;
	for( int pos = 0; pos<lowerCase.length(); pos++ ){
		if( lowerCase[pos]>='a'||lowerCase[pos]<='z' ){
			upperStr += (lowerCase[pos]-32);
		} else{
			upperStr += lowerCase[pos];
		}
	}

	return upperStr;
}

// Prints a report of Total of unique item and total worth of all invetory
void printReport(Record recordBook[], const int count){
	double totalValue = 0.0;

	for( int i = 0; i<count; i++ )
		totalValue += (recordBook[i].GetQuantity()*recordBook[i].GetPrice());

	std::cout<<"Report:\nTotal Number of items: "<<count<<"\nTotal worth of Inventory: $";
	std::cout<<std::setprecision(2)<<std::fixed<<totalValue;
};

