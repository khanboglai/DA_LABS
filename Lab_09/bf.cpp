#include <bits/stdc++.h>


using graph = std::vector<std::vector<std::pair<int, int64_t>>>;
const int64_t INF = 1e18;


void bellman_ford(const graph &g, int s, int f, std::vector<int64_t> &d) {
    int n = g.size();
    d[s] = 0;

    // n-1 максимальное количество ребер, которое необходимо для достижения n вершин
    // Даже при наличии цикла, все равно будет n-1 ребро
    // То есть за n-1 итерацию можно релаксировать все ребра в графе
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) {
            for (const auto &edge: g[u]) {
                int v = edge.first;
                int64_t w = edge.second;

                if (d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                }
            }
        }
    }

    // данный блок нужен для выявления отрицательных циклов
    // так как мы ослабили (релаксировали) все ребра, то повтороное ослабление может быть только при наличии отрицательного цикла
    for (int u = 0; u < n; u++) {
        for (const auto &edge: g[u]) {
            int v = edge.first;
            int64_t w = edge.second;

            if (d[u] + w < d[v]) {
                std::cout << "No solution\n"; // отрицательный цикл
                return;
            }
        }
    }

    // if (d[f] != INF) {
    //     std::cout << d[f] << std::endl;
    // } else {
    //     std::cout << "No solution\n"; // на случай, если до вершины нет пути
    // }
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    graph g(n);

    double st, end;

    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t w;
        std::cin >> u >> v >> w;
        u--;
        v--;
        g[u].push_back(std::make_pair(v, w));
        // g[v].push_back(std::make_pair(u, w));
    }

    start--;
    finish--;
    std::vector<int64_t> d(n, INF);

    st = clock();
    bellman_ford(g, start, finish, d);
    end = clock();

    double res = end - st;
    std::cout << "Bellman-Ford: " << std::fixed << std::setprecision(3) << res / 1000.0 << " ms"<< std::endl;
    return 0;
}
