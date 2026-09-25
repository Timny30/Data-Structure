#ifndef ARRAY_MEMORY_H
#define ARRAY_MEMORY_H

#include "patientRecord.h"
#include <iostream>
#include <iomanip>
#include <string>

// Estimated structural memory footprint for the custom Array.
//
// This is a structural estimate based on sizeof() only: it accounts for the
// Array object itself plus its raw patientRecord backing store (split into
// live vs unused capacity). It does NOT represent exact total process memory,
// because patientRecord contains std::string fields (patientID, careType)
// whose dynamic/internal heap storage and allocator overhead are not visible
// to sizeof() and are therefore not included here.
//
// (capacity - size) can never be negative: Array only grows its capacity
// (never shrinks it), so capacity >= size is always guaranteed by push_back().
inline void printArrayMemoryFootprint(const Array& array, const std::string& facilityName) {
    const size_t recordSize = sizeof(patientRecord);
    const size_t structuralMemory = sizeof(Array) + static_cast<size_t>(array.capacity) * recordSize;
    const size_t liveRecordStorage = static_cast<size_t>(array.size) * recordSize;
    const size_t unusedCapacityStorage = static_cast<size_t>(array.capacity - array.size) * recordSize;

    std::cout << "\n+" << std::string(76, '=') << "+\n";
    std::cout << "| " << std::left << std::setw(72)
              << ("ARRAY MEMORY FOOTPRINT - " + facilityName) << " |\n";
    std::cout << "+" << std::string(76, '-') << "+\n";

    std::cout << "| " << std::left << std::setw(28) << "Records"
              << std::right << std::setw(44) << array.size << " |\n";
    std::cout << "| " << std::left << std::setw(28) << "Capacity"
              << std::right << std::setw(44) << array.capacity << " |\n";
    std::cout << "| " << std::left << std::setw(28) << "sizeof(patientRecord)"
              << std::right << std::setw(38) << recordSize << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Estimated Structural Mem."
              << std::right << std::setw(38) << structuralMemory << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Live Record Storage"
              << std::right << std::setw(38) << liveRecordStorage << " bytes |\n";
    std::cout << "| " << std::left << std::setw(28) << "Unused Capacity Storage"
              << std::right << std::setw(38) << unusedCapacityStorage << " bytes |\n";
    std::cout << "+" << std::string(76, '-') << "+\n";
    std::cout << "| Note: structural estimate based on sizeof(); dynamic std::string\n";
    std::cout << "| heap storage and allocator overhead are not included.\n";
    std::cout << "+" << std::string(76, '=') << "+\n";
}

#endif
