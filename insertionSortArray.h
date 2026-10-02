#ifndef INSERTION_SORT_ARRAY_H
#define INSERTION_SORT_ARRAY_H

#include "patientRecord.h"
#include "mergeSort.h"
#include "sortMetrics.h"

class InsertionSortArray {
public:
    static SortMetrics sort(Array& arr, SortKey key) {
        SortMetrics metrics;
        if (arr.size < 2) return metrics;

        for (int i = 1; i < arr.size; ++i) {
            patientRecord keyRecord = arr.data[i];
            int j = i - 1;
            
            while (j >= 0) {
                metrics.comparisons++;
                if (comesBefore(keyRecord, arr.data[j], key)) {
                    arr.data[j + 1] = arr.data[j];
                    metrics.dataMovements++;
                    j--;
                } else {
                    break;
                }
            }
            arr.data[j + 1] = keyRecord;
            metrics.dataMovements++;
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