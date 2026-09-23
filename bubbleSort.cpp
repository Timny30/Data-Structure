#include "bubbleSort.h"
#include <chrono>
#include <iomanip>
#include <iostream>

namespace {
bool shouldSwap(const patientRecord& left, const patientRecord& right, SortField field) {
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

	int linkedListSize(const LinkedList& list) {
		int size = 0;
		for (node* current = list.head; current != nullptr; current = current->next) {
			++size;
		}
		return size;
	}
}

const char* BubbleSort::fieldName(SortField field) {
	switch (field) {
	case SortField::VisitDuration:
		return "Visit Duration";
	case SortField::TotalMedicalCost:
		return "Total Medical Cost";
	case SortField::Age:
	default:
		return "Age";
	}
}

void BubbleSort::printPerformance(const Array& sourceArray, const LinkedList& sourceList) {
	printPerformance(sourceArray, sourceList, "Dataset");
}

void BubbleSort::printPerformance(const Array& sourceArray, const LinkedList& sourceList,
	const std::string& facilityName) {
		const SortField fields[] = {
			SortField::Age,
			SortField::VisitDuration,
			SortField::TotalMedicalCost
		};

		std::cout << "\nSORTING PERFORMANCE (Bubble Sort) - " << facilityName << "\n";
		std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";
		std::cout << "| Sort Key            | Data Structure | Records         | Time (microsec)  | Time Complexity  |\n";
		std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";

		for (SortField field : fields) {
			Array array = sourceArray;
			auto arrayStart = std::chrono::high_resolution_clock::now();
			BubbleSort::sort(array, field);
			auto arrayEnd = std::chrono::high_resolution_clock::now();

			LinkedList list = sourceList;
			auto listStart = std::chrono::high_resolution_clock::now();
			BubbleSort::sort(list, field);
			auto listEnd = std::chrono::high_resolution_clock::now();

			const auto arrayTime = std::chrono::duration_cast<std::chrono::microseconds>(
				arrayEnd - arrayStart).count();
			const auto listTime = std::chrono::duration_cast<std::chrono::microseconds>(
				listEnd - listStart).count();

			std::cout << "| " << std::left << std::setw(20) << BubbleSort::fieldName(field)
				<< " | " << std::setw(13) << "Array"
				<< " | " << std::setw(15) << sourceArray.size
				<< " | " << std::setw(16) << arrayTime
				<< " | " << std::setw(16) << "O(n^2)"
				<< " |\n";
			std::cout << "| " << std::left << std::setw(20) << BubbleSort::fieldName(field)
				<< " | " << std::setw(13) << "Singly Linked"
				<< " | " << std::setw(15) << linkedListSize(sourceList)
				<< " | " << std::setw(16) << listTime
				<< " | " << std::setw(16) << "O(n^2)"
				<< " |\n";
		}
		std::cout << "+---------------------+---------------+------------------+------------------+------------------+\n";

		std::cout << "\nMEMORY AND COMPLEXITY COMPARISON\n";
		std::cout << "+---------------------+------------------+----------------------+-----------------------------+\n";
		std::cout << "| Data Structure      | Extra Sort Space | Approx. Storage      | Storage                     |\n";
		std::cout << "+---------------------+------------------+----------------------+-----------------------------+\n";
		std::cout << "| Array               | O(1)             | "
			<< sourceArray.size * sizeof(patientRecord)
			<< " bytes              | O(n) contiguous records       |\n";
		std::cout << "| Singly Linked List  | O(1)             | "
			<< linkedListSize(sourceList) * sizeof(node)
			<< " bytes              | O(n) records + one next ptr  |\n";
		std::cout << "+---------------------+------------------+----------------------+-----------------------------+\n";
		std::cout << "Both implementations use optimized Bubble Sort: O(n) best case and O(n^2) average/worst case.\n";
}

void BubbleSort::sort(Array& array) {
	sort(array, SortField::Age);
}

void BubbleSort::sort(LinkedList& list) {
	sort(list, SortField::Age);
}

void BubbleSort::sort(Array& array, SortField field) {
	for (int end = array.size - 1; end > 0; --end) {
		bool swapped = false;
		for (int i = 0; i < end; ++i) {
			if (shouldSwap(array.data[i], array.data[i + 1], field)) {
				patientRecord temporary = array.data[i];
				array.data[i] = array.data[i + 1];
				array.data[i + 1] = temporary;
				swapped = true;
			}
		}
		if (!swapped) {
			break;
		}
	}
}

void BubbleSort::sort(LinkedList& list, SortField field) {
    // If the list is empty or has only one element, it is already sorted
    if (list.head == nullptr || list.head->next == nullptr) {
        return;
    }

    // Dummy node guarantees that 'prev' always exists, simplifying head swaps
    node dummy(patientRecord{});
    dummy.next = list.head;

    node* lastSorted = nullptr;
    bool swapped;

    do {
        swapped = false;
        node* prev = &dummy;
        node* current = prev->next;

        // Traverse only up to the boundary of elements confirmed sorted in previous passes
        while (current->next != lastSorted) {
            node* nextNode = current->next;

            if (shouldSwap(current->data, nextNode->data, field)) {
                // 1. Relink pointers to swap 'current' and 'nextNode'
                current->next = nextNode->next;
                nextNode->next = current;
                prev->next = nextNode;

                // 2. 'nextNode' is now before 'current'; update prev to nextNode
                prev = nextNode;
                swapped = true;
            } else {
                // No swap needed: advance both pointers forward
                prev = current;
                current = current->next;
            }
        }

        // 'current' carried the largest unsorted element to the end of this pass
        lastSorted = current;
    } while (swapped);

    // Update list head to point to the actual first node
    list.head = dummy.next;

    // Resynchronize list tail pointer to maintain container integrity
    list.tail = list.head;
    while (list.tail != nullptr && list.tail->next != nullptr) {
        list.tail = list.tail->next;
    }
}
