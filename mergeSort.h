#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "patientRecord.h"
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

    static void merge(patientRecord* data, patientRecord* buffer,
                      int first, int middle, int last, SortKey key) {
        int left = first;
        int right = middle + 1;
        int output = first;

        while (left <= middle && right <= last) {
            if (comesBefore(data[right], data[left], key)) {
                buffer[output++] = data[right++];
            } else {
                buffer[output++] = data[left++];
            }
        }
        while (left <= middle) {
            buffer[output++] = data[left++];
        }
        while (right <= last) {
            buffer[output++] = data[right++];
        }
        for (int i = first; i <= last; ++i) {
            data[i] = buffer[i];
        }
    }

    static void sortArray(patientRecord* data, patientRecord* buffer,
                          int first, int last, SortKey key) {
        if (first >= last) {
            return;
        }
        const int middle = first + (last - first) / 2;
        sortArray(data, buffer, first, middle, key);
        sortArray(data, buffer, middle + 1, last, key);
        merge(data, buffer, first, middle, last, key);
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
    static void sort(Array& array, SortKey key) {
        if (array.size < 2) {
            return;
        }
        patientRecord* buffer = new patientRecord[array.size];
        sortArray(array.data, buffer, 0, array.size - 1, key);
        delete[] buffer;
    }

    static void printPerformance(const Array& array, const std::string& datasetName) {
        const SortKey keys[] = {
            SortKey::Age, SortKey::VisitDuration, SortKey::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(86, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(84)
                  << ("MERGE SORT PERFORMANCE (Array): " + datasetName) << "|\n";
        std::cout << "+" << std::string(86, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(24) << "Sort Key"
                  << std::right << std::setw(18) << "Time (us)"
                  << std::setw(20) << "Time Complexity"
                  << std::setw(20) << "Auxiliary Memory" << " |\n";
        std::cout << "+" << std::string(86, '-') << "+\n";

        for (SortKey key : keys) {
            Array arrayCopy(array);

            const auto start = std::chrono::high_resolution_clock::now();
            sort(arrayCopy, key);
            const auto end = std::chrono::high_resolution_clock::now();

            const auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(
                end - start).count();

            std::cout << "| " << std::left << std::setw(24) << keyName(key)
                      << std::right << std::setw(18) << arrayTime
                      << std::setw(20) << "O(n log n)"
                      << std::setw(20) << "O(n)" << " |\n";
        }
        std::cout << "+" << std::string(86, '=') << "+\n";
    }
};

#endif