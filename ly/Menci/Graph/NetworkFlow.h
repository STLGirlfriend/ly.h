#ifndef _LY_MENCI_GRAPH_NETWORKFLOW_H
#define _LY_MENCI_GRAPH_NETWORKFLOW_H

#include <climits>
#include <queue>

namespace ly {
namespace Menci {
namespace Graph {

template <int MAXN, typename T = int, T MAX = INT_MAX>
struct NetworkFlow {
private:
	struct Node;
	struct Edge;

	struct Node {
		struct Edge *e, *c;
		T d, f, l;
		bool inq;
	} N[MAXN];

	struct Edge {
		Node *s, *t;
		T f, c, w;
		Edge *next, *r;

		Edge(Node *s, Node *t, T c, T w) : s(s), t(t), f(0), c(c), w(w), next(s->e), r(NULL) {}
	};

public:
	inline void addEdge(int s, int t, T cap, T cost = 0) {
		Edge *e1 = N[s].e = new Edge(&N[s], &N[t], cap, cost);
		Edge *e2 = N[t].e = new Edge(&N[t], &N[s], 0, -cost);

		e1->r = e2;
		e2->r = e1;
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
			N[i].c = NULL;
			N[i].d = N[i].f = N[i].l = 0;
		}
	}

private:
	bool dinicLevel(Node *s, Node *t, int n) {
		for (int i = 0; i < n; i++) {
			N[i].c = N[i].e;
			N[i].l = 0;
		}

		std::queue<Node *> q;

		s->l = 1;
		q.push(s);

		while (!q.empty()) {
			Node *v = q.front();
			q.pop();

			for (Edge *e = v->e; e; e = e->next) {
				if (e->f < e->c && !e->t->l) {
					e->t->l = v->l + 1;
					if (e->t == t) return true;
					else q.push(e->t);
				}
			}
		}

		return false;
	}

	T dinicFind(Node *s, Node *t, T limit = MAX) {
		if (s == t) return limit;

		for (Edge *&e = s->c; e; e = e->next) {
			if (e->f < e->c && e->t->l == s->l + 1) {
				int f = dinicFind(e->t, t, std::min(limit, e->c - e->f));
				if (f) {
					e->f += f;
					e->r->f -= f;
					return f;
				}
			}
		}

		return 0;
	}

public:
	T dinic(int s, int t, int n) {
		T res = 0;
		while (dinicLevel(&N[s], &N[t], n)) {
			T f;
			while ((f = dinicFind(&N[s], &N[t])) > 0) res += f;
		}
		return res;
	}

	// In EK, use var `c` for in edge
	void edmondskarp(int s, int t, int n, T &flow, T &cost) {
		flow = cost = 0;
		while (1) {
			for (int i = 0; i < n; i++) {
				N[i].c = NULL;
				N[i].inq = false;
				N[i].f = 0;
				N[i].d = MAX;
			}

			N[s].d = 0;
			N[s].f = MAX;
			N[s].inq = true;

			std::queue<Node *> q;
			q.push(&N[s]);

			while (!q.empty()) {
				Node *v = q.front();
				q.pop();
				
				v->inq = false;

				for (Edge *e = v->e; e; e = e->next) {
					if (e->f < e->c && e->t->d > v->d + e->w) {
						e->t->d = v->d + e->w;
						e->t->c = e;
						e->t->f = std::min(v->f, e->c - e->f);
						if (!e->t->inq) {
							e->t->inq = true;
							q.push(e->t);
						}
					}
				}
			}

			if (N[t].d == MAX) break;


			for (Edge *e = N[t].c; e; e = e->s->c) {
				e->f += N[t].f;
				e->r->f -= N[t].f;
			}

			flow += N[t].f;
			cost += N[t].f * N[t].d;
		}
	}
};

}
}
}

#endif // _LY_MENCI_GRAPH_NETWORKFLOW_H
