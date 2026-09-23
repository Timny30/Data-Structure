#include <iostream>
#include <iomanip>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "searchExperiment.h"

int main() {
    // Load Datasets into custom Array
    Array arr1 = toArray("Datasets/dataset1 facility_a.csv");
    DatasetSummary summary1 = analyzeDataset(arr1, "General Hospital (Facility A)");
    printSummary(summary1);

    Array arr2 = toArray("Datasets/dataset2 facility_b.csv");
    DatasetSummary summary2 = analyzeDataset(arr2, "University Medical Center (Facility B)");
    printSummary(summary2);

    Array arr3 = toArray("Datasets/dataset3_facility_c.csv");
    DatasetSummary summary3 = analyzeDataset(arr3, "Community Health Clinic (Facility C)");
    printSummary(summary3);

    // Sorting Performance (Array Only)
    MergeSort::printPerformance(arr1, "Facility A");
    MergeSort::printPerformance(arr2, "Facility B");
    MergeSort::printPerformance(arr3, "Facility C");

    BubbleSort::printPerformance(arr1, "Facility A");
    BubbleSort::printPerformance(arr2, "Facility B");
    BubbleSort::printPerformance(arr3, "Facility C");

    // Search Experiments (Array Only)
    SearchExperiment::runAgeSearchExperiment(arr1, 61, 100, "General Hospital");
    SearchExperiment::runAgeSearchExperiment(arr2, 18, 25, "University Medical Center");
    SearchExperiment::runCareTypeSearchExperiment(arr2, "Emergency", "University Medical Center");
    SearchExperiment::runDurationSearchExperiment(arr3, 24, "Community Health Clinic");

    return 0;
}