#pragma once
#include "seq.h"


parlay::sequence<int> parallel_bfs(parlay::sequence<parlay::sequence<int>>& edges)
{
    auto n = edges.size();
    int start = 0, d = -1;
    parlay::sequence<std::atomic_bool> visited(n);
    parlay::sequence<int> distance(n), current_row = {start};

    visited[start] = true;

    while (!current_row.empty())
    {
        auto [indexes, total_size] = parlay::scan(
            parlay::map(current_row, [&](auto vertex)
            {
                distance[vertex] = d + 1;
                return edges[vertex].size();
            })
        );

        parlay::sequence<int> next_row(total_size);


        parlay::parallel_for(0, current_row.size(), [&](int i)
        {
            for (auto v : edges[current_row[i]])
            {
                bool is_visited = false;
                next_row[indexes[i]++] = visited[v].compare_exchange_strong(is_visited, true) ? v : -1;
            }
        });

        current_row = parlay::filter(
            next_row, [&](int vertex) { return vertex != -1; }
        );

        d++;
    }

    return distance;
}
