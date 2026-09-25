#ifndef BENCHMARK_STATS_H
#define BENCHMARK_STATS_H

// Repeated-benchmark policy, shared identically by sort and search
// benchmarks on both the Array and Linked List implementations.
constexpr int SORT_WARMUP_RUNS = 3;
constexpr int SORT_MEASURED_RUNS = 30;
constexpr int SEARCH_WARMUP_RUNS = 5;
constexpr int SEARCH_MEASURED_RUNS = 100;

// Timing-sample statistics for a repeated benchmark. Deliberately generic -
// no dependency on Array/LinkedList/patientRecord - so it can be reused by
// any repeated-timing benchmark. Times are nanoseconds throughout.
struct BenchmarkStats {
    long long medianTimeNs = 0;
    double averageTimeNs = 0.0;
    long long minTimeNs = 0;
    long long maxTimeNs = 0;
    int measuredRuns = 0;
};

// Sorts a raw array of nanosecond timing samples in place (simple insertion
// sort - sample counts here are small: 30 for sorts, 100 for searches). This
// is timing-sample bookkeeping only and is completely separate from any
// patient-record sorting algorithm.
inline void sortTimingSamples(long long* samples, int count) {
    for (int i = 1; i < count; ++i) {
        const long long key = samples[i];
        int j = i - 1;
        while (j >= 0 && samples[j] > key) {
            samples[j + 1] = samples[j];
            --j;
        }
        samples[j + 1] = key;
    }
}

// Computes BenchmarkStats from `count` nanosecond timing samples (measured
// runs only - warm-up samples must never be passed in). The caller owns the
// array; this only reorders it in place for median calculation.
inline BenchmarkStats computeBenchmarkStats(long long* samples, int count) {
    BenchmarkStats stats;
    if (count <= 0) {
        return stats;
    }

    sortTimingSamples(samples, count);

    stats.measuredRuns = count;
    stats.minTimeNs = samples[0];
    stats.maxTimeNs = samples[count - 1];

    if (count % 2 == 0) {
        stats.medianTimeNs = (samples[count / 2 - 1] + samples[count / 2]) / 2;
    } else {
        stats.medianTimeNs = samples[count / 2];
    }

    // Accumulate in long double to avoid integer truncation of fractional ns.
    long double total = 0.0L;
    for (int i = 0; i < count; ++i) {
        total += static_cast<long double>(samples[i]);
    }
    stats.averageTimeNs = static_cast<double>(total / count);

    return stats;
}

#endif
