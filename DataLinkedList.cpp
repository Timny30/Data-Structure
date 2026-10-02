#include <iostream>
#include <iomanip>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "searchExperiment.h"
#include "linkedListMemory.h"
#include "insertionSort.h"
#include "selectionSort.h"
#include "quickSort.h"

template <typename Func>
void runComparativeBenchmark(const std::string& algoName, LinkedList listCopy, Func sortAlgorithm) {
    auto start = std::chrono::high_resolution_clock::now();
    SortMetrics metrics = sortAlgorithm(listCopy);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "| " << std::left << std::setw(20) << algoName
              << "| " << std::right << std::setw(15) << duration << " us "
              << "| " << std::setw(15) << metrics.comparisons 
              << "| " << std::setw(15) << metrics.dataMovements << " |\n";
}

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

    // =====================================================================
    // EXPERIMENT A: ALGORITHM VS. ALGORITHM (Same Dataset, Same Sort Key)
    // Testing how different algorithms perform on Facility A by Visit Duration
    // =====================================================================
    std::cout << "\n+" << std::string(73, '=') << "+\n";
    std::cout << "| EXPERIMENT A: ALGORITHM COMPARISON ON FACILITY A (Sort: Duration)       |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(20) << "Algorithm"
              << "| " << std::right << std::setw(18) << "Time (Microsec)"
              << "| " << std::setw(15) << "Comparisons"
              << "| " << std::setw(15) << "Data Movements" << " |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";

    runComparativeBenchmark("Bubble Sort", list1, [](LinkedList& l) { return BubbleSort::sort(l, SortField::VisitDuration); });
    runComparativeBenchmark("Insertion Sort", list1, [](LinkedList& l) { return InsertionSort::sort(l, SortKey::VisitDuration); });
    runComparativeBenchmark("Selection Sort", list1, [](LinkedList& l) { return SelectionSort::sort(l, SortKey::VisitDuration); });
    runComparativeBenchmark("Merge Sort", list1, [](LinkedList& l) { return MergeSort::sort(l, SortKey::VisitDuration); });
    runComparativeBenchmark("Quick Sort", list1, [](LinkedList& l) { return QuickSort::sort(l, SortKey::VisitDuration); });
    
    std::cout << "+" << std::string(73, '=') << "+\n";

    // =====================================================================
    // EXPERIMENT B: DATASET SIZE COMPARISON (Same Algorithm, Different Sets)
    // Testing Quick Sort performance scaling across Facilities A, B, and C
    // =====================================================================
    std::cout << "\n+" << std::string(73, '=') << "+\n";
    std::cout << "| EXPERIMENT B: QUICK SORT SCALING ACROSS FACILITIES (Sort: Total Cost)   |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(20) << "Dataset"
              << "| " << std::right << std::setw(18) << "Time (Microsec)"
              << "| " << std::setw(15) << "Comparisons"
              << "| " << std::setw(15) << "Data Movements" << " |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";

    runComparativeBenchmark("Facility A", list1, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
    runComparativeBenchmark("Facility B", list2, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
    runComparativeBenchmark("Facility C", list3, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
    
    std::cout << "+" << std::string(73, '=') << "+\n";

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