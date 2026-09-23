#include <iostream>
#include <iomanip>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "searchExperiment.h"

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << "      PROGRAM 1: ARRAY-BASED PROTOTYPE EXECUTION       \n";
    std::cout << "=======================================================\n";

    // Facility A
    Array arr1 = toArray("Datasets/dataset1 facility_a.csv");
    DatasetSummary summary1 = analyzeDataset(arr1, "General Hospital (Facility A)");
    printSummary(summary1);

    // Facility B
    Array arr2 = toArray("Datasets/dataset2 facility_b.csv");
    DatasetSummary summary2 = analyzeDataset(arr2, "University Medical Center (Facility B)");
    printSummary(summary2);

    // Facility C
    Array arr3 = toArray("Datasets/dataset3_facility_c.csv");
    DatasetSummary summary3 = analyzeDataset(arr3, "Community Health Clinic (Facility C)");
    printSummary(summary3);

    LinkedList list1 = toLinkedList("Datasets/dataset1 facility_a.csv");
    LinkedList list2 = toLinkedList("Datasets/dataset2 facility_b.csv");
    LinkedList list3 = toLinkedList("Datasets/dataset3_facility_c.csv");

    MergeSort::printPerformance(arr1, list1, "Facility A");
    MergeSort::printPerformance(arr2, list2, "Facility B");
    MergeSort::printPerformance(arr3, list3, "Facility C");

    BubbleSort::printPerformance(arr1, list1, "Facility A");
    BubbleSort::printPerformance(arr2, list2, "Facility B");
    BubbleSort::printPerformance(arr3, list3, "Facility C");

    // =======================================================
    // RUN SEARCH EXPERIMENTS
    // =======================================================
    // Example: Searching for Senior Citizens (Age 61 to 100)
    SearchExperiment::runAgeSearchExperiment(arr1, list1, 61, 100, "General Hospital");
    
    // Example: Searching for Young Adults (Age 18 to 25)
    SearchExperiment::runAgeSearchExperiment(arr2, list2, 18, 25, "University Medical Center");
    
    // NEW: Search for 'Emergency' Care Type in Facility B
    SearchExperiment::runCareTypeSearchExperiment(arr2, list2, "Emergency", "University Medical Center");

    // NEW: Search for Visit Durations >= 24 hours in Facility C
    SearchExperiment::runDurationSearchExperiment(arr3, list3, 24, "Community Health Clinic");

    return 0;
}