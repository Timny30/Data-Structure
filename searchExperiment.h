#ifndef SEARCH_EXPERIMENT_H
#define SEARCH_EXPERIMENT_H

#include "patientRecord.h"
#include "mergeSort.h"
#include "benchmarkStats.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>

// comparisons    = one logical evaluation of a patient record's relevant
//                  search key against the search criterion (Age/CareType/
//                  Duration). Index/pointer/nullptr/loop-boundary checks and
//                  step arithmetic are never counted here.
// recordAccesses = one patient-record position access performed while
//                  searching/navigating. For Array (direct indexing) this
//                  normally coincides 1:1 with comparisons; for a Linked
//                  List it also captures pure node-to-node traversal that
//                  performs no key comparison (see searchExperiment.h on the
//                  Linked List branch).
struct SearchResult {
    int matchesFound = 0;
    long long comparisons = 0;
    long long recordAccesses = 0;
    long long durationNanosec = 0;
    std::string memoryOverhead = "O(1)";
};

// Result of a repeated benchmark of one search configuration.
// result: the deterministic values (matches/comparisons/recordAccesses/
//         memoryOverhead) captured from the first measured run.
// timing: aggregated Median/Average/Min/Max statistics across all measured
//         runs (warm-up runs never contribute samples).
// metricsConsistent: false only if a later measured run's deterministic
//         values differed from the first run's - a correctness flag, not a
//         timing statistic.
struct RepeatedSearchOutcome {
    SearchResult result;
    BenchmarkStats timing;
    bool metricsConsistent = true;
};

class SearchExperiment {
private:
    // ==========================================
    // 1. AGE SEARCH LOGIC
    // ==========================================
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

    // Array direct indexing means every record examined is accessed exactly
    // once to make its comparison decision, so comparisons and recordAccesses
    // increment together throughout this function - there is no separate
    // "skipped element" traversal cost, unlike the Linked List equivalent.
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

    // ==========================================
    // 2. CARE TYPE SEARCH LOGIC (Linear Only)
    // ==========================================
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

    // ==========================================
    // 3. VISIT DURATION SEARCH LOGIC (Threshold)
    // ==========================================
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

    // ==========================================
    // UTILITY: Print Format
    // ==========================================
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

    // Runs searchFunc SEARCH_WARMUP_RUNS times (discarded) then
    // SEARCH_MEASURED_RUNS times (timed and validated). Deterministic values
    // (matches/comparisons/recordAccesses) are taken from the first measured
    // run and cross-checked against every subsequent measured run.
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

        // Sorted copy is created and sorted ONCE, before repeated-search
        // timing begins; sort cost stays excluded from search time.
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
        printHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");

        printRow("Linear Search", "Array", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchCareTypeArray(originalArr, targetType);
        }));

        std::cout << "+" << std::string(166, '-') << "+\n";
        std::cout << "| Note: Jump Search skipped because dataset is not sorted by non-numerical CareType string.     |\n";
        std::cout << "+" << std::string(166, '=') << "+\n";
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