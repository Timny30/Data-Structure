#ifndef SORT_METRICS_H
#define SORT_METRICS_H

// Tracks logical sort-key comparisons and record rearrangements for a single sort run.
//
// comparisons   = one increment per evaluation of two patient records' selected
//                 sort-key values (Age / Visit Duration / Total Medical Cost)
//                 to determine their relative order.
// dataMovements = one increment per logical rearrangement of a patient record:
//                 for Bubble Sort, one completed adjacent-node swap/relink;
//                 for Merge Sort, one node attached into the merged ordering.
//                 These are structure-specific logical units, not a shared
//                 machine-instruction count (see mergeSort.h/bubbleSort.h).
//
// Loop/index/pointer/nullptr checks, recursion base cases, traversal
// mechanics, and timer calls are NOT counted as comparisons or movements.
struct SortMetrics {
    long long comparisons = 0;
    long long dataMovements = 0;
};

#endif
