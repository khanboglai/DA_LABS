#include <bits/stdc++.h>


const int64_t INF = 1e18; // определили бесконечность
using graph = std::vector<std::vector<std::pair<int, int64_t>>>; // неориентированный граф
using g_item = std::pair<int64_t, int>; // такое положение элементов в паре нужно для правильного расположения в очереди


int dijkstra(const graph &g, int u, int f, std::vector<int64_t> &d) {
    int n = g.size();
    d[u] = 0; // расстояние до начальной вершины 0
    // можно реализовать алгоритм на множестве
    // тут определили порядок элементов в очереди так, чтобы в начале были элемнты с наименим весом
    std::priority_queue<g_item, std::vector<g_item>, std::greater<g_item>> pq;

    pq.push(std::make_pair(0, u)); // кладем старторую вершину
    std::vector<bool> visited(n, false);

    while (!pq.empty()) {
        g_item current = pq.top();
        pq.pop();

        u = current.second;
        
        if (u == f) { // мы дошли до нужной вершины?
            break;
        }

        if (visited[u]) { // если мы посетили вершину, нет смысла заходить в нее, из нее уже все релаксировано
            continue;
        }

        visited[u] = true;

        for (size_t i = 0; i < g[u].size(); i++) {
            int v = g[u][i].first; // достаем вершину, в которое есть ребро из u
            int64_t w = g[u][i].second; // доставем вес этого ребра

            // Релоксация
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push(std::make_pair(d[v], v)); // после релоксации нужно положить вершину с новым весом обратно в очередь
            }
        }
    }

    if (d[f] != INF) { // если путь равен бесконечности, то он не достижим из начальной вершины
        return d[f];
    } else {
        return -1; // вес не может получится меньше 0, алгоритм не работает с отрицательными весами
    }
}


int main() {
    int n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    graph g(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t w;
        std::cin >> u >> v >> w;
        --u; // 0-индекс нотация
        --v;

        // граф неориентированный, нужно рассмотреть путь не только из u->v, но и из v->u
        g[u].push_back(std::make_pair(v, w));
        g[v].push_back(std::make_pair(u, w));
    }

    std::vector<int64_t> d(n, INF); // вектор наименьших расстояний до вершины, изначально бесконечность до каждой

    int res = dijkstra(g, start - 1, finish - 1, d);

    if (res != -1) {
        std::cout << res << std::endl;
    } else {
        std::cout << "No solution\n";
    }

    // for (int i = 0; i < n; i++) {
    //     std::cout << "Вершина " << (i + 1) << ": "; // +1 для 1-индексации
    //     for (const auto& edge : g[i]) {
    //         std::cout << " -> (Вершина " << (edge.first + 1) << ", Вес " << edge.second << ")";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
