#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include "patientRecord.h"
#include "sortMetrics.h"
#include "mergeSort.h"

class SelectionSort {
public:
    static SortMetrics sort(LinkedList& list, SortKey key) {
        SortMetrics metrics;
        if (list.head == nullptr || list.head->next == nullptr) return metrics;

        node dummy(patientRecord{});
        dummy.next = list.head;
        
        node* sortedTail = &dummy;

        while (sortedTail->next != nullptr) {
            node* minPrev = sortedTail;
            node* minNode = sortedTail->next;
            node* currentPrev = sortedTail->next;
            node* current = currentPrev->next;

            // Scan remaining list for the minimum element
            while (current != nullptr) {
                metrics.comparisons++;
                if (comesBefore(current->data, minNode->data, key)) {
                    minPrev = currentPrev;
                    minNode = current;
                }
                currentPrev = current;
                current = current->next;
            }

            // Unlink minNode and append to the sorted portion
            if (minNode != sortedTail->next) {
                minPrev->next = minNode->next;
                minNode->next = sortedTail->next;
                sortedTail->next = minNode;
                metrics.dataMovements++;
            }
            sortedTail = sortedTail->next;
        }

        list.head = dummy.next;
        
        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
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