// Date created: 2024-06-15
// Date last modified: 2024-06-15

#include "patientRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>

Array toArray(const std::string& filename) {
	Array array;
	std::ifstream file(filename);
	std::string line, temp;

	if (file.is_open()) {
		std::getline(file, line);
		while (std::getline(file, line)) {
			patientRecord record;
			std::stringstream ss(line);

			std::getline(ss, record.patientID, ',');

			std::getline(ss, temp, ',');
			record.age = std::stoi(temp);

			std::getline(ss, record.careType, ',');

			std::getline(ss, temp, ',');
			record.lengthOfStay = std::stoi(temp);

			std::getline(ss, temp, ',');
			record.baseCostPerHour = std::stod(temp);

			std::getline(ss, temp, ',');
			record.daysVisitPerYear = std::stoi(temp);

			array.push_back(record);
		}
		file.close();
	} else {
		std::cerr << "Error opening file: " << filename << "\n";
	}
	return array;
}