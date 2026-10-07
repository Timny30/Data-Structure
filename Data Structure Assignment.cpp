#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "quickSortArray.h"
#include "searchExperiment.h"
#include "sortMetrics.h"

template <typename Func>
void runArrayBenchmark(const std::string& rowLabel, const std::string& sortKey, Array arrayCopy, Func sortAlgorithm) {
    auto start = std::chrono::high_resolution_clock::now();
    SortMetrics metrics = sortAlgorithm(arrayCopy);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    size_t memoryUsage = sizeof(Array) + (arrayCopy.size * sizeof(patientRecord));

    std::cout << "| " << std::left << std::setw(15) << rowLabel
              << "| " << std::left << std::setw(20) << sortKey
              << "| " << std::right << std::setw(13) << duration << " us"
              << " | " << std::setw(15) << metrics.comparisons 
              << " | " << std::setw(15) << metrics.dataMovements 
              << " | " << std::setw(15) << memoryUsage << " |\n";
}

int main() {
    Array arr1 = toArray("Datasets/dataset1 facility_a.csv");
    DatasetSummary summary1 = analyzeDataset(arr1, "General Hospital (Facility A)");
    
    Array arr2 = toArray("Datasets/dataset2 facility_b.csv");
    DatasetSummary summary2 = analyzeDataset(arr2, "University Medical Center (Facility B)");
    
    Array arr3 = toArray("Datasets/dataset3_facility_c.csv");
    DatasetSummary summary3 = analyzeDataset(arr3, "Community Health Clinic (Facility C)");
    
    int choice;
    do {
        std::cout << "\n============================================\n";
        std::cout << "         DATA STRUCTURE EXPERIMENTS         \n";
        std::cout << "============================================\n";
        std::cout << "1. Print Dataset Summaries\n";
        std::cout << "2. Quick Sort Data Distribution Experiment\n";
        std::cout << "3. Run All Sorting & Searching Experiments\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                printSummary(summary1);
                printSummary(summary2);
                printSummary(summary3);
                break;

            case 2:
                // =====================================================================
                // QUICK SORT DATA DISTRIBUTION SENSITIVITY
                // =====================================================================
                std::cout << "\n+" << std::string(111, '=') << "+\n";
                std::cout << "| " << std::left << std::setw(109) << "QUICK SORT DATA DISTRIBUTION SENSITIVITY (Array)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";
                std::cout << "| " << std::left << std::setw(15) << "Dataset"
                          << "| " << std::left << std::setw(20) << "Sort Key"
                          << "| " << std::right << std::setw(16) << "Time (Microsec)"
                          << " | " << std::setw(15) << "Comparisons"
                          << " | " << std::setw(15) << "Data Movements" 
                          << " | " << std::setw(15) << "Memory (Bytes)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";

                runArrayBenchmark("Facility A", "Total Medical Cost", arr1, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
                runArrayBenchmark("Facility B", "Total Medical Cost", arr2, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
                runArrayBenchmark("Facility C", "Total Medical Cost", arr3, [](Array& a) { return QuickSortArray::sort(a, SortKey::TotalMedicalCost); });
                
                std::cout << "+" << std::string(111, '=') << "+\n";
                break;

            case 3:
                // Initial Sorting Performance Profiles (Array Only)
                MergeSort::printPerformance(arr1, "Facility A");
                MergeSort::printPerformance(arr2, "Facility B");
                MergeSort::printPerformance(arr3, "Facility C");

                BubbleSort::printPerformance(arr1, "Facility A");
                BubbleSort::printPerformance(arr2, "Facility B");
                BubbleSort::printPerformance(arr3, "Facility C");

                // =====================================================================
                // ALGORITHM VS. ALGORITHM
                // =====================================================================
                std::cout << "\n+" << std::string(111, '=') << "+\n";
                std::cout << "| " << std::left << std::setw(109) << "ALGORITHM COMPARISON ON FACILITY A (Array)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";
                std::cout << "| " << std::left << std::setw(15) << "Algorithm"
                          << "| " << std::left << std::setw(20) << "Sort Key"
                          << "| " << std::right << std::setw(16) << "Time (Microsec)"
                          << " | " << std::setw(15) << "Comparisons"
                          << " | " << std::setw(15) << "Data Movements" 
                          << " | " << std::setw(15) << "Memory (Bytes)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";

                runArrayBenchmark("Bubble Sort", "Visit Duration", arr1, [](Array& a) { return BubbleSort::sort(a, SortField::VisitDuration); });
                runArrayBenchmark("Merge Sort", "Visit Duration", arr1, [](Array& a) { return MergeSort::sort(a, SortKey::VisitDuration); });
                runArrayBenchmark("Quick Sort", "Visit Duration", arr1, [](Array& a) { return QuickSortArray::sort(a, SortKey::VisitDuration); });
                
                std::cout << "+" << std::string(111, '=') << "+\n";

                // Search Experiments
                SearchExperiment::runAgeSearchExperiment(arr1, 61, 100, "General Hospital");
                SearchExperiment::runAgeSearchExperiment(arr2, 18, 25, "University Medical Center");
                SearchExperiment::runCareTypeSearchExperiment(arr2, "Emergency", "University Medical Center");
                SearchExperiment::runDurationSearchExperiment(arr3, 24, "Community Health Clinic");
                break;

            case 0:
                std::cout << "Exiting program...\n";
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}