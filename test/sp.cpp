#include <cstdio>
#include <ly.h>

const int MAXN = 2500;

ly::Menci::Graph::ShortestPath<MAXN + 1> graph;

int main() {
	int n, m, s, t;
	scanf("%d %d %d %d", &n, &m, &s, &t);

	while (m--) {
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		graph.addEdge(u, v, w, true);
	}

	int ans = graph.spfa(s, t);
	printf("%d\n", ans);

	return 0;
}
