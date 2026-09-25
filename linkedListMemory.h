#ifndef LINKED_LIST_MEMORY_H
#define LINKED_LIST_MEMORY_H

#include "patientRecord.h"
#include <iostream>
#include <iomanip>
#include <string>

// Estimated structural memory footprint for the custom LinkedList.
//
// Node count is obtained by traversing the list OUTSIDE any sorting/searching
// timer, purely for this memory report - never inside a benchmarked region.
//
// Estimated Structural Memory = sizeof(LinkedList) + nodeCount * sizeof(node)
// This is the authoritative total. "Patient Payload Storage" and "Next
// Pointer Overhead" below are explanatory components only; their sum may not
// exactly equal nodeCount * sizeof(node) if the compiler adds alignment
// padding inside node. Any such padding is reported separately below rather
// than hidden.
//
// This is a structural estimate based on sizeof(); dynamic/internal
// std::string storage (patientID, careType) and allocator overhead are not
// included, since sizeof() cannot see heap-allocated string payloads.
inline void printLinkedListMemoryFootprint(const LinkedList& list, const std::string& facilityName) {
    long long nodeCount = 0;
    for (node* current = list.head; current != nullptr; current = current->next) {
        ++nodeCount;
    }

    const size_t recordSize = sizeof(patientRecord);
    const size_t nodeSize = sizeof(node);
    const size_t listObjectSize = sizeof(LinkedList);
    const size_t pointerSize = sizeof(node*);

    const size_t structuralMemory = listObjectSize + static_cast<size_t>(nodeCount) * nodeSize;
    const size_t payloadStorage = static_cast<size_t>(nodeCount) * recordSize;
    const size_t pointerOverhead = static_cast<size_t>(nodeCount) * pointerSize;
    const size_t perNodePadding = (nodeSize > recordSize + pointerSize) ? (nodeSize - recordSize - pointerSize) : 0;
    const size_t alignmentPadding = static_cast<size_t>(nodeCount) * perNodePadding;

    std::cout << "\n+" << std::string(76, '=') << "+\n";
    std::cout << "| " << std::left << std::setw(72)
              << ("LINKED LIST MEMORY FOOTPRINT - " + facilityName) << " |\n";
    std::cout << "+" << std::string(76, '-') << "+\n";

    std::cout << "| " << std::left << std::setw(28) << "Records / Nodes"
              << std::right << std::setw(44) << nodeCount << " |\n";
    std::cout << "| " << std::left << std::setw(28) << "sizeof(patientRecord)"
              << std::right << std::setw(38) << recordSize << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "sizeof(node)"
              << std::right << std::setw(38) << nodeSize << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "sizeof(LinkedList)"
              << std::right << std::setw(38) << listObjectSize << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Estimated Structural Mem."
              << std::right << std::setw(38) << structuralMemory << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Patient Payload Storage"
              << std::right << std::setw(38) << payloadStorage << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Next Pointer Overhead"
              << std::right << std::setw(38) << pointerOverhead << " bytes |\n";
    if (alignmentPadding > 0) {
        std::cout << "| " << std::left << std::setw(28) << "Per-Node Alignment Padding"
                  << std::right << std::setw(38) << alignmentPadding << " bytes |\n";
    }
    std::cout << "+" << std::string(76, '-') << "+\n";
    std::cout << "| Note: structural estimate based on sizeof(); dynamic std::string\n";
    std::cout << "| heap storage and allocator overhead are not included.\n";
    std::cout << "+" << std::string(76, '=') << "+\n";
}

#endif
