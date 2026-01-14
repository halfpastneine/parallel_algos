#pragma once

#include <iostream>
#include <chrono>
#include <random>

#include "par.h"
#include "graph_helper.h"

void run_base_test(parlay::sequence<parlay::sequence<int>>& test, parlay::sequence<int>& answer)
{
    assert(seqeuential_bfs(test) == answer);
    assert(parallel_bfs(test) == answer);
}

void run_cubic_test(int N)
{
    auto edges = generate_graph(N);

    auto seq = seqeuential_bfs(edges);
    auto par = parallel_bfs(edges);

    for (int i = 0; i < seq.size(); i++)
    {
        auto distance = dist_calc(i, N);
        assert(seq[i] == distance);
        assert(par[i] == distance);
    }
}


void base_graph_test()
{
    parlay::sequence<parlay::sequence<int>> test = {{1, 2}, {0, 2}, {0, 1, 3, 4}, {2}, {2}};
    parlay::sequence answer = {0, 1, 1, 2, 2};
    run_base_test(test, answer);

    test = {{1, 2}, {0, 2, 3}, {0, 1, 4}, {1, 4}, {2, 3}};
    run_base_test(test, answer);

    test = {{1, 2, 3}, {0, 4}, {0, 5, 6}, {0, 7}, {1, 8}, {2, 9}, {2}, {3}, {4}, {5}};
    answer = {0, 1, 1, 1, 2, 2, 2, 2, 3, 3};
    run_base_test(test, answer);

    test = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
    answer = {0, 1, 1, 1};
    run_base_test(test, answer);

    test = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}};
    answer = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    run_base_test(test, answer);

    test = {
        {1, 2, 3}, {0, 4, 5}, {0, 6, 7}, {0, 8}, {1, 9, 10},
        {1, 11}, {2, 12}, {2, 13, 14}, {3, 15}, {4}, {4, 16},
        {5, 17}, {6}, {7}, {7, 18}, {8}, {10}, {11, 19}, {14}, {17}
    };
    answer = {0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 5};
    run_base_test(test, answer);

    std::cout << "All base tests passed\n";
}


inline std::random_device dev;
inline std::mt19937 rng(dev());
const int n = 10000;
inline std::uniform_int_distribution<std::mt19937::result_type> vertex_num(0, n - 1);
parlay::sequence<parlay::sequence<int>> graph(n);
parlay::sequence<int> answer(n, -1);
std::vector layers(6, std::vector<int>());

void generate_edges(int v, int distance)
{
    while (graph[v].size() != 6)
    {
        int vertex = static_cast<int>(vertex_num(rng));
        if (vertex == v || std::find(graph[v].begin(), graph[v].end(), vertex) != graph[v].end()) continue;

        graph[v].push_back(vertex);
        layers[distance].push_back(vertex);
        if (answer[vertex] == -1)
        {
            answer[vertex] = distance;
            continue;
        }

        answer[vertex] = std::min(answer[vertex], distance);
    }
}

void generate_random_graph(int depth)
{
    if (depth == 5) return;
    auto v = layers[depth];

    for (auto el : v) generate_edges(el, depth + 1);
    generate_random_graph(depth + 1);
}


void random_graph_test()
{

    for (int i = 0; i < 5; ++i)
    {
        graph.assign(n, parlay::sequence<int>());
        answer.assign(n, -1);
        layers.assign(6, std::vector<int>());
        answer[0] = 0;
        layers[0].push_back(0);
        generate_random_graph(0);
        std::for_each(answer.begin(), answer.end(), [](int& v) {v = v == -1 ? 0 : v;});
        assert(seqeuential_bfs(graph) == answer);
        assert(parallel_bfs(graph) == answer);
    }

    std::cout << "All random tests passed\n";
}


void cubic_graph_test()
{
    run_cubic_test(13);
    run_cubic_test(25);
    // run_cubic_test(57);
    run_cubic_test(77);
    run_cubic_test(101);
    run_cubic_test(121);
    run_cubic_test(200);
    run_cubic_test(231);
    run_cubic_test(300);

    std::cout << "All cubic tests passed\n";
}


const int N = 300;


void parallel_and_sequential_comparison()
{
    long long seq_time = 0;
    long long par_time = 0;

    auto edges = generate_graph(N);

    for (int i = 0; i < 5; i++)
    {
        auto seq_start = std::chrono::high_resolution_clock::now();
        seqeuential_bfs(edges);
        auto seq_end = std::chrono::high_resolution_clock::now();

        auto seq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(seq_end - seq_start).count();
        std::cout << seq_duration << "ms, ";


        auto par_start = std::chrono::high_resolution_clock::now();
        parallel_bfs(edges);
        auto par_end = std::chrono::high_resolution_clock::now();

        auto par_duration = std::chrono::duration_cast<std::chrono::milliseconds>(par_end - par_start).count();


        std::cout << par_duration << "ms, " << static_cast<double>(seq_duration) / static_cast<double>(par_duration) <<
            std::endl;

        seq_time += seq_duration;
        par_time += par_duration;
    }

    auto mean_seq_time = seq_time / 5;
    auto mean_par_time = par_time / 5;

    std::cout << "Mean_Seq_Time = " << mean_seq_time << std::endl;
    std::cout << "Mean_Par_Time = " << mean_par_time << std::endl;
    std::cout << "Seq_To_Par = " << static_cast<double>(mean_seq_time) / static_cast<double>(mean_par_time) <<
        std::endl;
}
