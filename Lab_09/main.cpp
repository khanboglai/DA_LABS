#include <bits/stdc++.h>


const int64_t INF = 1e18; // определили бесконечность
using graph = std::vector<std::vector<std::pair<int, int64_t>>>; // неориентированный граф
using g_item = std::pair<int64_t, int>; // такое положение элементов в паре нужно для правильного расположения в очереди


void dijkstra(const graph &g, int u, int f, std::vector<int64_t> &d) {
    d[u] = 0; // расстояние до начальной вершины 0
    // можно реализовать алгоритм на множестве
    // тут определили порядок элементов в очереди так, чтобы в начале были элемнты с наименим весом
    std::priority_queue<g_item, std::vector<g_item>, std::greater<g_item>> pq;

    pq.push(std::make_pair(0, u)); // кладем старторую вершину

    while (!pq.empty()) {
        g_item current = pq.top();
        pq.pop();

        u = current.second;
        
        if (u == f) { // мы дошли до нужной вершины?
            break;
        }

        if (current.first > d[u]) { // в очереди могут быть старые данные
            continue;
        }

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

    // if (d[f] != INF) { // если путь равен бесконечности, то он не достижим из начальной вершины
    //     std::cout << d[f] << std::endl;
    // } else {
    //     std::cout << "No solution\n";
    // }
}


int main() {
    int n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    graph g(n);

    double st, end;

    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t w;
        std::cin >> u >> v >> w;
        u--; // 0-индекс нотация
        v--;

        // граф неориентированный, нужно рассмотреть путь не только из u->v, но и из v->u
        g[u].push_back(std::make_pair(v, w));
        g[v].push_back(std::make_pair(u, w));
    }

    std::vector<int64_t> d(n, INF); // вектор наименьших расстояний до вершины, изначально бесконечность до каждой

    start--;
    finish--;

    st = clock();
    dijkstra(g, start, finish, d);
    end = clock();

    double res = end - st;
    std::cout << "Dijkstra: " << std::fixed << std::setprecision(3) << res / 1000.0 << " ms"<< std::endl;

    return 0;
}
