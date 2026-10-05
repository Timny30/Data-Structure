#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include "patientRecord.h"
#include "calculation.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "searchExperiment.h"
#include "linkedListMemory.h"
#include "quickSort.h"

// Helper function to time linked list-based sorting algorithms and track metrics, now including memory
template <typename Func>
void runComparativeBenchmark(const std::string& rowLabel, const std::string& sortKey, LinkedList listCopy, Func sortAlgorithm) {
    // Calculate memory usage (size of the LinkedList struct + size of each dynamically allocated node)
    size_t memoryUsage = sizeof(LinkedList);
    node* tempNode = listCopy.head;
    while (tempNode != nullptr) {
        memoryUsage += sizeof(*tempNode);
        tempNode = tempNode->next;
    }

    auto start = std::chrono::high_resolution_clock::now();
    SortMetrics metrics = sortAlgorithm(listCopy);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "| " << std::left << std::setw(15) << rowLabel
              << "| " << std::left << std::setw(20) << sortKey
              << "| " << std::right << std::setw(13) << duration << " us"
              << " | " << std::setw(15) << metrics.comparisons 
              << " | " << std::setw(15) << metrics.dataMovements 
              << " | " << std::setw(15) << memoryUsage << " |\n";
}

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << "   PROGRAM 2: LINKEDLIST-BASED PROTOTYPE EXECUTION     \n";
    std::cout << "=======================================================\n";

    // Load Datasets
    LinkedList list1 = toLinkedList("Datasets/dataset1 facility_a.csv");
    DatasetSummary summary1 = analyzeDataset(list1, "General Hospital (Facility A)");
    
    LinkedList list2 = toLinkedList("Datasets/dataset2 facility_b.csv");
    DatasetSummary summary2 = analyzeDataset(list2, "University Medical Center (Facility B)");
    
    LinkedList list3 = toLinkedList("Datasets/dataset3_facility_c.csv");
    DatasetSummary summary3 = analyzeDataset(list3, "Community Health Clinic (Facility C)");

    int choice;
    do {
        std::cout << "\n============================================\n";
        std::cout << "         DATA STRUCTURE EXPERIMENTS         \n";
        std::cout << "============================================\n";
        std::cout << "1. Print Dataset Summaries & Memory Footprint\n";
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

                // Linked List Structural Memory Footprint
                printLinkedListMemoryFootprint(list1, "Facility A");
                printLinkedListMemoryFootprint(list2, "Facility B");
                printLinkedListMemoryFootprint(list3, "Facility C");
                break;

            case 2:
                // =====================================================================
                // DATA DISTRIBUTION SENSITIVITY (Same Algorithm, Different Sets)
                // =====================================================================
                std::cout << "\n+" << std::string(111, '=') << "+\n";
                std::cout << "| " << std::left << std::setw(109) << "QUICK SORT DATA DISTRIBUTION SENSITIVITY (Linked List)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";
                std::cout << "| " << std::left << std::setw(15) << "Dataset"
                          << "| " << std::left << std::setw(20) << "Sort Key"
                          << "| " << std::right << std::setw(16) << "Time (Microsec)"
                          << " | " << std::setw(15) << "Comparisons"
                          << " | " << std::setw(15) << "Data Movements" 
                          << " | " << std::setw(15) << "Memory (Bytes)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";

                runComparativeBenchmark("Facility A", "Total Medical Cost", list1, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
                runComparativeBenchmark("Facility B", "Total Medical Cost", list2, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
                runComparativeBenchmark("Facility C", "Total Medical Cost", list3, [](LinkedList& l) { return QuickSort::sort(l, SortKey::TotalMedicalCost); });
                
                std::cout << "+" << std::string(111, '=') << "+\n";
                break;

            case 3:
                MergeSort::printPerformance(list1, "Facility A");
                MergeSort::printPerformance(list2, "Facility B");
                MergeSort::printPerformance(list3, "Facility C");

                BubbleSort::printPerformance(list1, "Facility A");
                BubbleSort::printPerformance(list2, "Facility B");
                BubbleSort::printPerformance(list3, "Facility C");

                // =====================================================================
                // EXPERIMENT A: ALGORITHM VS. ALGORITHM
                // =====================================================================
                std::cout << "\n+" << std::string(111, '=') << "+\n";
                std::cout << "| " << std::left << std::setw(109) << "EXPERIMENT A: ALGORITHM COMPARISON ON FACILITY A (Linked List)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";
                std::cout << "| " << std::left << std::setw(15) << "Algorithm"
                          << "| " << std::left << std::setw(20) << "Sort Key"
                          << "| " << std::right << std::setw(16) << "Time (Microsec)"
                          << " | " << std::setw(15) << "Comparisons"
                          << " | " << std::setw(15) << "Data Movements" 
                          << " | " << std::setw(15) << "Memory (Bytes)" << " |\n";
                std::cout << "+" << std::string(111, '-') << "+\n";

                runComparativeBenchmark("Bubble Sort", "Visit Duration", list1, [](LinkedList& l) { return BubbleSort::sort(l, SortField::VisitDuration); });
                runComparativeBenchmark("Merge Sort", "Visit Duration", list1, [](LinkedList& l) { return MergeSort::sort(l, SortKey::VisitDuration); });
                runComparativeBenchmark("Quick Sort", "Visit Duration", list1, [](LinkedList& l) { return QuickSort::sort(l, SortKey::VisitDuration); });
                
                std::cout << "+" << std::string(111, '=') << "+\n";

                // =======================================================
                // RUN SEARCH EXPERIMENTS
                // =======================================================
                SearchExperiment::runAgeSearchExperiment(list1, 61, 100, "General Hospital");
                SearchExperiment::runAgeSearchExperiment(list2, 18, 25, "University Medical Center");
                SearchExperiment::runCareTypeSearchExperiment(list2, "Emergency", "University Medical Center");
                SearchExperiment::runDurationSearchExperiment(list3, 24, "Community Health Clinic");
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