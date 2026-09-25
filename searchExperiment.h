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
//                  Duration). Pointer/nullptr/loop-boundary checks, pure
//                  node traversal (current = current->next), and step
//                  arithmetic are never counted here.
// recordAccesses = one patient-record/node position access performed while
//                  searching/navigating. This exists specifically to expose
//                  the Linked List's node-by-node traversal cost (including
//                  jump-simulation hops and the O(n) length-discovery walk)
//                  that has no equivalent cost on the Array side, where
//                  direct indexing keeps recordAccesses equal to comparisons.
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

    static SearchResult linearSearchAgeList(const LinkedList& list, int minAge, int maxAge, bool isSorted) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        node* curr = list.head;
        while (curr != nullptr) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.age >= minAge && curr->data.age <= maxAge) res.matchesFound++;
            else if (isSorted && curr->data.age > maxAge) break;
            curr = curr->next;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }


    // Counting rule for this Linked List Jump Search:
    // - comparisons increments ONLY where a node's patient key (data.age) is
    //   actually evaluated against minAge/maxAge to make an algorithmic
    //   decision (the boundary checks and the final linear-scan check).
    // - recordAccesses increments for every node position the algorithm
    //   physically visits: (1) the length-discovery traversal below (the
    //   list has no O(1) size field, so this O(n) walk is genuine, required
    //   work for this implementation and stays inside the timer), (2) every
    //   node stepped over inside advanceStep while simulating a "jump" (the
    //   list cannot random-access a target index - it must walk node by
    //   node), and (3) every node whose key is read for a comparison above.
    // No extra traversal is added beyond what the algorithm already performs.
    static SearchResult jumpSearchAgeList(const LinkedList& list, int minAge, int maxAge) {
        SearchResult res;
        if (list.head == nullptr) return res;
        auto start = std::chrono::high_resolution_clock::now();

        int n = 0;
        node* counter = list.head;
        while (counter) { n++; res.recordAccesses++; counter = counter->next; }

        int step = std::sqrt(n);
        node* prevNode = list.head;
        node* stepNode = list.head;

        auto advanceStep = [&](node* current, int steps) {
            for (int i = 0; i < steps && current != nullptr; i++) {
                current = current->next;
                res.recordAccesses++;
            }
            return current;
        };

        stepNode = advanceStep(stepNode, step - 1);

        while (stepNode != nullptr && stepNode->data.age < minAge) {
            res.comparisons++;
            res.recordAccesses++;
            prevNode = stepNode->next;
            stepNode = advanceStep(stepNode, step);
        }
        if (stepNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        while (prevNode != nullptr && prevNode->data.age < minAge) {
            res.comparisons++;
            res.recordAccesses++;
            prevNode = prevNode->next;
        }
        if (prevNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        while (prevNode != nullptr && prevNode->data.age <= maxAge) {
            res.comparisons++;
            res.recordAccesses++;
            if (prevNode->data.age >= minAge) res.matchesFound++;
            prevNode = prevNode->next;
        }
        if (prevNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }


    // ==========================================
    // 2. CARE TYPE SEARCH LOGIC (Linear Only)
    // ==========================================

    static SearchResult linearSearchCareTypeList(const LinkedList& list, const std::string& targetType) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        node* curr = list.head;
        while (curr != nullptr) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.careType == targetType) res.matchesFound++;
            curr = curr->next;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }

    // ==========================================
    // 3. VISIT DURATION SEARCH LOGIC (Threshold)
    // ==========================================

    static SearchResult linearSearchDurationList(const LinkedList& list, int threshold) {
        SearchResult res;
        auto start = std::chrono::high_resolution_clock::now();
        node* curr = list.head;
        while (curr != nullptr) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.lengthOfStay >= threshold) res.matchesFound++;
            curr = curr->next;
        }
        auto end = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        return res;
    }


    // Same counting rule as jumpSearchAgeList: comparisons only where
    // data.lengthOfStay is evaluated against threshold; recordAccesses for
    // every node position physically visited (length discovery, jump-step
    // traversal, and evaluated boundary/scan nodes). See jumpSearchAgeList
    // for the full rationale.
    static SearchResult jumpSearchDurationList(const LinkedList& list, int threshold) {
        SearchResult res;
        if (list.head == nullptr) return res;
        auto start = std::chrono::high_resolution_clock::now();

        int n = 0;
        node* counter = list.head;
        while (counter) { n++; res.recordAccesses++; counter = counter->next; }

        int step = std::sqrt(n);
        node* prevNode = list.head;
        node* stepNode = list.head;

        auto advanceStep = [&](node* current, int steps) {
            for (int i = 0; i < steps && current != nullptr; i++) {
                current = current->next;
                res.recordAccesses++;
            }
            return current;
        };

        stepNode = advanceStep(stepNode, step - 1);

        while (stepNode != nullptr && stepNode->data.lengthOfStay < threshold) {
            res.comparisons++;
            res.recordAccesses++;
            prevNode = stepNode->next;
            stepNode = advanceStep(stepNode, step);
        }
        if (stepNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        while (prevNode != nullptr && prevNode->data.lengthOfStay < threshold) {
            res.comparisons++;
            res.recordAccesses++;
            prevNode = prevNode->next;
        }
        if (prevNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        while (prevNode != nullptr) {
            res.comparisons++;
            res.recordAccesses++;
            if (prevNode->data.lengthOfStay >= threshold) res.matchesFound++;
            prevNode = prevNode->next;
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
    // run and cross-checked against every subsequent measured run. For Jump
    // Search, searchFunc recomputes n from scratch every single call (no
    // caching between runs), so the length-discovery traversal's real O(n)
    // cost is paid, and timed, on every warm-up and measured invocation.
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
    static void runAgeSearchExperiment(const LinkedList& originalList, int minAge, int maxAge, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Target Age: " + std::to_string(minAge) + " - " + std::to_string(maxAge) + ")");

        printRow("Linear Search", "Singly List", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchAgeList(originalList, minAge, maxAge, false);
        }));

        // Sorted copy is created and sorted ONCE, before repeated-search
        // timing begins; sort cost stays excluded from search time.
        LinkedList sortedList(originalList);
        MergeSort::sort(sortedList, SortKey::Age);

        printRow("Linear Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return linearSearchAgeList(sortedList, minAge, maxAge, true);
        }));
        printRow("Jump Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return jumpSearchAgeList(sortedList, minAge, maxAge);
        }));
        std::cout << "+" << std::string(166, '=') << "+\n";
    }

    static void runCareTypeSearchExperiment(const LinkedList& originalList, const std::string& targetType, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");

        printRow("Linear Search", "Singly List", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchCareTypeList(originalList, targetType);
        }));

        std::cout << "+" << std::string(166, '-') << "+\n";
        std::cout << "| Note: Jump Search skipped because dataset is not sorted by non-numerical CareType string.     |\n";
        std::cout << "+" << std::string(166, '=') << "+\n";
    }

    static void runDurationSearchExperiment(const LinkedList& originalList, int threshold, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Visit Duration >= " + std::to_string(threshold) + " Hours)");

        printRow("Linear Search", "Singly List", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchDurationList(originalList, threshold);
        }));

        LinkedList sortedList(originalList);
        MergeSort::sort(sortedList, SortKey::VisitDuration);

        printRow("Linear Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return linearSearchDurationList(sortedList, threshold);
        }));
        printRow("Jump Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return jumpSearchDurationList(sortedList, threshold);
        }));
        std::cout << "+" << std::string(166, '=') << "+\n";
    }
};

#endif