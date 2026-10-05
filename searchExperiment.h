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

struct SearchResult {
    int matchesFound = 0;
    long long comparisons = 0;
    long long recordAccesses = 0;
    long long durationNanosec = 0;
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

    // Dataset PREPARATION only for the Care Type unsorted-vs-sorted Linear
    // Search experiment - not a measured/benchmarked sorting algorithm, not
    // counted anywhere, and never timed. Sorts a copy's records ascending by
    // careType (lexicographical) in place on the raw backing array.
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

    static SearchResult binarySearchAgeArray(const Array& arr, int minAge, int maxAge) {
        SearchResult res;
        if (arr.size == 0) return res;
        auto startClock = std::chrono::high_resolution_clock::now();

        int low = 0;
        int high = arr.size - 1;
        int lowerBound = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            res.comparisons++;
            if (arr.data[mid].age >= minAge) {
                lowerBound = mid;
                high = mid - 1; 
            } else {
                low = mid + 1;
            }
        }

        int curr = (lowerBound != -1) ? lowerBound : low;
        while (curr < arr.size && arr.data[curr].age <= maxAge) {
            res.comparisons++;
            if (arr.data[curr].age >= minAge) res.matchesFound++;
            curr++;
        }
        if (curr < arr.size) res.comparisons++;

        auto endClock = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(endClock - startClock).count();
        return res;
    }

    static SearchResult exponentialSearchAgeArray(const Array& arr, int minAge, int maxAge) {
        SearchResult res;
        if (arr.size == 0) return res;
        auto startClock = std::chrono::high_resolution_clock::now();

        int bound = 1;
        
        while (bound < arr.size && arr.data[bound].age < minAge) {
            res.comparisons++;
            bound *= 2;
        }
        if (bound < arr.size) res.comparisons++;

        int low = bound / 2;
        int high = std::min(bound, arr.size - 1);
        int lowerBound = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            res.comparisons++;
            if (arr.data[mid].age >= minAge) {
                lowerBound = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        int curr = (lowerBound != -1) ? lowerBound : low;
        while (curr < arr.size && arr.data[curr].age <= maxAge) {
            res.comparisons++;
            if (arr.data[curr].age >= minAge) res.matchesFound++;
            curr++;
        }
        if (curr < arr.size) res.comparisons++;

        auto endClock = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(endClock - startClock).count();
        return res;
    }

    static SearchResult interpolationSearchAgeArray(const Array& arr, int minAge, int maxAge) {
        SearchResult res;
        if (arr.size == 0) return res;
        auto startClock = std::chrono::high_resolution_clock::now();

        int low = 0;
        int high = arr.size - 1;
        int lowerBound = -1;

        while (low <= high && minAge >= arr.data[low].age && minAge <= arr.data[high].age) {
            res.comparisons += 2; 
            
            if (low == high) {
                if (arr.data[low].age >= minAge) lowerBound = low;
                break;
            }

            double proportion = static_cast<double>(minAge - arr.data[low].age) / (arr.data[high].age - arr.data[low].age);
            int pos = low + static_cast<int>(proportion * (high - low));

            res.comparisons++;
            if (arr.data[pos].age >= minAge) {
                lowerBound = pos;
                high = pos - 1; 
            } else {
                low = pos + 1;
            }
        }

        int curr = (lowerBound != -1) ? lowerBound : low;
        while (curr < arr.size && arr.data[curr].age <= maxAge) {
            res.comparisons++;
            if (arr.data[curr].age >= minAge) res.matchesFound++;
            curr++;
        }
        if (curr < arr.size) res.comparisons++;

        auto endClock = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(endClock - startClock).count();
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
        
        printRow("Binary Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return binarySearchAgeArray(sortedArr, minAge, maxAge);
        }));
        
        printRow("Exponen. Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return exponentialSearchAgeArray(sortedArr, minAge, maxAge);
        }));
        
        printRow("Interpol. Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return interpolationSearchAgeArray(sortedArr, minAge, maxAge);
        }));
        
        std::cout << "+" << std::string(166, '=') << "+\n";
    }

    static void runCareTypeSearchExperiment(const Array& originalArr, const std::string& targetType, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");

        printRow("Linear Search", "Array", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchCareTypeArray(originalArr, targetType);
        }));

        // Sorted-by-careType copy is prepared ONCE, before repeated-search
        // timing begins; preparation cost stays excluded from search time.
        // The same linearSearchCareTypeArray function (no early-termination,
        // no changed comparison/access definitions) is reused unmodified -
        // this experiment isolates the effect of data ordering alone.
        Array careTypeSortedArr(originalArr);
        sortCareTypeArrayCopy(careTypeSortedArr);

        printRow("Linear Search", "Array", "Sorted", runRepeatedSearch([&]() {
            return linearSearchCareTypeArray(careTypeSortedArr, targetType);
        }));

        std::cout << "+" << std::string(166, '-') << "+\n";
        std::cout << "| Note: Care Type experiment evaluates Linear Search only; Jump Search is\n";
        std::cout << "| not implemented for string-based Care Type searching.\n";
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