#pragma once
#include <parlay/primitives.h>


int partition(parlay::sequence<int>& a, int i, int j, int value) {
    while (i <= j) {
        while (a[i] < value) i++;
        while (a[j] > value) j--;
        if (i >= j) break;
        std::swap(a[i], a[j]);
        i++; j--;
    }
    return j;
}


void sequential_quickSort(parlay::sequence<int>& a, int l, int r) {

    if (l < r) {
        int q = partition(a, l, r, a[(l + r) / 2]);
        sequential_quickSort(a, l, q);
        sequential_quickSort(a, q + 1, r);
    }
}