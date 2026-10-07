#ifndef BENCHMARK_STATS_H
#define BENCHMARK_STATS_H

constexpr int SORT_WARMUP_RUNS = 3;
constexpr int SORT_MEASURED_RUNS = 10;
constexpr int SEARCH_WARMUP_RUNS = 5;
constexpr int SEARCH_MEASURED_RUNS = 100;

struct BenchmarkStats {
    long long medianTimeNs = 0;
    double averageTimeNs = 0.0;
    long long minTimeNs = 0;
    long long maxTimeNs = 0;
    int measuredRuns = 0;
};

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

    long double total = 0.0L;
    for (int i = 0; i < count; ++i) {
        total += static_cast<long double>(samples[i]);
    }
    stats.averageTimeNs = static_cast<double>(total / count);

    return stats;
}

#endif
