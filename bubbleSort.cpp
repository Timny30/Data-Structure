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

void BubbleSort::printPerformance(const Array& sourceArray) {
	printPerformance(sourceArray, "Dataset");
}

void BubbleSort::printPerformance(const Array& sourceArray, const std::string& facilityName) {
	const SortField fields[] = {
		SortField::Age,
		SortField::VisitDuration,
		SortField::TotalMedicalCost
	};

	std::cout << "\nSORTING PERFORMANCE (Bubble Sort) - " << facilityName << "\n";
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n";
	std::cout << "| Sort Key            | Data Structure | Records         | Median (ns)      | Average (ns)     | Min (ns)         | Max (ns)         | Comparisons      | Data Movements   | Time Complexity  |\n";
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n";

	for (SortField field : fields) {
		// Warm-up runs: fresh copy each time, results discarded.
		for (int w = 0; w < SORT_WARMUP_RUNS; ++w) {
			Array warmupCopy = sourceArray;
			BubbleSort::sort(warmupCopy, field);
		}

		// Measured runs: fresh copy each time, starting from the identical
		// original ordering every run.
		long long samples[SORT_MEASURED_RUNS];
		SortMetrics firstMetrics;
		bool metricsConsistent = true;
		for (int r = 0; r < SORT_MEASURED_RUNS; ++r) {
			Array runCopy = sourceArray;
			auto start = std::chrono::high_resolution_clock::now();
			SortMetrics runMetrics = BubbleSort::sort(runCopy, field);
			auto end = std::chrono::high_resolution_clock::now();

			samples[r] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

			if (r == 0) {
				firstMetrics = runMetrics;
			} else if (runMetrics.comparisons != firstMetrics.comparisons ||
			           runMetrics.dataMovements != firstMetrics.dataMovements) {
				metricsConsistent = false;
			}
		}

		const BenchmarkStats stats = computeBenchmarkStats(samples, SORT_MEASURED_RUNS);

		std::cout << "| " << std::left << std::setw(20) << BubbleSort::fieldName(field)
			<< " | " << std::setw(13) << "Array"
			<< " | " << std::setw(15) << sourceArray.size
			<< " | " << std::setw(16) << stats.medianTimeNs
			<< " | " << std::setw(16) << std::fixed << std::setprecision(1) << stats.averageTimeNs
			<< " | " << std::setw(16) << stats.minTimeNs
			<< " | " << std::setw(16) << stats.maxTimeNs
			<< " | " << std::setw(16) << firstMetrics.comparisons
			<< " | " << std::setw(16) << firstMetrics.dataMovements
			<< " | " << std::setw(16) << "O(n^2)"
			<< " |\n";
		if (!metricsConsistent) {
			std::cout << "| WARNING: comparisons/dataMovements differed across measured runs for "
			          << BubbleSort::fieldName(field) << "\n";
		}
	}
	std::cout << "+---------------------+---------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n";
}

SortMetrics BubbleSort::sort(Array& array) {
	return sort(array, SortField::Age);
}

SortMetrics BubbleSort::sort(Array& array, SortField field) {
	SortMetrics metrics;
	for (int end = array.size - 1; end > 0; --end) {
		bool swapped = false;
		for (int i = 0; i < end; ++i) {
			// One comparison = one evaluation of whether two adjacent records should swap.
			const bool needsSwap = shouldSwap(array.data[i], array.data[i + 1], field);
			metrics.comparisons++;
			if (needsSwap) {
				patientRecord temporary = array.data[i];
				array.data[i] = array.data[i + 1];
				array.data[i + 1] = temporary;
				metrics.dataMovements++; // one logical record swap
				swapped = true;
			}
		}
		if (!swapped) {
			break;
		}
	}
	return metrics;
}