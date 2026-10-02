#include <iostream>
#include <iomanip>
#include <chrono>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "insertionSortArray.h"
#include "selectionSortArray.h"
#include "quickSortArray.h"
#include "searchExperiment.h"
#include "sortMetrics.h"

// Helper function to time array-based sorting algorithms and track metrics
template <typename Func>
void runArrayBenchmark(const std::string& algoName, Array arrayCopy, Func sortAlgorithm) {
    auto start = std::chrono::high_resolution_clock::now();
    SortMetrics metrics = sortAlgorithm(arrayCopy);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "| " << std::left << std::setw(20) << algoName
              << "| " << std::right << std::setw(15) << duration << " us "
              << "| " << std::setw(15) << metrics.comparisons 
              << "| " << std::setw(15) << metrics.dataMovements << " |\n";
}

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

    // Initial Sorting Performance Profiles (Array Only)
    MergeSort::printPerformance(arr1, "Facility A");
    MergeSort::printPerformance(arr2, "Facility B");
    MergeSort::printPerformance(arr3, "Facility C");

    BubbleSort::printPerformance(arr1, "Facility A");
    BubbleSort::printPerformance(arr2, "Facility B");
    BubbleSort::printPerformance(arr3, "Facility C");

    // =====================================================================
    // EXPERIMENT A: ALGORITHM VS. ALGORITHM (Same Dataset, Same Sort Key)
    // =====================================================================
    std::cout << "\n+" << std::string(73, '=') << "+\n";
    std::cout << "| EXPERIMENT A: ALGORITHM COMPARISON ON FACILITY A (Array)                |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(20) << "Algorithm"
              << "| " << std::right << std::setw(18) << "Time (Microsec)"
              << "| " << std::setw(15) << "Comparisons"
              << "| " << std::setw(15) << "Data Movements" << " |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";

    runArrayBenchmark("Bubble Sort", arr1, [](Array& a) { return BubbleSort::sort(a, SortField::VisitDuration); });
    runArrayBenchmark("Insertion Sort", arr1, [](Array& a) { return InsertionSortArray::sort(a, SortKey::VisitDuration); });
    runArrayBenchmark("Selection Sort", arr1, [](Array& a) { return SelectionSortArray::sort(a, SortKey::VisitDuration); });
    runArrayBenchmark("Merge Sort", arr1, [](Array& a) { return MergeSort::sort(a, SortKey::VisitDuration); });
    runArrayBenchmark("Quick Sort", arr1, [](Array& a) { return QuickSortArray::sort(a, SortKey::VisitDuration); });
    
    std::cout << "+" << std::string(73, '=') << "+\n";

    // =====================================================================
    // EXPERIMENT B: DATASET SIZE SCALING (Same Algorithm, Different Sets)
    // =====================================================================
    std::cout << "\n+" << std::string(73, '=') << "+\n";
    std::cout << "| EXPERIMENT B: QUICK SORT SCALING ACROSS FACILITIES (Array)              |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";
    std::cout << "| " << std::left << std::setw(20) << "Dataset"
              << "| " << std::right << std::setw(18) << "Time (Microsec)"
              << "| " << std::setw(15) << "Comparisons"
              << "| " << std::setw(15) << "Data Movements" << " |\n";
    std::cout << "+" << std::string(73, '-') << "+\n";

    runArrayBenchmark("Facility A", arr1, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
    runArrayBenchmark("Facility B", arr2, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
    runArrayBenchmark("Facility C", arr3, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
    
    std::cout << "+" << std::string(73, '=') << "+\n";

    // Search Experiments (Array Only)
    SearchExperiment::runAgeSearchExperiment(arr1, 61, 100, "General Hospital");
    SearchExperiment::runAgeSearchExperiment(arr2, 18, 25, "University Medical Center");
    SearchExperiment::runCareTypeSearchExperiment(arr2, "Emergency", "University Medical Center");
    SearchExperiment::runDurationSearchExperiment(arr3, 24, "Community Health Clinic");

    return 0;
}