#include <cstdio>
#include <ly.h>

ly::Menci::DataStructure::Splay<int> splay;

int main() {
	int n;
    scanf("%d", &n);

    while (n--) {
        int opt, x;
        scanf("%d %d", &opt, &x);

        if (opt == 1) {
            splay.insert(x);
        } else if (opt == 2) {
            splay.erase(x);
        } else if (opt == 3) {
            printf("%d\n", splay.rank(x));
        } else if (opt == 4) {
            printf("%d\n", splay.select(x));
        } else if (opt == 5) {
            printf("%d\n", splay.pred(x));
        } else if (opt == 6) {
            printf("%d\n", splay.succ(x));
        }
    }

	return 0;
}
