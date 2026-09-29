#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>

struct patientRecord { 
	std::string patientID;
	int age;
	std::string careType;
	int lengthOfStay;
	double baseCostPerHour;
	int daysVisitPerYear;

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

struct node { 
	patientRecord data;
	node* next; 
	node(const patientRecord& record) : data(record), next(nullptr) {}
};

class LinkedList {
public:
	node* head;
	node* tail;

	LinkedList() : head(nullptr), tail(nullptr) {}

	void push_back(const patientRecord& record) { //Adding new records
		node* newnode = new node(record); 
		if (head == nullptr) { 
			head = tail = newnode; 
		}
		else {
			tail->next = newnode;
			tail = newnode;
		}
	}

	LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) { //Copy Constructor Function
		node* current = other.head;
		while (current != nullptr) {
			push_back(current->data); 
			current = current->next; 
		}
	}

	~LinkedList() { //Destructor Function
		node* current = head;
		while (current != nullptr) {
			node* nextNode = current->next; 
			delete current; 
			current = nextNode; 
		}
	}

};

LinkedList toLinkedList(const std::string& filename);

#endif