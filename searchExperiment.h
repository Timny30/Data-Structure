#ifndef SEARCH_EXPERIMENT_H
#define SEARCH_EXPERIMENT_H

#include "patientRecord.h"
#include "mergeSort.h"
#include "bubbleSort.h"
#include "benchmarkStats.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>

struct SearchResult {
    int matchesFound = 0;
    long long comparisons = 0;
    long long recordAccesses = 0;
    long long durationNanosec = 0;

    std::size_t memoryBytes = 0;

    std::string memoryOverhead = "O(1)";
};

struct RepeatedSearchOutcome {
    SearchResult result;
    BenchmarkStats timing;
    bool metricsConsistent = true;
};

class SearchExperiment {
private:
    static SearchResult linearSearchAgeArray(const Array& arr, int minAge, int maxAge, bool isSorted) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < arr.size; i++) {
            res.comparisons++;
            res.recordAccesses++;
            if (arr.data[i].age >= minAge && arr.data[i].age <= maxAge) res.matchesFound++;
            else if (isSorted && arr.data[i].age > maxAge) break;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    static SearchResult jumpSearchAgeArray(const Array& arr, int minAge, int maxAge) {
        SearchResult res;
        if (arr.size == 0) return res;
        auto start = std::chrono::high_resolution_clock::now();
        int n = arr.size;
        int step = std::sqrt(n);
        int prev = 0;

        while (arr.data[std::min(step, n) - 1].age < minAge) {
            res.comparisons++;
            res.recordAccesses++;
            prev = step;
            step += std::sqrt(n);
            if (prev >= n) break;
        }
        if (prev < n) { res.comparisons++; res.recordAccesses++; }

        while (prev < std::min(step, n) && arr.data[prev].age < minAge) {
            res.comparisons++;
            res.recordAccesses++;
            prev++;
        }
        if (prev < std::min(step, n)) { res.comparisons++; res.recordAccesses++; }

        while (prev < n && arr.data[prev].age <= maxAge) {
            res.comparisons++;
            res.recordAccesses++;
            if (arr.data[prev].age >= minAge) res.matchesFound++;
            prev++;
        }
        if (prev < n) { res.comparisons++; res.recordAccesses++; }

        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    static SearchResult linearSearchCareTypeArray(const Array& arr, const std::string& targetType) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < arr.size; i++) {
            res.comparisons++;
            res.recordAccesses++;
            if (arr.data[i].careType == targetType) res.matchesFound++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    static void sortCareTypeArrayCopy(Array& arr) {
        std::sort(arr.data, arr.data + arr.size,
                  [](const patientRecord& a, const patientRecord& b) {
                      return a.careType < b.careType;
                  });
    }

    static SearchResult linearSearchDurationArray(const Array& arr, int threshold) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < arr.size; i++) {
            res.comparisons++;
            res.recordAccesses++;
            if (arr.data[i].lengthOfStay >= threshold) res.matchesFound++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    static SearchResult jumpSearchDurationArray(const Array& arr, int threshold) {
        SearchResult res;
        if (arr.size == 0) return res;
        auto start = std::chrono::high_resolution_clock::now();

        int n = arr.size;
        int step = std::sqrt(n);
        int prev = 0;

        while (arr.data[std::min(step, n) - 1].lengthOfStay < threshold) {
            res.comparisons++;
            res.recordAccesses++;
            prev = step;
            step += std::sqrt(n);
            if (prev >= n) break;
        }
        if (prev < n) { res.comparisons++; res.recordAccesses++; }

        while (prev < std::min(step, n) && arr.data[prev].lengthOfStay < threshold) {
            res.comparisons++;
            res.recordAccesses++;
            prev++;
        }
        if (prev < std::min(step, n)) { res.comparisons++; res.recordAccesses++; }

        while (prev < n) {
            res.comparisons++;
            res.recordAccesses++;
            if (arr.data[prev].lengthOfStay >= threshold) res.matchesFound++;
            prev++;
        }

        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    

    static void printHeader(const std::string& title) {
        std::cout << "\n+" << std::string(166, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(164) << title << " |\n";
        std::cout << "+" << std::string(166, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(15) << "Algorithm"
                  << "| " << std::setw(15) << "Data Struct"
                  << "| " << std::setw(10) << "State"
                  << "| " << std::setw(8) << "Matches"
                  << "| " << std::setw(12) << "Comparisons"
                  << "| " << std::setw(16) << "Record Accesses"
                  << "| " << std::setw(12) << "Median (ns)"
                  << "| " << std::setw(14) << "Average (ns)"
                  << "| " << std::setw(10) << "Min (ns)"
                  << "| " << std::setw(10) << "Max (ns)"
                  << "| " << std::setw(12) << "Mem Ovhd" << " |\n";
        std::cout << "+" << std::string(166, '-') << "+\n";
    }

    static void printRow(const std::string& algo, const std::string& ds, const std::string& state, const RepeatedSearchOutcome& outcome) {
        std::cout << "| " << std::left << std::setw(15) << algo
                  << "| " << std::setw(15) << ds
                  << "| " << std::setw(10) << state
                  << "| " << std::right << std::setw(8) << outcome.result.matchesFound
                  << "| " << std::setw(12) << outcome.result.comparisons
                  << "| " << std::setw(16) << outcome.result.recordAccesses
                  << "| " << std::setw(12) << outcome.timing.medianTimeNs
                  << "| " << std::setw(14) << std::fixed << std::setprecision(1) << outcome.timing.averageTimeNs
                  << "| " << std::setw(10) << outcome.timing.minTimeNs
                  << "| " << std::setw(10) << outcome.timing.maxTimeNs
                  << "| " << std::setw(12) << outcome.result.memoryOverhead << " |\n";
        if (!outcome.metricsConsistent) {
            std::cout << "| WARNING: matches/comparisons/recordAccesses differed across measured runs for "
                      << algo << "\n";
        }
    }

    static void printCareTypeHeader(const std::string& title) {
        std::cout << "\n+" << std::string(182, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(180) << title << " |\n";
        std::cout << "+" << std::string(182, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(15) << "Algorithm"
                  << "| " << std::setw(15) << "Data Struct"
                  << "| " << std::setw(10) << "State"
                  << "| " << std::setw(8) << "Matches"
                  << "| " << std::setw(12) << "Comparisons"
                  << "| " << std::setw(16) << "Record Accesses"
                  << "| " << std::setw(12) << "Median (ns)"
                  << "| " << std::setw(14) << "Average (ns)"
                  << "| " << std::setw(10) << "Min (ns)"
                  << "| " << std::setw(10) << "Max (ns)"
                  << "| " << std::setw(16) << "Memory (Bytes)"
                  << "| " << std::setw(12) << "Aux Space" << " |\n";
        std::cout << "+" << std::string(182, '-') << "+\n";
    }

    static void printCareTypeRow(const std::string& algo, const std::string& ds, const std::string& state, const RepeatedSearchOutcome& outcome) {
        std::cout << "| " << std::left << std::setw(15) << algo
                  << "| " << std::setw(15) << ds
                  << "| " << std::setw(10) << state
                  << "| " << std::right << std::setw(8) << outcome.result.matchesFound
                  << "| " << std::setw(12) << outcome.result.comparisons
                  << "| " << std::setw(16) << outcome.result.recordAccesses
                  << "| " << std::setw(12) << outcome.timing.medianTimeNs
                  << "| " << std::setw(14) << std::fixed << std::setprecision(1) << outcome.timing.averageTimeNs
                  << "| " << std::setw(10) << outcome.timing.minTimeNs
                  << "| " << std::setw(10) << outcome.timing.maxTimeNs
                  << "| " << std::setw(16) << outcome.result.memoryBytes
                  << "| " << std::setw(12) << outcome.result.memoryOverhead << " |\n";
        if (!outcome.metricsConsistent) {
            std::cout << "| WARNING: matches/comparisons/recordAccesses differed across measured runs for "
                      << algo << "\n";
        }
    }

    template <typename SearchFunc>
    static RepeatedSearchOutcome runRepeatedSearch(SearchFunc searchFunc) {
        for (int w = 0; w < SEARCH_WARMUP_RUNS; ++w) {
            searchFunc();
        }

        long long samples[SEARCH_MEASURED_RUNS];
        RepeatedSearchOutcome outcome;
        for (int r = 0; r < SEARCH_MEASURED_RUNS; ++r) {
            SearchResult res = searchFunc();
            samples[r] = res.durationNanosec;
            if (r == 0) {
                outcome.result = res;
            } else if (res.matchesFound != outcome.result.matchesFound ||
                       res.comparisons != outcome.result.comparisons ||
                       res.recordAccesses != outcome.result.recordAccesses) {
                outcome.metricsConsistent = false;
            }
        }

        outcome.timing = computeBenchmarkStats(samples, SEARCH_MEASURED_RUNS);
        return outcome;
    }

public:
    static void runAgeSearchExperiment(const Array& originalArr, int minAge, int maxAge, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Target Age: " + std::to_string(minAge) + " - " + std::to_string(maxAge) + ")");

        printRow("Linear Search", "Array", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchAgeArray(originalArr, minAge, maxAge, false);
        }));

        Array sortedArr(originalArr);
        MergeSort::sort(sortedArr, SortKey::Age);

        printRow("Linear Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return linearSearchAgeArray(sortedArr, minAge, maxAge, true);
        }));
        
        printRow("Jump Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return jumpSearchAgeArray(sortedArr, minAge, maxAge);
        }));
        
        std::cout << "+" << std::string(166, '=') << "+\n";
    }

    static void runCareTypeSearchExperiment(const Array& originalArr, const std::string& targetType, const std::string& facility) {
        printCareTypeHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");

        RepeatedSearchOutcome unsortedOutcome = runRepeatedSearch([&]() {
            return linearSearchCareTypeArray(originalArr, targetType);
        });

        unsortedOutcome.result.memoryBytes = BubbleSort::calculateMemoryUsage(originalArr);
        printCareTypeRow("Linear Search", "Array", "Unsorted", unsortedOutcome);

        Array careTypeSortedArr(originalArr);
        sortCareTypeArrayCopy(careTypeSortedArr);

        RepeatedSearchOutcome sortedOutcome = runRepeatedSearch([&]() {
            return linearSearchCareTypeArray(careTypeSortedArr, targetType);
        });
        sortedOutcome.result.memoryBytes = BubbleSort::calculateMemoryUsage(careTypeSortedArr);
        printCareTypeRow("Linear Search", "Array", "Sorted", sortedOutcome);

        std::cout << "+" << std::string(182, '-') << "+\n";
        std::cout << "| Note: Care Type experiment evaluates Linear Search only; Jump Search is\n";
        std::cout << "| not implemented for string-based Care Type searching.\n";
        std::cout << "+" << std::string(182, '=') << "+\n";
    }

    static void runDurationSearchExperiment(const Array& originalArr, int threshold, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Visit Duration >= " + std::to_string(threshold) + " Hours)");

        printRow("Linear Search", "Array", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchDurationArray(originalArr, threshold);
        }));

        Array sortedArr(originalArr);
        MergeSort::sort(sortedArr, SortKey::VisitDuration);

        printRow("Linear Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return linearSearchDurationArray(sortedArr, threshold);
        }));
        printRow("Jump Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return jumpSearchDurationArray(sortedArr, threshold);
        }));
        std::cout << "+" << std::string(166, '=') << "+\n";
    }
};

#endif