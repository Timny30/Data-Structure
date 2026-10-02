#ifndef SELECTION_SORT_ARRAY_H
#define SELECTION_SORT_ARRAY_H

#include "patientRecord.h"
#include "mergeSort.h"
#include "sortMetrics.h"

class SelectionSortArray {
public:
    static SortMetrics sort(Array& arr, SortKey key) {
        SortMetrics metrics;
        if (arr.size < 2) return metrics;

        for (int i = 0; i < arr.size - 1; ++i) {
            int minIndex = i;
            for (int j = i + 1; j < arr.size; ++j) {
                metrics.comparisons++;
                if (comesBefore(arr.data[j], arr.data[minIndex], key)) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                patientRecord temp = arr.data[i];
                arr.data[i] = arr.data[minIndex];
                arr.data[minIndex] = temp;
                metrics.dataMovements++;
            }
        }
        return metrics;
    }

private:
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