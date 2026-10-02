#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "patientRecord.h"
#include "sortMetrics.h"
#include "mergeSort.h" // To reuse SortKey and comesBefore logic

class InsertionSort {
public:
    static SortMetrics sort(LinkedList& list, SortKey key) {
        SortMetrics metrics;
        if (list.head == nullptr || list.head->next == nullptr) return metrics;

        node* sortedHead = nullptr;
        node* current = list.head;

        while (current != nullptr) {
            node* nextNode = current->next; // Store next before rewiring current
            
            // Insert at head if sorted list is empty or current comes before sortedHead
            metrics.comparisons++;
            if (sortedHead == nullptr || comesBefore(current->data, sortedHead->data, key)) {
                current->next = sortedHead;
                sortedHead = current;
                metrics.dataMovements++;
            } else {
                // Locate the node before the point of insertion
                node* search = sortedHead;
                while (search->next != nullptr) {
                    metrics.comparisons++;
                    if (comesBefore(current->data, search->next->data, key)) {
                        break;
                    }
                    search = search->next;
                }
                current->next = search->next;
                search->next = current;
                metrics.dataMovements++;
            }
            current = nextNode;
        }

        list.head = sortedHead;
        
        // Repair the tail pointer
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