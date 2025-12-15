#pragma once
#include <queue>
#include <parlay/primitives.h>


parlay::sequence<int> seqeuential_bfs(parlay::sequence<parlay::sequence<int>>& edges)
{
    auto n = edges.size();
    std::vector<bool> visited(n);
    parlay::sequence<int> distance(n);
    std::queue<int> q;

    int start = 0;
    visited[start] = true;
    q.push(start);
    distance[start] = 0;

    while (!q.empty())
    {
        start = q.front();
        q.pop();

        for (auto el : edges[start])
        {
            if (!visited[el])
            {
                visited[el] = true;
                q.push(el);
                distance[el] = distance[start] + 1;
            }
        }
    }

    return distance;
}
