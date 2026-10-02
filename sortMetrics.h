#ifndef SORT_METRICS_H
#define SORT_METRICS_H

// Tracks logical sort-key comparisons and record rearrangements for a single sort run.
struct SortMetrics {
    long long comparisons = 0;
    long long dataMovements = 0;
};

#endif