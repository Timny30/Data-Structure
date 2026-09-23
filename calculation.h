#ifndef CALCULATION_H
#define CALCULATION_H

#include "patientRecord.h"
#include <iostream>
#include <iomanip>
#include <string>

// Maximum age demographic brackets
const int NUM_AGE_GROUPS = 6;

// Metrics for an individual demographic bracket
struct AgeGroupMetrics {
    std::string groupName;
    int patientCount = 0;
    double totalCost = 0.0;
    double averageCost = 0.0;
    std::string preferredCareType = "N/A";
};

// Summary report across all age brackets for a facility
struct DatasetSummary {
    std::string facilityName;
    int totalPatients = 0;
    double overallTotalCost = 0.0;
    double overallAverageCost = 0.0;
    AgeGroupMetrics ageGroups[NUM_AGE_GROUPS];
};

// =============================================================================
// Custom Node-Based Frequency Tracker (Replaces std::map for Care Type Counting)
// =============================================================================
struct CareTypeFreqNode {
    std::string careType;
    int count;
    CareTypeFreqNode* next;
    CareTypeFreqNode(const std::string& type) : careType(type), count(1), next(nullptr) {}
};

class CareTypeTracker {
private:
    CareTypeFreqNode* head;

public:
    CareTypeTracker() : head(nullptr) {}

    ~CareTypeTracker() {
        clear();
    }

    void clear() {
        CareTypeFreqNode* curr = head;
        while (curr != nullptr) {
            CareTypeFreqNode* temp = curr->next;
            delete curr;
            curr = temp;
        }
        head = nullptr;
    }

    void record(const std::string& type) {
        if (type.empty()) return;
        CareTypeFreqNode* curr = head;
        while (curr != nullptr) {
            if (curr->careType == type) {
                curr->count++;
                return;
            }
            curr = curr->next;
        }
        // If not found, insert new node at the front
        CareTypeFreqNode* newNode = new CareTypeFreqNode(type);
        newNode->next = head;
        head = newNode;
    }

    std::string getMostPreferred() const {
        if (head == nullptr) return "N/A";
        CareTypeFreqNode* bestNode = head;
        CareTypeFreqNode* curr = head->next;
        while (curr != nullptr) {
            if (curr->count > bestNode->count) {
                bestNode = curr;
            }
            curr = curr->next;
        }
        return bestNode->careType;
    }
};

// Map patient age to index [0 - 5]
inline int mapAgeToGroupIndex(int age) {
    if (age >= 0 && age <= 17)   return 0;
    if (age >= 18 && age <= 25)  return 1;
    if (age >= 26 && age <= 45)  return 2;
    if (age >= 46 && age <= 60)  return 3;
    if (age >= 61 && age <= 100) return 4;
    return 5;
}

inline void initSummaryGroups(DatasetSummary& summary) {
    summary.ageGroups[0].groupName = "0-17: Pediatrics & Adolescents";
    summary.ageGroups[1].groupName = "18-25: Young Adults / Students";
    summary.ageGroups[2].groupName = "26-45: Working Adults (Early)";
    summary.ageGroups[3].groupName = "46-60: Working Adults (Late)";
    summary.ageGroups[4].groupName = "61-100: Senior / Geriatric Care";
    summary.ageGroups[5].groupName = "Unknown Age Group";
}

// =============================================================================
// Analysis Implementations
// =============================================================================

