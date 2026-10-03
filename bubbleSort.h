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

    static const char* fieldName(SortField field) {
        switch (field) {
        case SortField::VisitDuration:
            return "Visit Duration";
        case SortField::TotalMedicalCost:
            return "Total Medical Cost";
        case SortField::Age:
        default:
            return "Age";
        }
    }

public:
    static const char* keyName(SortField field) {
        return fieldName(field);
    }

    static SortMetrics sort(Array& array, SortField field = SortField::Age) {
        SortMetrics metrics;
        if (array.size < 2) {
            return metrics;
        }

        for (int end = array.size - 1; end > 0; --end) {
            bool swapped = false;
            for (int i = 0; i < end; ++i) {
                metrics.comparisons++;
                if (shouldSwap(array.data[i], array.data[i + 1], field)) {
                    patientRecord temporary = array.data[i];
                    array.data[i] = array.data[i + 1];
                    array.data[i + 1] = temporary;
                    metrics.dataMovements++;
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
        return metrics;
    }

    // Performance runner
    static void printPerformance(const Array& array, const std::string& datasetName = "Dataset") {
        const SortField fields[] = {
            SortField::Age,
            SortField::VisitDuration,
            SortField::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(150, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(148)
                  << ("BUBBLE SORT PERFORMANCE (Array): " + datasetName + " (3 WarmUp & 10 Measured)") << "|\n";
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

        for (SortField field : fields) {
            for (int w = 0; w < SORT_WARMUP_RUNS; ++w) {
                Array warmupCopy(array);
                sort(warmupCopy, field);
            }

            long long samples[SORT_MEASURED_RUNS];
            SortMetrics firstMetrics;
            bool metricsConsistent = true;
            for (int r = 0; r < SORT_MEASURED_RUNS; ++r) {
                Array runCopy(array);

                const auto start = std::chrono::high_resolution_clock::now();
                SortMetrics runMetrics = sort(runCopy, field);
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

            std::cout << "| " << std::left << std::setw(20) << fieldName(field)
                      << std::right << std::setw(14) << stats.medianTimeNs
                      << std::setw(16) << std::fixed << std::setprecision(1) << stats.averageTimeNs
                      << std::setw(12) << stats.minTimeNs
                      << std::setw(12) << stats.maxTimeNs
                      << std::setw(16) << firstMetrics.comparisons
                      << std::setw(18) << firstMetrics.dataMovements
                      << std::setw(16) << "O(n^2)"
                      << std::setw(18) << "O(1)" << " |\n";
            if (!metricsConsistent) {
                std::cout << "| WARNING: comparisons/dataMovements differed across measured runs for "
                          << fieldName(field) << "\n";
            }
        }
        std::cout << "+" << std::string(150, '=') << "+\n";
    }
};

#endif