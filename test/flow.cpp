#include <cstdio>
#include <ly.h>

const int MAXN = 200;

ly::Menci::Graph::NetworkFlow<MAXN + 1> graph;

int main() {
    int n, m;
    scanf("%d %d", &m, &n);
    while (m--) {
        int s, t, c;
        scanf("%d %d %d", &s, &t, &c);

        graph.addEdge(s, t, c);
    }

    int ans = graph.dinic(1, n, n + 1);
    printf("%d\n", ans);

	return 0;
}
