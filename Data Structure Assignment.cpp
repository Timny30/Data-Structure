#include <iostream>
#include "patientRecord.h"


int main()
{
    // load Datasets into repectives array and linkedlist
    // General Hospital - Metropolitan Hub
    std::string dataset1 = "Datasets/dataset1 facility_a.csv";

    Array hospitalArray = toArray(dataset1);
    if (hospitalArray.size > 0) {
        std::cout << "Dataset: " << dataset1 << "\n";
        std::cout << "[Array] Loaded " << hospitalArray.size << " records.\n";
        std::cout << "First Patient ID: " << hospitalArray.data[0].patientID << "\n\n";
    }

    LinkedList hospitalLinkedList = toLinkedList(dataset1);
    if (hospitalLinkedList.head != nullptr) {
        std::cout << "[LinkedList] Loaded.\n";
        std::cout << "First Patient ID: " << hospitalLinkedList.head->data.patientID << "\n\n";
    }

    // University Medical Center
    std::string dataset2 = "Datasets/dataset2 facility_b.csv";

    Array universitylArray = toArray(dataset2);
    if (universitylArray.size > 0) {
        std::cout << "Dataset: " << dataset2 << "\n";
        std::cout << "[Array] Loaded " << universitylArray.size << " records.\n";
        std::cout << "First Patient ID: " << universitylArray.data[0].patientID << "\n\n";
    }

    LinkedList universityLinkedList = toLinkedList(dataset1);
    if (universityLinkedList.head != nullptr) {
        std::cout << "[LinkedList] Loaded.\n";
        std::cout << "First Patient ID: " << universityLinkedList.head->data.patientID << "\n\n";
    }

    // Community Health Clinic - Rural/Suburban
    std::string dataset3 = "Datasets/dataset3_facility_c.csv";

    Array communitylArray = toArray(dataset3);
    if (communitylArray.size > 0) {
        std::cout << "Dataset: " << dataset3 << "\n";
        std::cout << "[Array] Loaded " << communitylArray.size << " records.\n";
        std::cout << "First Patient ID: " << communitylArray.data[0].patientID << "\n\n";
    }

    LinkedList communityLinkedList = toLinkedList(dataset1);
    if (communityLinkedList.head != nullptr) {
        std::cout << "[LinkedList] Loaded.\n";
        std::cout << "First Patient ID: " << communityLinkedList.head->data.patientID << "\n\n";
    }

    return 0;
}

