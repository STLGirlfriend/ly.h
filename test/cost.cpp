#include <cstdio>
#include <ly.h>

const int MAXN = 400;

ly::Menci::Graph::NetworkFlow<MAXN + 1> graph;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    while (m--) {
        int s, t, c, w;
        scanf("%d %d %d %d", &s, &t, &c, &w);

        graph.addEdge(s, t, c, w);
    }

	int flow, cost;
	graph.edmondskarp(1, n, n + 1, flow, cost);
    printf("%d %d\n", flow, cost);

	return 0;
}
