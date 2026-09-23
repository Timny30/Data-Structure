// #include <iostream>
// #include <iomanip>
// #include "patientRecord.h"
// #include "calculation.h"

// int main() {
//     std::cout << "\n=======================================================\n";
//     std::cout << "   PROGRAM 2: LINKEDLIST-BASED PROTOTYPE EXECUTION     \n";
//     std::cout << "=======================================================\n";

//     // Dataset 1
//     LinkedList list1 = toLinkedList("Datasets/dataset1 facility_a.csv");
//     DatasetSummary summary1 = analyzeDataset(list1, "General Hospital (Facility A)");
//     printSummary(summary1);

//     // Dataset 2
//     LinkedList list2 = toLinkedList("Datasets/dataset2 facility_b.csv");
//     DatasetSummary summary2 = analyzeDataset(list2, "University Medical Center (Facility B)");
//     printSummary(summary2);

//     // Dataset 3
//     LinkedList list3 = toLinkedList("Datasets/dataset3_facility_c.csv");
//     DatasetSummary summary3 = analyzeDataset(list3, "Community Health Clinic (Facility C)");
//     printSummary(summary3);

//     return 0;
// }