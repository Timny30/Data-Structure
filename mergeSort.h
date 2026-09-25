#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "patientRecord.h"
#include "sortMetrics.h"
#include "benchmarkStats.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

enum class SortKey {
    Age,
    VisitDuration,
    TotalMedicalCost
};

class MergeSort {
private:
    static bool comesBefore(const patientRecord& left, const patientRecord& right, SortKey key) {
        switch (key) {
        case SortKey::Age:
            return left.age < right.age;
        case SortKey::VisitDuration:
            return left.lengthOfStay < right.lengthOfStay;
        case SortKey::TotalMedicalCost:
            return left.calculateTotalCost() < right.calculateTotalCost();
        }
        return false;
    }

    // metrics.comparisons: one increment per left-vs-right sort-key evaluation.
    // metrics.dataMovements: one increment per node attached into the merged
    // ordering while both candidate lists are still non-empty. Once one side
    // is exhausted, the remaining (already-ordered) chain is attached directly
    // without traversing/counting each of its nodes individually, matching
    // the algorithm's actual O(1) remainder-attach behavior.
    static node* mergeLists(node* left, node* right, SortKey key, SortMetrics& metrics) {
        node dummy(patientRecord{});
        node* tail = &dummy;

        while (left != nullptr && right != nullptr) {
            const bool rightComesFirst = comesBefore(right->data, left->data, key);
            metrics.comparisons++;
            if (rightComesFirst) {
                tail->next = right;
                right = right->next;
            } else {
                tail->next = left;
                left = left->next;
            }
            metrics.dataMovements++;
            tail = tail->next;
        }

        tail->next = (left != nullptr) ? left : right;
        return dummy.next;
    }

    static node* splitList(node* head) {
        node* slow = head;
        node* fast = head->next;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        node* second = slow->next;
        slow->next = nullptr;
        return second;
    }

    static node* sortList(node* head, SortKey key, SortMetrics& metrics) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        node* second = splitList(head);
        head = sortList(head, key, metrics);
        second = sortList(second, key, metrics);

        return mergeLists(head, second, key, metrics);
    }

    static const char* keyName(SortKey key) {
        switch (key) {
        case SortKey::Age: return "Age";
        case SortKey::VisitDuration: return "Visit Duration";
        case SortKey::TotalMedicalCost: return "Total Medical Cost";
        }
        return "Unknown";
    }

public:
    static SortMetrics sort(LinkedList& list, SortKey key) {
        SortMetrics metrics;
        list.head = sortList(list.head, key, metrics);
        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
        }
        return metrics;
    }

    static void printPerformance(const LinkedList& list, const std::string& datasetName) {
        const SortKey keys[] = {
            SortKey::Age, SortKey::VisitDuration, SortKey::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(158, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(156)
                  << ("MERGE SORT PERFORMANCE: " + datasetName) << " |\n";
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

        for (SortKey key : keys) {
            // Warm-up runs: fresh deep copy each time, results discarded.
            for (int w = 0; w < SORT_WARMUP_RUNS; ++w) {
                LinkedList warmupCopy(list);
                sort(warmupCopy, key);
            }

            // Measured runs: fresh deep copy each time (copy construction is
            // outside the timed region), starting from the identical
            // original ordering every run.
            long long samples[SORT_MEASURED_RUNS];
            SortMetrics firstMetrics;
            bool metricsConsistent = true;
            for (int r = 0; r < SORT_MEASURED_RUNS; ++r) {
                LinkedList runCopy(list);

                const auto listStart = std::chrono::high_resolution_clock::now();
                SortMetrics runMetrics = sort(runCopy, key);
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

            std::cout << "| " << std::left << std::setw(22) << keyName(key)
                      << std::right << std::setw(14) << stats.medianTimeNs
                      << std::setw(16) << std::fixed << std::setprecision(1) << stats.averageTimeNs
                      << std::setw(12) << stats.minTimeNs
                      << std::setw(12) << stats.maxTimeNs
                      << std::setw(16) << firstMetrics.comparisons
                      << std::setw(18) << firstMetrics.dataMovements
                      << std::setw(20) << "O(n log n)"
                      << std::setw(20) << "O(log n)" << " |\n";
            if (!metricsConsistent) {
                std::cout << "| WARNING: comparisons/dataMovements differed across measured runs for "
                          << keyName(key) << "\n";
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