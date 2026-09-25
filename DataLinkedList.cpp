#include <iostream>
#include <iomanip>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "searchExperiment.h"
#include "linkedListMemory.h"

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << "   PROGRAM 2: LINKEDLIST-BASED PROTOTYPE EXECUTION     \n";
    std::cout << "=======================================================\n";

    // Dataset 1
    LinkedList list1 = toLinkedList("Datasets/dataset1 facility_a.csv");
    DatasetSummary summary1 = analyzeDataset(list1, "General Hospital (Facility A)");
    printSummary(summary1);

    // Dataset 2
    LinkedList list2 = toLinkedList("Datasets/dataset2 facility_b.csv");
    DatasetSummary summary2 = analyzeDataset(list2, "University Medical Center (Facility B)");
    printSummary(summary2);

    // Dataset 3
    LinkedList list3 = toLinkedList("Datasets/dataset3_facility_c.csv");
    DatasetSummary summary3 = analyzeDataset(list3, "Community Health Clinic (Facility C)");
    printSummary(summary3);

    // Linked List Structural Memory Footprint (Linked List Only)
    printLinkedListMemoryFootprint(list1, "Facility A");
    printLinkedListMemoryFootprint(list2, "Facility B");
    printLinkedListMemoryFootprint(list3, "Facility C");

    MergeSort::printPerformance(list1, "Facility A");
    MergeSort::printPerformance(list2, "Facility B");
    MergeSort::printPerformance(list3, "Facility C");

    BubbleSort::printPerformance(list1, "Facility A");
    BubbleSort::printPerformance(list2, "Facility B");
    BubbleSort::printPerformance(list3, "Facility C");

    // =======================================================
    // RUN SEARCH EXPERIMENTS
    // =======================================================
    // Example: Searching for Senior Citizens (Age 61 to 100)
    SearchExperiment::runAgeSearchExperiment(list1, 61, 100, "General Hospital");
    
    // Example: Searching for Young Adults (Age 18 to 25)
    SearchExperiment::runAgeSearchExperiment(list2, 18, 25, "University Medical Center");
    
    // NEW: Search for 'Emergency' Care Type in Facility B
    SearchExperiment::runCareTypeSearchExperiment(list2, "Emergency", "University Medical Center");

    // NEW: Search for Visit Durations >= 24 hours in Facility C
    SearchExperiment::runDurationSearchExperiment(list3, 24, "Community Health Clinic");

    return 0;
}