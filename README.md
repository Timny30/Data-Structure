# Data Structure Assignment — Patient Dataset Analysis

C++ prototypes that load healthcare patient records from CSV datasets and run the **same analytical pipeline** on two different data structures:

| Branch / Program | Data Structure | Main Entry Point |
|------------------|----------------|------------------|
| **`master`** | Custom dynamic **Array** | `Data Structure Assignment.cpp` |
| **`LinkedList`** | Singly **Linked List** | `DataLinkedList.cpp` |

Both programs share the same patient model, datasets, sorting algorithms, search experiments, and summary logic so performance and memory trade-offs can be compared side by side.

---

## Overview

Three facility datasets are loaded and analyzed:

| Dataset | Facility | File |
|---------|----------|------|
| 1 | General Hospital (Facility A) | `Datasets/dataset1 facility_a.csv` |
| 2 | University Medical Center (Facility B) | `Datasets/dataset2 facility_b.csv` |
| 3 | Community Health Clinic (Facility C) | `Datasets/dataset3_facility_c.csv` |

Each CSV has a header row plus ~200 patient records with fields:

- `patientID`
- `age`
- `careType` (e.g. Emergency, Inpatient, Outpatient)
- `lengthOfStay` (hours)
- `baseCostPerHour`
- `daysVisitPerYear`

Derived values used in analysis:

- **Total medical cost** = `lengthOfStay × baseCostPerHour × daysVisitPerYear`
- **Age groups** (pediatrics, young adults, working adults, seniors, etc.)

---

## What Each Program Does

Both programs perform the same sequence of analyses:

1. **Load** the three CSV files into the chosen data structure.
2. **Dataset summary** — record counts, age-group breakdown, care-type distribution, cost statistics.
3. **Structural memory footprint** — estimated memory based on `sizeof()` (payload vs overhead; dynamic `std::string` heap storage is noted but not counted).
4. **Sorting performance** — Merge Sort and Bubble Sort, with comparison/data-movement metrics and repeated timing (median / average / min / max).
5. **Search experiments** — linear search by:
   - Age range (e.g. seniors 61–100, young adults 18–25)
   - Care type (e.g. `"Emergency"`)
   - Visit duration threshold (e.g. ≥ 24 hours)

Metrics reported include matches found, comparisons, record accesses, and timing.

---

## Repository Layout

```
Data-Structure/
├── Data Structure Assignment.cpp   # Array program (master)
├── DataLinkedList.cpp              # Linked List program (LinkedList branch)
├── patientRecord.h                 # patientRecord, Array, node, LinkedList
├── toArray.cpp / toLinkedList.cpp  # CSV loaders
├── calculation.h                   # Dataset summaries & printing
├── arrayMemory.h                   # Array memory footprint (master)
├── linkedListMemory.h              # Linked List memory footprint (LinkedList branch)
├── mergeSort.h / bubbleSort.h      # Sorting algorithms + performance reporting
├── searchExperiment.h              # Search experiments
├── sortMetrics.h / benchmarkStats.h
├── Datasets/                       # Three facility CSV files
└── *.vcxproj / *.slnx              # Visual Studio project files
```

> **Note:** On `master`, the Linked List sources (`DataLinkedList.cpp`, `toLinkedList.cpp`, `linkedListMemory.h`) are not present. Check out the `LinkedList` branch (or merge as needed) to run the list-based program.

---

## Data Structures

### Array (`master`)

- Contiguous `patientRecord*` buffer with `size` and `capacity`.
- Grows by doubling capacity when full.
- Copy constructor and destructor manage ownership.
- Strengths: cache-friendly, O(1) random access, efficient sorting.
- Trade-off: unused capacity and occasional reallocation cost.

### Linked List (`LinkedList` branch)

- Singly linked nodes (`node`: data + `next`), with `head` and `tail`.
- `push_back` is O(1) via the tail pointer.
- Copy constructor deep-copies; destructor walks and frees nodes.
- Strengths: no unused capacity, flexible growth.
- Trade-off: pointer overhead per node, poor locality, O(n) access by index.

---

## Building & Running

### Visual Studio (Windows)

1. Open `Data Structure Assignment.slnx` (or the `.vcxproj`).
2. Ensure the active source is the one you want:
   - **Array:** `Data Structure Assignment.cpp` on `master`
   - **Linked List:** `DataLinkedList.cpp` on `LinkedList` branch
3. Build (Debug/Release, x64 recommended).
4. Run from the project directory so relative paths `Datasets/...` resolve.

### Command line (example)

```bash
# Array version (master)
g++ -std=c++17 -O2 "Data Structure Assignment.cpp" toArray.cpp -o assignment_array
./assignment_array

# Linked List version (checkout LinkedList branch first)
g++ -std=c++17 -O2 DataLinkedList.cpp toLinkedList.cpp -o assignment_list
./assignment_list
```


