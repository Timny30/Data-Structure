#ifndef SORT_METRICS_H
#define SORT_METRICS_H

// Tracks logical sort-key comparisons and record rearrangements for a single sort run.
//
// comparisons   = one increment per evaluation of two patient records' selected
//                 sort-key values to determine their relative order.
// dataMovements = one increment per logical relocation of a patientRecord:
//                 one completed swap for Bubble Sort, or one placement of a
//                 record into the merge buffer for Merge Sort.
//
// Loop/index/pointer/flag checks and timer calls are NOT counted as comparisons.
struct SortMetrics {
    long long comparisons = 0;
    long long dataMovements = 0;
};

#endif
