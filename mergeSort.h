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
    static void sort(LinkedList& list, SortKey key) {
        list.head = sortList(list.head, key);
        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
        }
    }

    static void printPerformance(const LinkedList& list, const std::string& datasetName) {
        const SortKey keys[] = {
            SortKey::Age, SortKey::VisitDuration, SortKey::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(84, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(82)
                  << ("MERGE SORT PERFORMANCE: " + datasetName) << " |\n";
        std::cout << "+" << std::string(84, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(22) << "Sort Key"
                  << std::right << std::setw(20) << "Singly List (us)"
                  << std::setw(20) << "Time Complexity"
                  << std::setw(20) << "Auxiliary Memory" << " |\n";
        std::cout << "+" << std::string(84, '-') << "+\n";

        for (SortKey key : keys) {
            LinkedList listCopy(list);

            const auto listStart = std::chrono::high_resolution_clock::now();
            sort(listCopy, key);
            const auto listEnd = std::chrono::high_resolution_clock::now();

            const auto listTime = std::chrono::duration_cast<std::chrono::microseconds>(
                listEnd - listStart).count();

            std::cout << "| " << std::left << std::setw(22) << keyName(key)
                      << std::right << std::setw(20) << listTime
                      << std::setw(20) << "O(n log n)"
                      << std::setw(20) << "O(log n)" << " |\n";
        }

        std::cout << "+" << std::string(84, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(82)
                  << "Storage: Singly linked list O(n) plus one next pointer per node."
                  << " |\n";
        std::cout << "+" << std::string(84, '=') << "+\n";
    }
};

#endif