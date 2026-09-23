#ifndef SEARCH_EXPERIMENT_H
#define SEARCH_EXPERIMENT_H

#include "patientRecord.h"
#include "mergeSort.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>

struct SearchResult {
    int matchesFound = 0;
    long long comparisons = 0;
    long long durationNanosec = 0;
    std::string memoryOverhead = "O(1)";
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
            prev = step;
            step += std::sqrt(n);
            if (prev >= n) break;
        }
        if (prev < n) res.comparisons++;

        while (prev < std::min(step, n) && arr.data[prev].age < minAge) {
            res.comparisons++;
            prev++;
        }
        if (prev < std::min(step, n)) res.comparisons++; 

        while (prev < n && arr.data[prev].age <= maxAge) {
            res.comparisons++;
            if (arr.data[prev].age >= minAge) res.matchesFound++;
            prev++;
        }
        if (prev < n) res.comparisons++;

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
            prev = step;
            step += std::sqrt(n);
            if (prev >= n) break;
        }
        if (prev < n) res.comparisons++;

        while (prev < std::min(step, n) && arr.data[prev].lengthOfStay < threshold) {
            res.comparisons++;
            prev++;
        }
        if (prev < std::min(step, n)) res.comparisons++;

        while (prev < n) {
            res.comparisons++;
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
        std::cout << "\n+" << std::string(93, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(91) << title << " |\n";
        std::cout << "+" << std::string(93, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(15) << "Algorithm" 
                  << "| " << std::setw(15) << "Data Struct" 
                  << "| " << std::setw(10) << "State" 
                  << "| " << std::setw(8) << "Matches" 
                  << "| " << std::setw(12) << "Comparisons" 
                  << "| " << std::setw(12) << "Time (ns)" 
                  << "| " << std::setw(12) << "Mem Ovhd" << " |\n";
        std::cout << "+" << std::string(93, '-') << "+\n";
    }

    static void printRow(const std::string& algo, const std::string& ds, const std::string& state, SearchResult res) {
        std::cout << "| " << std::left << std::setw(15) << algo 
                  << "| " << std::setw(15) << ds 
                  << "| " << std::setw(10) << state 
                  << "| " << std::right << std::setw(8) << res.matchesFound 
                  << "| " << std::setw(12) << res.comparisons 
                  << "| " << std::setw(12) << res.durationNanosec 
                  << "| " << std::setw(12) << res.memoryOverhead << " |\n";
    }

public:
    static void runAgeSearchExperiment(const Array& originalArr, int minAge, int maxAge, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Target Age: " + std::to_string(minAge) + " - " + std::to_string(maxAge) + ")");
        
        printRow("Linear Search", "Array", "Unsorted", linearSearchAgeArray(originalArr, minAge, maxAge, false));

        Array sortedArr(originalArr);
        MergeSort::sort(sortedArr, SortKey::Age);

        printRow("Linear Search", "Array", "Sorted", linearSearchAgeArray(sortedArr, minAge, maxAge, true));
        printRow("Jump Search", "Array", "Sorted", jumpSearchAgeArray(sortedArr, minAge, maxAge));
        std::cout << "+" << std::string(93, '=') << "+\n";
    }

    static void runCareTypeSearchExperiment(const Array& originalArr, const std::string& targetType, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Care Type: " + targetType + ")");
        
        printRow("Linear Search", "Array", "Unsorted", linearSearchCareTypeArray(originalArr, targetType));
        
        std::cout << "+" << std::string(93, '-') << "+\n";
        std::cout << "| Note: Jump Search skipped because dataset is not sorted by non-numerical CareType string.     |\n";
        std::cout << "+" << std::string(93, '=') << "+\n";
    }

    static void runDurationSearchExperiment(const Array& originalArr, int threshold, const std::string& facility) {
        printHeader("SEARCH EXPERIMENT: " + facility + " (Visit Duration >= " + std::to_string(threshold) + " Hours)");
        
        printRow("Linear Search", "Array", "Unsorted", linearSearchDurationArray(originalArr, threshold));

        Array sortedArr(originalArr);
        MergeSort::sort(sortedArr, SortKey::VisitDuration);

        printRow("Linear Search", "Array", "Sorted", linearSearchDurationArray(sortedArr, threshold));
        printRow("Jump Search", "Array", "Sorted", jumpSearchDurationArray(sortedArr, threshold));
        std::cout << "+" << std::string(93, '=') << "+\n";
    }
};

#endif