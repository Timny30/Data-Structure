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
    // ==========================================
    // AGE SEARCH LOGIC
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
    // CARE TYPE SEARCH LOGIC (Linear Only)
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

    static void sortCareTypeListCopy(LinkedList& list) {
        if (list.head == nullptr) return;
        bool swapped;
        do {
            swapped = false;
            node* curr = list.head;
            while (curr->next != nullptr) {
                if (curr->data.careType > curr->next->data.careType) {
                    patientRecord temp = curr->data;
                    curr->data = curr->next->data;
                    curr->next->data = temp;
                    swapped = true;
                }
                curr = curr->next;
            }
        } while (swapped);
    }

    // ==========================================
    // VISIT DURATION SEARCH LOGIC (Threshold)
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
    // BINARY SEARCH (Linked List Adaptation)
    // ==========================================
    static SearchResult binarySearchAgeList(const LinkedList& list, int minAge, int maxAge) {
        SearchResult res;
        auto startClock = std::chrono::high_resolution_clock::now();

        node* start = list.head;
        node* end = nullptr;
        node* lowerBound = nullptr;

        while (start != end) {
            node* mid = getMiddle(start, end, res);
            if (mid == nullptr) break;

            res.comparisons++;
            res.recordAccesses++;
            if (mid->data.age >= minAge) {
                lowerBound = mid;
                end = mid;
            } else {
                start = mid->next;
            }
        }

        node* curr = lowerBound ? lowerBound : start;
        while (curr != nullptr && curr->data.age <= maxAge) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.age >= minAge) res.matchesFound++;
            curr = curr->next;
        }
        if (curr != nullptr) { res.comparisons++; res.recordAccesses++; }

        auto endClock = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(endClock - startClock).count();
        return res;
    }

    // ==========================================
    // EXPONENTIAL SEARCH (Linked List Adaptation)
    // ==========================================
    static SearchResult exponentialSearchAgeList(const LinkedList& list, int minAge, int maxAge) {
        SearchResult res;
        if (list.head == nullptr) return res;
        auto startClock = std::chrono::high_resolution_clock::now();

        int bound = 1;
        node* boundNode = list.head;
        node* prevNode = nullptr;

        while (boundNode != nullptr && boundNode->data.age < minAge) {
            res.comparisons++;
            res.recordAccesses++;
            prevNode = boundNode;

            for (int i = 0; i < bound && boundNode != nullptr; i++) {
                boundNode = boundNode->next;
                res.recordAccesses++;
            }
            bound *= 2;
        }
        if (boundNode != nullptr) { res.comparisons++; res.recordAccesses++; }

        node* start = prevNode ? prevNode : list.head;
        node* end = boundNode ? boundNode->next : nullptr;
        node* lowerBound = nullptr;

        while (start != end) {
            node* mid = getMiddle(start, end, res);
            if (mid == nullptr) break;

            res.comparisons++;
            res.recordAccesses++;
            if (mid->data.age >= minAge) {
                lowerBound = mid;
                end = mid;
            } else {
                start = mid->next;
            }
        }

        node* curr = lowerBound ? lowerBound : start;
        while (curr != nullptr && curr->data.age <= maxAge) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.age >= minAge) res.matchesFound++;
            curr = curr->next;
        }
        if (curr != nullptr) { res.comparisons++; res.recordAccesses++; }

        auto endClock = std::chrono::high_resolution_clock::now();
        res.durationNanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(endClock - startClock).count();
        return res;
    }

    // ==========================================
    // INTERPOLATION SEARCH (Linked List Adaptation)
    // ==========================================
    static SearchResult interpolationSearchAgeList(const LinkedList& list, int minAge, int maxAge) {
        SearchResult res;
        if (list.head == nullptr) return res;
        auto startClock = std::chrono::high_resolution_clock::now();

        int n = 0;
        node* counter = list.head;
        while (counter != nullptr) {
            n++;
            res.recordAccesses++;
            counter = counter->next;
        }

        int low = 0;
        int high = n - 1;
        node* lowNode = list.head;
        node* highNode = getNodeAtIndex(list.head, high, res);
        node* lowerBound = nullptr;

        while (low <= high && minAge >= lowNode->data.age && minAge <= highNode->data.age) {
            res.comparisons += 2;
            
            if (low == high) {
                if (lowNode->data.age >= minAge) lowerBound = lowNode;
                break;
            }

            double proportion = static_cast<double>(minAge - lowNode->data.age) / (highNode->data.age - lowNode->data.age);
            int pos = low + static_cast<int>(proportion * (high - low));

            node* posNode = getNodeAtIndex(list.head, pos, res);
            res.comparisons++;

            if (posNode->data.age >= minAge) {
                lowerBound = posNode;
                high = pos - 1;
                highNode = getNodeAtIndex(list.head, high, res);
            } else {
                low = pos + 1;
                lowNode = getNodeAtIndex(list.head, low, res);
            }
        }

        node* curr = lowerBound ? lowerBound : lowNode;
        while (curr != nullptr && curr->data.age <= maxAge) {
            res.comparisons++;
            res.recordAccesses++;
            if (curr->data.age >= minAge) res.matchesFound++;
            curr = curr->next;
        }
        if (curr != nullptr) { res.comparisons++; res.recordAccesses++; }

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

    static node* getMiddle(node* start, node* end, SearchResult& res) {
        if (start == nullptr) return nullptr;
        node* slow = start;
        node* fast = start->next;
        while (fast != end) {
            fast = fast->next;
            res.recordAccesses++;
            if (fast != end) {
                slow = slow->next;
                fast = fast->next;
                res.recordAccesses += 2;
            }
        }
        return slow;
    }

    static node* getNodeAtIndex(node* start, int index, SearchResult& res) {
        node* curr = start;
        for (int i = 0; i < index && curr != nullptr; i++) {
            curr = curr->next;
            res.recordAccesses++;
        }
        return curr;
    }

public:
    static void runAgeSearchExperiment(const LinkedList& originalList, int minAge, int maxAge, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Target Age: " + std::to_string(minAge) + " - " + std::to_string(maxAge) + ")");

        printRow("Linear Search", "Singly List", "Unsorted", runRepeatedSearch([&]() {
            return linearSearchAgeList(originalList, minAge, maxAge, false);
        }));

        LinkedList sortedList(originalList);
        MergeSort::sort(sortedList, SortKey::Age);

        printRow("Linear Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return linearSearchAgeList(sortedList, minAge, maxAge, true);
        }));
        
        printRow("Jump Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return jumpSearchAgeList(sortedList, minAge, maxAge);
        }));
        
        printRow("Binary Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return binarySearchAgeList(sortedList, minAge, maxAge);
        }));
        
        printRow("Exponen. Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return exponentialSearchAgeList(sortedList, minAge, maxAge);
        }));
        
        printRow("Interpol. Search", "Singly List", "Sorted", runRepeatedSearch([&]() {
            return interpolationSearchAgeList(sortedList, minAge, maxAge);
        }));

        std::cout << "+" << std::string(166, '=') << "+\n";
    }

    static void runCareTypeSearchExperiment(const LinkedList& originalList, const std::string& targetType, const std::string& facility) {
        printCareTypeHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");

        RepeatedSearchOutcome unsortedOutcome = runRepeatedSearch([&]() {
            return linearSearchCareTypeList(originalList, targetType);
        });

        unsortedOutcome.result.memoryBytes = BubbleSort::calculateMemoryUsage(originalList);
        printCareTypeRow("Linear Search", "Singly List", "Unsorted", unsortedOutcome);

        LinkedList careTypeSortedList(originalList);
        sortCareTypeListCopy(careTypeSortedList);

        RepeatedSearchOutcome sortedOutcome = runRepeatedSearch([&]() {
            return linearSearchCareTypeList(careTypeSortedList, targetType);
        });
        sortedOutcome.result.memoryBytes = BubbleSort::calculateMemoryUsage(careTypeSortedList);
        printCareTypeRow("Linear Search", "Singly List", "Sorted", sortedOutcome);

        std::cout << "+" << std::string(182, '-') << "+\n";
        std::cout << "| Note: Care Type experiment evaluates Linear Search only; Jump Search is\n";
        std::cout << "| not implemented for string-based Care Type searching.\n";
        std::cout << "+" << std::string(182, '=') << "+\n";
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