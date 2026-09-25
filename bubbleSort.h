#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <string>
#include "patientRecord.h"
#include "sortMetrics.h"
#include "benchmarkStats.h"

enum class SortField {
	Age,
	VisitDuration,
	TotalMedicalCost
};

class BubbleSort {
public:
	static SortMetrics sort(Array& array);
	static SortMetrics sort(Array& array, SortField field);
	static const char* fieldName(SortField field);
	static void printPerformance(const Array& sourceArray);
	static void printPerformance(const Array& sourceArray, const std::string& facilityName);
};

#endif