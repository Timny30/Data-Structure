
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

class Array {

public:
	patientRecord* data;
	int size;
	int capacity;
	
	Array() {
		capacity = 10;
		size = 0;
		data = new patientRecord[capacity];
	}

	Array(const Array& other) {
		size = other.size;
		capacity = other.capacity;
		data = new patientRecord[capacity];
		for (int i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	~Array() {
		delete[] data;
	}

	void push_back(const patientRecord& record) {
		if (size == capacity) {
			capacity *= 2;
			patientRecord* newData = new patientRecord[capacity];
			for (int i = 0; i < size; i++) {
				newData[i] = data[i];
			}
			delete[] data;
			data = newData; 
		}
		data[size++] = record;
	}
};

Array toArray(const std::string& filename);

#endif