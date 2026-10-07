#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include "patientRecord.h"
#include "sortMetrics.h"
#include "mergeSort.h"

class QuickSort {
public:
    static SortMetrics sort(LinkedList& list, SortKey key) {
        SortMetrics metrics;
        list.head = quickSortRecursive(list.head, key, metrics);
        
        list.tail = list.head;
        if (list.tail) {
            while (list.tail->next != nullptr) {
                list.tail = list.tail->next;
            }
        }
        return metrics;
    }

private:
    static node* quickSortRecursive(node* head, SortKey key, SortMetrics& metrics) {
        if (head == nullptr || head->next == nullptr) return head;

        node* pivot = head;
        node* current = head->next;
        
        node* lessHead = nullptr;
        node* lessTail = nullptr;
        node* greaterHead = nullptr;
        node* greaterTail = nullptr;

        pivot->next = nullptr;

        while (current != nullptr) {
            node* nextNode = current->next;
            current->next = nullptr;
            metrics.comparisons++;

            if (comesBefore(current->data, pivot->data, key)) {
                if (!lessHead) lessHead = lessTail = current;
                else { lessTail->next = current; lessTail = current; }
                metrics.dataMovements++;
            } else {
                if (!greaterHead) greaterHead = greaterTail = current;
                else { greaterTail->next = current; greaterTail = current; }
                metrics.dataMovements++;
            }
            current = nextNode;
        }

        lessHead = quickSortRecursive(lessHead, key, metrics);
        greaterHead = quickSortRecursive(greaterHead, key, metrics);

        if (lessHead != nullptr) {
            node* temp = lessHead;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = pivot;
        } else {
            lessHead = pivot;
        }
        pivot->next = greaterHead;

        return lessHead;
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