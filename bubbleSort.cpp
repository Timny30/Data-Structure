#include "bubbleSort.h"
#include <chrono>
#include <iomanip>
#include <iostream>

namespace {
bool shouldSwap(const patientRecord& left, const patientRecord& right, SortField field) {
	switch (field) {
	case SortField::VisitDuration:
		return left.lengthOfStay > right.lengthOfStay;
	case SortField::TotalMedicalCost:
		return left.calculateTotalCost() > right.calculateTotalCost();
	case SortField::Age:
	default:
		return left.age > right.age;
	}
}
}

const char* BubbleSort::fieldName(SortField field) {
	switch (field) {
	case SortField::VisitDuration:
		return "Visit Duration";
	case SortField::TotalMedicalCost:
		return "Total Medical Cost";
	case SortField::Age:
	default:
		return "Age";
	}
}

void BubbleSort::printPerformance(const Array& sourceArray) {
	printPerformance(sourceArray, "Dataset");
}

void BubbleSort::printPerformance(const Array& sourceArray, const std::string& facilityName) {
	const SortField fields[] = {
		SortField::Age,
		SortField::VisitDuration,
		SortField::TotalMedicalCost
	};

	std::cout << "\nSORTING PERFORMANCE (Bubble Sort) - " << facilityName << "\n";
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";
	std::cout << "| Sort Key            | Data Structure | Records         | Time (microsec)  | Time Complexity  |\n";
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";

	for (SortField field : fields) {
		Array array = sourceArray;
		auto start = std::chrono::high_resolution_clock::now();
		BubbleSort::sort(array, field);
		auto end = std::chrono::high_resolution_clock::now();

		const auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(
			end - start).count();

		std::cout << "| " << std::left << std::setw(20) << BubbleSort::fieldName(field)
			<< " | " << std::setw(13) << "Array"
			<< " | " << std::setw(15) << sourceArray.size
			<< " | " << std::setw(16) << arrayTime
			<< " | " << std::setw(16) << "O(n^2)"
			<< " |\n";
	}
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";
}

void BubbleSort::sort(Array& array) {
	sort(array, SortField::Age);
}

void BubbleSort::sort(Array& array, SortField field) {
	for (int end = array.size - 1; end > 0; --end) {
		bool swapped = false;
		for (int i = 0; i < end; ++i) {
			if (shouldSwap(array.data[i], array.data[i + 1], field)) {
				patientRecord temporary = array.data[i];
				array.data[i] = array.data[i + 1];
				array.data[i + 1] = temporary;
				swapped = true;
			}
		}
		if (!swapped) {
			break;
		}
	}
}