// 1. Analyze using custom Array
inline DatasetSummary analyzeDataset(const Array& arr, const std::string& facilityName) {
    DatasetSummary summary;
    summary.facilityName = facilityName;
    initSummaryGroups(summary);

    CareTypeTracker trackers[NUM_AGE_GROUPS];

    for (int i = 0; i < arr.size; ++i) {
        const patientRecord& record = arr.data[i];
        int idx = mapAgeToGroupIndex(record.age);
        double cost = record.calculateTotalCost();

        summary.ageGroups[idx].patientCount++;
        summary.ageGroups[idx].totalCost += cost;
        trackers[idx].record(record.careType);

        summary.totalPatients++;
        summary.overallTotalCost += cost;
    }

    // Finalize averages and preferred care type
    for (int i = 0; i < NUM_AGE_GROUPS; ++i) {
        if (summary.ageGroups[i].patientCount > 0) {
            summary.ageGroups[i].averageCost = summary.ageGroups[i].totalCost / summary.ageGroups[i].patientCount;
            summary.ageGroups[i].preferredCareType = trackers[i].getMostPreferred();
        }
    }

    summary.overallAverageCost = (summary.totalPatients > 0) ? (summary.overallTotalCost / summary.totalPatients) : 0.0;
    return summary;
}

// 2. Analyze using custom LinkedList
inline DatasetSummary analyzeDataset(const LinkedList& list, const std::string& facilityName) {
    DatasetSummary summary;
    summary.facilityName = facilityName;
    initSummaryGroups(summary);

    CareTypeTracker trackers[NUM_AGE_GROUPS];

    node* curr = list.head;
    while (curr != nullptr) {
        const patientRecord& record = curr->data;
        int idx = mapAgeToGroupIndex(record.age);
        double cost = record.calculateTotalCost();

        summary.ageGroups[idx].patientCount++;
        summary.ageGroups[idx].totalCost += cost;
        trackers[idx].record(record.careType);

        summary.totalPatients++;
        summary.overallTotalCost += cost;
        curr = curr->next;
    }

    for (int i = 0; i < NUM_AGE_GROUPS; ++i) {
        if (summary.ageGroups[i].patientCount > 0) {
            summary.ageGroups[i].averageCost = summary.ageGroups[i].totalCost / summary.ageGroups[i].patientCount;
            summary.ageGroups[i].preferredCareType = trackers[i].getMostPreferred();
        }
    }

    summary.overallAverageCost = (summary.totalPatients > 0) ? (summary.overallTotalCost / summary.totalPatients) : 0.0;
    return summary;
}

// =============================================================================
// Formatted Table Display (Using <iomanip>)
// =============================================================================
inline void printSummary(const DatasetSummary& summary) {
    std::cout << "\n+" << std::string(103, '=') << "+\n";
    std::cout << "| " << std::left << std::setw(101) 
              << ("FACILITY REPORT: " + summary.facilityName) << " |\n";
    std::cout << "+" << std::string(103, '=') << "+\n";
    std::cout << "| Total Patients       : " << std::left << std::setw(78) << summary.totalPatients << " |\n";
    std::cout << "| Total Medical Cost   : $" << std::fixed << std::setprecision(2) << std::left << std::setw(77) << summary.overallTotalCost << " |\n";
    std::cout << "| Overall Avg / Patient: $" << std::left << std::setw(77) << summary.overallAverageCost << " |\n";
    std::cout << "+" << std::string(103, '-') << "+\n";

    // Column Headers
    std::cout << "| " << std::left 
              << std::setw(34) << "Age Demographic Group"
              << std::right
              << std::setw(10) << "Patients"
              << std::setw(20) << "Total Cost ($)"
              << std::setw(18) << "Avg Cost ($)"
              << "   "
              << std::left
              << std::setw(16) << "Top Care Type"
              << " |\n";
    std::cout << "+" << std::string(103, '-') << "+\n";

    // Data Rows
    for (int i = 0; i < NUM_AGE_GROUPS; ++i) {
        const auto& g = summary.ageGroups[i];
        if (g.patientCount == 0) continue; // Skip groups with no records

        std::cout << "| " << std::left 
                  << std::setw(34) << g.groupName
                  << std::right
                  << std::setw(10) << g.patientCount
                  << std::setw(20) << std::fixed << std::setprecision(2) << g.totalCost
                  << std::setw(18) << std::fixed << std::setprecision(2) << g.averageCost
                  << "   "
                  << std::left
                  << std::setw(16) << g.preferredCareType
                  << " |\n";
    }
    std::cout << "+" << std::string(103, '=') << "+\n\n";
}

#endif