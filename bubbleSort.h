#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "patientRecord.h"
#include "sortMetrics.h"
#include "benchmarkStats.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

enum class SortField {
    Age,
    VisitDuration,
    TotalMedicalCost
};

class BubbleSort {
private:
    static bool shouldSwap(const patientRecord& left, const patientRecord& right, SortField field) {
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

    static int linkedListSize(const LinkedList& list) {
        int size = 0;
        for (node* current = list.head; current != nullptr; current = current->next) {
            ++size;
        }
        return size;
    }

    static const char* fieldName(SortField field) {
        switch (field) {
        case SortField::VisitDuration: return "Visit Duration";
        case SortField::TotalMedicalCost: return "Total Medical Cost";
        case SortField::Age:
        default: return "Age";
        }
    }

public:
    static SortMetrics sort(LinkedList& list) {
        return sort(list, SortField::Age);
    }

    // metrics.comparisons: one increment per adjacent-node key evaluation.
    // metrics.dataMovements: one increment per completed adjacent-node
    // rearrangement (A->B becoming B->A), regardless of how many internal
    // pointer assignments that relink required.
    static SortMetrics sort(LinkedList& list, SortField field) {
        SortMetrics metrics;
        if (list.head == nullptr || list.head->next == nullptr) {
            return metrics;
        }

        node dummy(patientRecord{});
        dummy.next = list.head;

        node* lastSorted = nullptr;
        bool swapped;

        do {
            swapped = false;
            node* prev = &dummy;
            node* current = prev->next;

            while (current->next != lastSorted) {
                node* nextNode = current->next;

                const bool needsSwap = shouldSwap(current->data, nextNode->data, field);
                metrics.comparisons++;
                if (needsSwap) {
                    current->next = nextNode->next;
                    nextNode->next = current;
                    prev->next = nextNode;

                    prev = nextNode;
                    metrics.dataMovements++;
                    swapped = true;
                } else {
                    prev = current;
                    current = current->next;
                }
            }

            lastSorted = current;
        } while (swapped);

        list.head = dummy.next;

        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
        }
        return metrics;
    }

    static void printPerformance(const LinkedList& sourceList) {
        printPerformance(sourceList, "Dataset");
    }

    static void printPerformance(const LinkedList& sourceList, const std::string& facilityName) {
        const SortField fields[] = {
            SortField::Age,
            SortField::VisitDuration,
            SortField::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(158, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(156)
                  << ("BUBBLE SORT PERFORMANCE: " + facilityName) << " |\n";
        std::cout << "+" << std::string(158, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(22) << "Sort Key"
                  << std::right << std::setw(14) << "Median (ns)"
                  << std::setw(16) << "Average (ns)"
                  << std::setw(12) << "Min (ns)"
                  << std::setw(12) << "Max (ns)"
                  << std::setw(16) << "Comparisons"
                  << std::setw(18) << "Data Movements"
                  << std::setw(20) << "Time Complexity"
                  << std::setw(20) << "Auxiliary Memory" << " |\n";
        std::cout << "+" << std::string(158, '-') << "+\n";

        for (SortField field : fields) {
            // Warm-up runs: fresh deep copy each time, results discarded.
            for (int w = 0; w < SORT_WARMUP_RUNS; ++w) {
                LinkedList warmupCopy = sourceList;
                sort(warmupCopy, field);
            }

            // Measured runs: fresh deep copy each time, starting from the
            // identical original ordering every run.
            long long samples[SORT_MEASURED_RUNS];
            SortMetrics firstMetrics;
            bool metricsConsistent = true;
            for (int r = 0; r < SORT_MEASURED_RUNS; ++r) {
                LinkedList runCopy = sourceList;

                const auto listStart = std::chrono::high_resolution_clock::now();
                SortMetrics runMetrics = sort(runCopy, field);
                const auto listEnd = std::chrono::high_resolution_clock::now();

                samples[r] = std::chrono::duration_cast<std::chrono::nanoseconds>(listEnd - listStart).count();

                if (r == 0) {
                    firstMetrics = runMetrics;
                } else if (runMetrics.comparisons != firstMetrics.comparisons ||
                           runMetrics.dataMovements != firstMetrics.dataMovements) {
                    metricsConsistent = false;
                }
            }

            const BenchmarkStats stats = computeBenchmarkStats(samples, SORT_MEASURED_RUNS);

            std::cout << "| " << std::left << std::setw(22) << fieldName(field)
                      << std::right << std::setw(14) << stats.medianTimeNs
                      << std::setw(16) << std::fixed << std::setprecision(1) << stats.averageTimeNs
                      << std::setw(12) << stats.minTimeNs
                      << std::setw(12) << stats.maxTimeNs
                      << std::setw(16) << firstMetrics.comparisons
                      << std::setw(18) << firstMetrics.dataMovements
                      << std::setw(20) << "O(n^2)"
                      << std::setw(20) << "O(1)" << " |\n";
            if (!metricsConsistent) {
                std::cout << "| WARNING: comparisons/dataMovements differed across measured runs for "
                          << fieldName(field) << "\n";
            }
        }

        std::cout << "+" << std::string(158, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(156)
                  << "Storage: Singly linked list O(n) plus one next pointer per node."
                  << " |\n";
        std::cout << "+" << std::string(158, '=') << "+\n";
    }
};

#endif