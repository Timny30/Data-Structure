/* #include <iostream>
#include <iomanip>
#include "patientRecord.h"

void printRecord(const patientRecord& p, int index);
void printFirstN(const Array& arr, int n = 5);
int main()
{
    // ----------------------------------------------------
    // 1. General Hospital - Metropolitan Hub
    // ----------------------------------------------------
    std::string dataset1 = "Datasets/dataset1 facility_a.csv";
    Array hospitalArray = toArray(dataset1);
    
    std::cout << "====================================================\n";
    std::cout << " Dataset 1: " << dataset1 << "\n";
    std::cout << " Loaded: " << hospitalArray.size << " records\n";
    std::cout << "====================================================\n";
    printFirstN(hospitalArray, 5);

    // ----------------------------------------------------
    // 2. University Medical Center
    // ----------------------------------------------------
    std::string dataset2 = "Datasets/dataset2 facility_b.csv";
    Array universityArray = toArray(dataset2);
    
    std::cout << "====================================================\n";
    std::cout << " Dataset 2: " << dataset2 << "\n";
    std::cout << " Loaded: " << universityArray.size << " records\n";
    std::cout << "====================================================\n";
    printFirstN(universityArray, 5);

    // ----------------------------------------------------
    // 3. Community Health Clinic - Rural/Suburban
    // ----------------------------------------------------
    std::string dataset3 = "Datasets/dataset3_facility_c.csv";
    Array communityArray = toArray(dataset3);
    
    std::cout << "====================================================\n";
    std::cout << " Dataset 3: " << dataset3 << "\n";
    std::cout << " Loaded: " << communityArray.size << " records\n";
    std::cout << "====================================================\n";
    printFirstN(communityArray, 5);

    return 0;
}

void printRecord(const patientRecord& p, int index) {
    std::cout << "--- Record #" << index + 1 << " ---\n";
    std::cout << "  Patient ID         : " << p.patientID << "\n";
    std::cout << "  Age                : " << p.age << "\n";
    std::cout << "  Age Category       : " << p.getAgeGroup() << "\n";
    std::cout << "  Care Type          : " << p.careType << "\n";
    std::cout << "  Length of Stay (h) : " << p.lengthOfStay << "\n";
    std::cout << "  Base Cost / Hour   : $" << std::fixed << std::setprecision(2) << p.baseCostPerHour << "\n";
    std::cout << "  Days Visit / Year  : " << p.daysVisitPerYear << "\n";
    std::cout << "  Calculated Cost    : $" << p.calculateTotalCost() << "\n\n";
}

void printFirstN(const Array& arr, int n) {
    int limit = (arr.size < n) ? arr.size : n;
    for (int i = 0; i < limit; ++i) {
        printRecord(arr.data[i], i);
    }
}

 */