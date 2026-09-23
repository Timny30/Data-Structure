#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include "patientRecord.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

enum class SortField {
    Age,
    VisitDuration,
    TotalMedicalCost
};

class BubbleSort {
private:
    static bool shouldSwap(const patientRecord& left, const patientRecord& right, SortField field) {
        switch (field) {
        case SortField::VisitDuration:
            return left.lengthOfStay > right.lengthOfStay;
        case SortField::TotalMedicalCost:
            return left.calculateTotalCost() > right.calculateTotalCost();
        case SortField::Age:
        default:
            return left.age > right.age;
        }
    }

    static int linkedListSize(const LinkedList& list) {
        int size = 0;
        for (node* current = list.head; current != nullptr; current = current->next) {
            ++size;
        }
        return size;
    }

    static const char* fieldName(SortField field) {
        switch (field) {
        case SortField::VisitDuration: return "Visit Duration";
        case SortField::TotalMedicalCost: return "Total Medical Cost";
        case SortField::Age:
        default: return "Age";
        }
    }

public:
    static void sort(LinkedList& list) {
        sort(list, SortField::Age);
    }

    static void sort(LinkedList& list, SortField field) {
        if (list.head == nullptr || list.head->next == nullptr) {
            return;
        }

        node dummy(patientRecord{});
        dummy.next = list.head;

        node* lastSorted = nullptr;
        bool swapped;

        do {
            swapped = false;
            node* prev = &dummy;
            node* current = prev->next;

            while (current->next != lastSorted) {
                node* nextNode = current->next;

                if (shouldSwap(current->data, nextNode->data, field)) {
                    current->next = nextNode->next;
                    nextNode->next = current;
                    prev->next = nextNode;

                    prev = nextNode;
                    swapped = true;
                } else {
                    prev = current;
                    current = current->next;
                }
            }

            lastSorted = current;
        } while (swapped);

        list.head = dummy.next;

        list.tail = list.head;
        while (list.tail != nullptr && list.tail->next != nullptr) {
            list.tail = list.tail->next;
        }
    }

    static void printPerformance(const LinkedList& sourceList) {
        printPerformance(sourceList, "Dataset");
    }

    static void printPerformance(const LinkedList& sourceList, const std::string& facilityName) {
        const SortField fields[] = {
            SortField::Age,
            SortField::VisitDuration,
            SortField::TotalMedicalCost
        };

        std::cout << "\n+" << std::string(84, '=') << "+\n";
        std::cout << "| " << std::left << std::setw(82)
                  << ("BUBBLE SORT PERFORMANCE: " + facilityName) << " |\n";
        std::cout << "+" << std::string(84, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(22) << "Sort Key"
                  << std::right << std::setw(20) << "Singly List (us)"
                  << std::setw(20) << "Time Complexity"
                  << std::setw(20) << "Auxiliary Memory" << " |\n";
        std::cout << "+" << std::string(84, '-') << "+\n";

        for (SortField field : fields) {
            LinkedList listCopy = sourceList;

            const auto listStart = std::chrono::high_resolution_clock::now();
            sort(listCopy, field);
            const auto listEnd = std::chrono::high_resolution_clock::now();

            const auto listTime = std::chrono::duration_cast<std::chrono::microseconds>(
                listEnd - listStart).count();

            std::cout << "| " << std::left << std::setw(22) << fieldName(field)
                      << std::right << std::setw(20) << listTime
                      << std::setw(20) << "O(n^2)"
                      << std::setw(20) << "O(1)" << " |\n";
        }

        std::cout << "+" << std::string(84, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(82)
                  << "Storage: Singly linked list O(n) plus one next pointer per node."
                  << " |\n";
        std::cout << "+" << std::string(84, '=') << "+\n";
    }
};

#endif