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
    // metrics.dataMovements: one increment per patientRecord placed into the
    // merge buffer (includes leftover left/right copies).
    static void merge(patientRecord* data, patientRecord* buffer,
                      int first, int middle, int last, SortKey key, SortMetrics& metrics) {
        int left = first;
        int right = middle + 1;
        int output = first;

        while (left <= middle && right <= last) {
            const bool rightComesFirst = comesBefore(data[right], data[left], key);
            metrics.comparisons++;
            if (rightComesFirst) {
                buffer[output++] = data[right++];
            } else {
                buffer[output++] = data[left++];
            }
            metrics.dataMovements++;
        }
        while (left <= middle) {
            buffer[output++] = data[left++];
            metrics.dataMovements++;
        }
        while (right <= last) {
            buffer[output++] = data[right++];
            metrics.dataMovements++;
        }
        for (int i = first; i <= last; ++i) {
            data[i] = buffer[i];
        }
    }

    static void sortArray(patientRecord* data, patientRecord* buffer,
                          int first, int last, SortKey key, SortMetrics& metrics) {
        if (first >= last) {
            return;
        }
        const int middle = first + (last - first) / 2;
        sortArray(data, buffer, first, middle, key, metrics);
        sortArray(data, buffer, middle + 1, last, key, metrics);
        merge(data, buffer, first, middle, last, key, metrics);
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
    static SortMetrics sort(Array& array, SortKey key) {
        SortMetrics metrics;
        if (array.size < 2) {
            return metrics;
        }
        patientRecord* buffer = new patientRecord[array.size];
        sortArray(array.data, buffer, 0, array.size - 1, key, metrics);
        delete[] buffer;
        return metrics;
    }

    static void printPerformance(const Array& array, const std::string& datasetName) {
        const SortKey keys[] = {
            SortKey::Age, SortKey::VisitDuration, SortKey::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(150, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(148)
                  << ("MERGE SORT PERFORMANCE (Array): " + datasetName) << "|\n";
        std::cout << "+" << std::string(150, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(20) << "Sort Key"
                  << std::right << std::setw(14) << "Median (ns)"
                  << std::setw(16) << "Average (ns)"
                  << std::setw(12) << "Min (ns)"
                  << std::setw(12) << "Max (ns)"
                  << std::setw(16) << "Comparisons"
                  << std::setw(18) << "Data Movements"
                  << std::setw(16) << "Time Complexity"
                  << std::setw(18) << "Auxiliary Memory" << " |\n";
        std::cout << "+" << std::string(150, '-') << "+\n";

        for (SortKey key : keys) {
            // Warm-up runs: fresh copy each time, results discarded.
            for (int w = 0; w < SORT_WARMUP_RUNS; ++w) {
                Array warmupCopy(array);
                sort(warmupCopy, key);
            }

            // Measured runs: fresh copy each time (copy construction is
            // outside the timed region), starting from the identical
            // original ordering every run.
            long long samples[SORT_MEASURED_RUNS];
            SortMetrics firstMetrics;
            bool metricsConsistent = true;
            for (int r = 0; r < SORT_MEASURED_RUNS; ++r) {
                Array runCopy(array);

                const auto start = std::chrono::high_resolution_clock::now();
                SortMetrics runMetrics = sort(runCopy, key);
                const auto end = std::chrono::high_resolution_clock::now();

                samples[r] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                if (r == 0) {
                    firstMetrics = runMetrics;
                } else if (runMetrics.comparisons != firstMetrics.comparisons ||
                           runMetrics.dataMovements != firstMetrics.dataMovements) {
                    metricsConsistent = false;
                }
            }

            const BenchmarkStats stats = computeBenchmarkStats(samples, SORT_MEASURED_RUNS);

            std::cout << "| " << std::left << std::setw(20) << keyName(key)
                      << std::right << std::setw(14) << stats.medianTimeNs
                      << std::setw(16) << std::fixed << std::setprecision(1) << stats.averageTimeNs
                      << std::setw(12) << stats.minTimeNs
                      << std::setw(12) << stats.maxTimeNs
                      << std::setw(16) << firstMetrics.comparisons
                      << std::setw(18) << firstMetrics.dataMovements
                      << std::setw(16) << "O(n log n)"
                      << std::setw(18) << "O(n)" << " |\n";
            if (!metricsConsistent) {
                std::cout << "| WARNING: comparisons/dataMovements differed across measured runs for "
                          << keyName(key) << "\n";
            }
        }
        std::cout << "+" << std::string(150, '=') << "+\n";
    }
};

#endif