#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include "patientRecord.h"
#include <algorithm>
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

    static node* mergeLists(node* left, node* right, SortKey key) {
        node dummy(patientRecord{});
        node* tail = &dummy;
        while (left != nullptr && right != nullptr) {
            if (comesBefore(right->data, left->data, key)) {
                tail->next = right;
                right = right->next;
            } else {
                tail->next = left;
                left = left->next;
            }
            tail = tail->next;
        }
        tail->next = (left != nullptr) ? left : right;
        return dummy.next;
    }

    static node* splitList(node* head) {
        node* slow = head;
        node* fast = head->next;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        node* second = slow->next;
        slow->next = nullptr;
        return second;
    }

    static node* sortList(node* head, SortKey key) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        node* second = splitList(head);
        head = sortList(head, key);
        second = sortList(second, key);
        return mergeLists(head, second, key);
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

    static void sort(LinkedList& list, SortKey key) {
        list.head = sortList(list.head, key);
        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
        }
    }

    static void printPerformance(const Array& array, const LinkedList& list,
                                 const std::string& datasetName) {
        const SortKey keys[] = {
            SortKey::Age, SortKey::VisitDuration, SortKey::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(116, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(114)
                  << ("MERGE SORT PERFORMANCE: " + datasetName) << "|\n";
        std::cout << "+" << std::string(116, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(22) << "Sort Key"
                  << std::right << std::setw(18) << "Array (us)"
                  << std::setw(20) << "Singly List (us)"
                  << std::setw(22) << "Time Complexity"
                  << std::setw(30) << "Auxiliary Memory" << "  |\n";
        std::cout << "+" << std::string(116, '-') << "+\n";

        for (SortKey key : keys) {
            Array arrayCopy(array);
            LinkedList listCopy(list);

            const auto arrayStart = std::chrono::high_resolution_clock::now();
            sort(arrayCopy, key);
            const auto arrayEnd = std::chrono::high_resolution_clock::now();
            const auto listStart = std::chrono::high_resolution_clock::now();
            sort(listCopy, key);
            const auto listEnd = std::chrono::high_resolution_clock::now();

            const auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(
                arrayEnd - arrayStart).count();
            const auto listTime = std::chrono::duration_cast<std::chrono::microseconds>(
                listEnd - listStart).count();

            std::cout << "| " << std::left << std::setw(22) << keyName(key)
                      << std::right << std::setw(18) << arrayTime
                      << std::setw(20) << listTime
                      << std::setw(22) << "O(n log n)"
                      << std::setw(30) << "Array: O(n), List: O(log n)" << "  |\n";
        }
        std::cout << "+" << std::string(116, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(114)
                  << "Storage: Array O(n) contiguous; singly linked list O(n) plus one next pointer per node."
                  << "|\n";
        std::cout << "+" << std::string(116, '=') << "+\n";
    }
};

#endif
