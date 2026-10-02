#ifndef QUICK_SORT_ARRAY_H
#define QUICK_SORT_ARRAY_H

#include "patientRecord.h"
#include "mergeSort.h"
#include "sortMetrics.h"

class QuickSortArray {
public:
    static SortMetrics sort(Array& arr, SortKey key) {
        SortMetrics metrics;
        if (arr.size < 2) return metrics;
        quickSortRecursive(arr.data, 0, arr.size - 1, key, metrics);
        return metrics;
    }

private:
    static void quickSortRecursive(patientRecord* data, int low, int high, SortKey key, SortMetrics& metrics) {
        if (low < high) {
            int partitionIndex = partition(data, low, high, key, metrics);
            quickSortRecursive(data, low, partitionIndex - 1, key, metrics);
            quickSortRecursive(data, partitionIndex + 1, high, key, metrics);
        }
    }

    static int partition(patientRecord* data, int low, int high, SortKey key, SortMetrics& metrics) {
        patientRecord pivot = data[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            metrics.comparisons++;
            if (comesBefore(data[j], pivot, key)) {
                i++;
                patientRecord temp = data[i];
                data[i] = data[j];
                data[j] = temp;
                metrics.dataMovements++;
            }
        }
        patientRecord temp = data[i + 1];
        data[i + 1] = data[high];
        data[high] = temp;
        metrics.dataMovements++;
        
        return i + 1;
    }

    static bool comesBefore(const patientRecord& left, const patientRecord& right, SortKey key) {
        switch (key) {
            case SortKey::Age: return left.age < right.age;
            case SortKey::VisitDuration: return left.lengthOfStay < right.lengthOfStay;
            case SortKey::TotalMedicalCost: return left.calculateTotalCost() < right.calculateTotalCost();
        }
        return false;
    }
};
#endif