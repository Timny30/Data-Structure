// Date created: 2024-06-15
// Date last modified: 2024-06-15

#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>

struct patientRecord { // data container: containing dataset patient information
	std::string patientID;
	int age;
	std::string careType;
	int lengthOfStay;
	double baseCostPerHour;
	int daysVisitPerYear;
	// Calculates total medical cost
    double calculateTotalCost() const {
        return lengthOfStay * baseCostPerHour * daysVisitPerYear;
    }

    // Recategorize patients
    std::string getAgeGroup() const {
        if (age >= 0 && age <= 17) {
            return "0-17: Pediatrics & Adolescents";
        } else if (age >= 18 && age <= 25) {
            return "18-25: Young Adults / University Students";
        } else if (age >= 26 && age <= 45) {
            return "26-45: Working Adults (Early Career)";
        } else if (age >= 46 && age <= 60) {
            return "46-60: Working Adults (Late Career)";
        } else if (age >= 61 && age <= 100) {
            return "61-100: Senior Citizens / Geriatric Care";
        } else {
            return "Unknown Age Group";
        }
    }
};

class Array { // build using raw memory allocation

public:
	patientRecord* data; // pointer to the array of patient records
	int size; // number of patients currently stored
	int capacity; // maximum patients number current memory can hold
	
	Array() { // Array constructor
		capacity = 10;
		size = 0;
		data = new patientRecord[capacity]; // generate 10 records to start 
	}

	/// when array is returned, computer will copy the memory address of the returned array
	/// However, the returned array address has been deleted by destructor
	/// computer crash when old address unable to access
	/// This function is to create memory address for return array in order to be accessed 
	Array(const Array& other) {
		size = other.size;
		capacity = other.capacity;
		data = new patientRecord[capacity];
		for (int i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	~Array() { // destructor: free up memory when array is no longer needed
		delete[] data;
	}

	// Expand the size of array if more data comes in
	void push_back(const patientRecord& record) {
		if (size == capacity) {
			capacity *= 2; // expand twice the array size
			patientRecord* newData = new patientRecord[capacity];
			for (int i = 0; i < size; i++) {
				newData[i] = data[i]; // replacing old data from old into new array
			}
			delete[] data; // delete old array
			data = newData; 
		}
		data[size++] = record; // add in the new data into new array
	}
};

struct node { // build via chaining data points across memory
	patientRecord data;
	node* next; // map the linkedList to memory 
	node(const patientRecord& record) : data(record), next(nullptr) {}
};

class LinkedList {
public:
	node* head; // start of the chain
	node* tail; // end of the chain

	// prevent crash while accessing the new linkedList 
	LinkedList() : head(nullptr), tail(nullptr) {}

	LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
		node* current = other.head; // link starting line of the original list
		while (current != nullptr) {
			push_back(current->data); // reads patient data within node
			current = current->next; // push data into bew node 
		}
	}

	~LinkedList() { // destructor: deleting each node preventing memory leak
		node* current = head;
		while (current != nullptr) {
			node* nextNode = current->next; // save it in another node
			delete current; 
			current = nextNode; // asign new node as current node
		}
	}

	void push_back(const patientRecord& record) {
		node* newnode = new node(record); // construct new node
		if (head == nullptr) { // if list is empty
			head = tail = newnode; // data will be the starting and ending point
		}
		else {
			tail->next = newnode; // added data will be the tail
			tail = newnode;
		}
	}

};

Array toArray(const std::string& filename);
LinkedList toLinkedList(const std::string& filename);

#endif