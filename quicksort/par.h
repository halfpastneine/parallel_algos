#pragma once
#include "seq.h"
#include <parlay/sequence.h>
#include <utility>

int median(parlay::sequence<int> &a, int l, int r) {
    int step = (r - l) / 11;

    auto arr = parlay::sort(parlay::tabulate(11, [&](int i) {
       return a[l + i * step];
    }));


    return arr[6];
}


// std::pair<int, int> parallel_partition(parlay::sequence<int> &a, int l, int r, int v) {
//     int n = r - l + 1;
//     parlay::sequence<int> t(n), left(n), equal(n);
//
//     parlay::parallel_for(0, n, [&](int i) {
//         t[i] = a[l + i];
//         left[i] = (t[i] < v);
//         equal[i] = (t[i] == v);
//     });
//
//     auto left_ = parlay::scan(left).first;
//     auto equal_ = parlay::scan(equal).first;
//
//     auto lt = left_[n - 1] + left[n - 1];
//     auto et = equal_[n - 1] + equal[n - 1];
//     auto ets = l + lt;
//     auto gts = ets + et;
//
//     parlay::parallel_for(0, n, [&](int i) {
//         int pos;
//         if (t[i] < v) {
//             pos = l + left_[i];
//         } else if (t[i] == v) {
//             pos = ets + equal_[i];
//         } else {
//             pos = gts + i - left_[i] - equal_[i];
//         }
//         a[pos] = t[i];
//     });
//
//     return {l + lt, gts};
// }


std::pair<int, int> parallel_partition(parlay::sequence<int> &a, int l, int r, int pivot) {
    parlay::sequence<int> b(a.begin() + l, a.begin() + r + 1);

    auto [b_sorted, from] =
            parlay::counting_sort(b, 3, [&](int x) -> unsigned {
                return x < pivot ? 0 : (x == pivot ? 1 : 2);
            });

    parlay::copy(b_sorted, a.cut(l, r + 1));


    return {l + from[1], l + from[2]};
}

void parallel_quicksort(parlay::sequence<int> &a, int l, int r) {
    if (l < r) {
        if (r - l <= 10000) {
            sequential_quickSort(a, l, r);
            return;
        }

        // int mid = l + (r - l) / 2;
        int pivot = median(a, l, r);


        auto [left, right] = parallel_partition(a, l, r, pivot);

        parlay::par_do(
            [&]() { parallel_quicksort(a, l, left - 1); },
            [&]() { parallel_quicksort(a, right, r); }
        );
    }
}

