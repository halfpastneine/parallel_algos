#pragma once

#include <iostream>
#include <string>
#include "par.h"
#include <chrono>
#include <vector>


void run_test(std::vector<int> input) {
    parlay::sequence<int> seq(input.begin(), input.end());
    parlay::sequence<int> par(input.begin(), input.end());

    sequential_quickSort(seq, 0, input.size() - 1);
    parallel_quicksort(par, 0, par.size() - 1);

    assert(parlay::is_sorted(seq));
    assert(parlay::is_sorted(par));
}



void base_test() {
    run_test({1});
    run_test({-1, -10});
    run_test({9, 8, 7, 6, 5, 4, 3, 2, 1});
    run_test({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    run_test({-9, 8, -7, 6, -5, 4, -3, 2, -1});
    run_test({-1, -2, -3, -4, -5, -6, -7, -8, -9});
    run_test({1, 234, 234, 2, 34, 54, -234, 34, 5654, -645, 45, 560, 907});
    run_test({});
}

void big_test() {
    size_t VEC_SIZE = 10e5;
    std::vector<int> input(VEC_SIZE);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = -i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = std::rand() % VEC_SIZE;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = 42;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = (i % 2 == 0) ? i : -i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = INT_MAX - i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = INT_MIN + i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = (std::rand() % 2000000) - 1000000;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = i;
    }

    for (int i = 0; i < VEC_SIZE / 100; i++) {
        int idx1 = std::rand() % VEC_SIZE;
        int idx2 = std::rand() % VEC_SIZE;
        std::swap(input[idx1], input[idx2]);
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = VEC_SIZE - i;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = std::rand() % 100;
    }
    run_test(input);

    for (int i = 0; i < VEC_SIZE; i++) {
        input[i] = 0;
    }
    run_test(input);
}


void run_all_tests() {
    base_test();
    std::cout << "All base tests passed" << std::endl;
    big_test();
    std::cout << "All big tests passed" << std::endl;
}




void run_comparison() {
    constexpr int VEC_SIZE = 10e8 ;
    constexpr int M = 5;
    constexpr int S = 400000;

    long long seq_time = 0;
    long long par_time = 0;

    for (int i = 0; i < M; i++) {
        parlay::sequence<int> input(VEC_SIZE);
        for (int j = 0; j < VEC_SIZE; j++) {
            input[j] = std::rand() % S;
        }

        auto start = std::chrono::high_resolution_clock::now();
        sequential_quickSort(input, 0, VEC_SIZE - 1);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Number " << i << ": | Seq = " << duration_1.count() << " ms"<< std::endl;
        seq_time += duration_1.count();
        for (int j = 0; j < VEC_SIZE; j++) {
            input[j] = std::rand() % S;
        }

        start = std::chrono::high_resolution_clock::now();
        parlay::execute_with_scheduler(4, [&]{
            // std::cout << parlay::num_workers() << std::endl;
            parallel_quicksort(input, 0, VEC_SIZE - 1);
        });
        end = std::chrono::high_resolution_clock::now();

        auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);



        std::cout << "Number " << i << ": | Par = " << duration_2.count() << "ms" << std::endl;
        par_time += duration_2.count();
    }

    auto mean_seq_time = seq_time / M;
    auto mean_par_time = par_time / M;

    std::cout << "Mean_Seq_Time = " << mean_seq_time << std::endl;
    std::cout << "Mean_Par_Time = " << mean_par_time << std::endl;
    std::cout << "Seq_To_Par = " << static_cast<double>(mean_seq_time) / mean_par_time << std::endl;
}

