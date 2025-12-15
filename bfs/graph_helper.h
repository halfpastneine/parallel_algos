int get_index(int x, int y, int z, int N)
{
    return x * N * N + y * N + z;
}

int dist_calc(int idx, int N)
{
    return idx / N / N % N + idx / N % N + idx % N;
}

auto generate_graph(int N)
{
    parlay::sequence<parlay::sequence<int>> edges(N * N * N);
    const int dx[6] = {1, -1, 0, 0, 0, 0};
    const int dy[6] = {0, 0, 1, -1, 0, 0};
    const int dz[6] = {0, 0, 0, 0, 1, -1};

    for (int x = 0; x < N; ++x)
    {
        for (int y = 0; y < N; ++y)
        {
            for (int z = 0; z < N; ++z)
            {
                auto vertex_from = get_index(x, y, z, N);

                for (int i = 0; i < 6; ++i)
                {
                    auto new_x = x + dx[i];
                    auto new_y = y + dy[i];
                    auto new_z = z + dz[i];

                    if (new_x >= 0 && new_x < N && new_y >= 0 && new_y < N && new_z >= 0 && new_z < N)
                    {
                        auto vertex_to = get_index(new_x, new_y, new_z, N);
                        edges[vertex_from].push_back(vertex_to);
                    }
                }
            }
        }
    }

    return edges;
}