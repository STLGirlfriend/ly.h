#ifndef _LY_MENCI_GRAPH_SHORTESTPATH_H
#define _LY_MENCI_GRAPH_SHORTESTPATH_H

#include <climits>
#include <queue>

namespace ly {
namespace Menci {
namespace Graph {

template <int MAXN, typename T = int, T MAX = INT_MAX>
struct ShortestPath {
private:
	struct Node;
	struct Edge;

	struct Node {
		struct Edge *e;
		T d;
		bool inq;
	} N[MAXN];

	struct Edge {
		Node *s, *t;
		T w;
		Edge *next;

		Edge(Node *s, Node *t, T w) : s(s), t(t), w(w), next(s->e) {}
	};

public:
	inline void addEdge(int s, int t, T w = 1, bool rev = false) {
		N[s].e = new Edge(&N[s], &N[t], w);
		if (rev) N[t].e = new Edge(&N[t], &N[s], w);
	}

	inline void clear() {
		reset();
		for (int i = 0; i < MAXN; i++) {
			for (Edge *&e = N[i].e, *next; e; next = e->next, delete e, e = next);
		}
	}

	inline void reset() {
		for (int i = 0; i < MAXN; i++) {
			N[i].inq = false;
			N[i].d = MAX;
		}
	}

	inline T spfa(int s, int t = -1) {
		reset();

		std::queue<Node *> q;

		N[s].d = 0;
		N[s].inq = true;
		q.push(&N[s]);

		while (!q.empty()) {
			Node *v = q.front();
			q.pop();

			v->inq = false;

			for (Edge *e = v->e; e; e = e->next) {
				if (e->t->d > v->d + e->w) {
					e->t->d = v->d + e->w;
					if (!e->t->inq) {
						e->t->inq = true;
						q.push(e->t);
					}
				}
			}
		}

		if (t != -1) return N[t].d;
		else return -1;
	}

	inline T dijkstra(int s, int t = -1) {
		reset();

		std::priority_queue< std::pair<T, Node *> > q;

		N[s].d = 0;
		q.push(std::make_pair(0, &N[s]));

		while (!q.empty()) {
			std::pair<T, Node *> p = q.top();
			q.pop();

			Node *v = p.second;

			if (-v->d != p.first) continue;
			if (t != -1 && v->d >= N[t].d) break;

			for (Edge *e = v->e; e; e = e->next) {
				if (e->t->d > v->d + e->w) {
					e->t->d = v->d + e->w;
					q.push(std::make_pair(-e->t->d, e->t));
				}
			}
		}

		if (t != -1) return N[t].dist;
		else return -1;
	}

	inline T getDist(int u) {
		return N[u].dist;
	}
};

}
}
}

#endif // _LY_MENCI_GRAPH_SHORTESTPATH_H
