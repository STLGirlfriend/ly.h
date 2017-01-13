#ifndef _LY_MENCI_DATASTRUCTURE_SPLAY_H
#define _LY_MENCI_DATASTRUCTURE_SPLAY_H

#include <algorithm>

namespace ly {
namespace Menci {
namespace DataStructure {

template < typename T, typename Comp = std::less<T> >
struct Splay {
private:
    struct Node {
        // root 可能会被修改，使用指向指针的指针
        Node *ch[2], **root, *fa;
        // x 表示这个节点上的数
        T x;
        // size 表示整棵树（左右子树和自身）的大小
        // cnt 表示这个数 x 有多少个
        int cnt, size;
        // -1 表示左边界（负无穷），1 表示右边界（正无穷）
        int bound;

        Node(Node **root, Node *fa, const T &x, int bound = 0) : root(root), fa(fa), x(x), cnt(1), size(1), bound(bound) {
            ch[0] = ch[1] = nullptr; // 不要忘记初始化
        }

        // 如果当前节点是其父节点的左儿子，返回 0 否则返回 1
        int relation() {
            return this == fa->ch[0] ? 0 : 1;
        }

        void maintain() {
            size = cnt;
            if (ch[0]) size += ch[0]->size;
            if (ch[1]) size += ch[1]->size;
        }

        void rotate() {
            // 旧的父节点
            Node *old = fa;
            // 旋转前当前节点与父节点的关系
            int r = relation();

            // 自身 <--> 父节点的父节点
            fa = old->fa;
            if (old->fa) {
                // 如果父节点有父节点（不是根），那么用当前节点替换父节点的父节点中父节点的位置的指向
                old->fa->ch[old->relation()] = this;
            }

            // 原有的另一个子节点 <--> 原有的父节点
            if (ch[r ^ 1]) {
                ch[r ^ 1]->fa = old;
            }
            old->ch[r] = ch[r ^ 1]; // 右边的 ch[r ^ 1] 相当于 this->ch[r ^ 1]

            // 原有的父节点 作为自身的子节点
            old->fa = this;
            ch[r ^ 1] = old;

            // 更新维护信息
            old->maintain();
            maintain();

            // 如果转到了根，更新储存的指向根节点的指针
            if (fa == nullptr) {
                *root = this;
            }
        }

        // 旋转到某一特定位置，如在删除时将后继节点旋转为根的右儿子
        // target 为目标父节点，缺省参数为 nullptr，即旋转直到成为根（没有父节点）
        void splay(Node *target = nullptr) {
            while (fa != target) { // while (父节点不是目标父节点)
                if (fa->fa == target) { // 父节点的父节点是目标父节点，直接转一次
                    rotate();
                } else if (fa->relation() == relation()) { // 关系相同，先转父节点，再转自身
                    fa->rotate();
                    rotate();
                } else {
                    rotate();
                    rotate();
                }
            }
        }

        // 前趋，全称 precursor 或 predecessor
        Node *pred() {
            // 左子树的最右点
            Node *v = ch[0];
            while (v->ch[1]) v = v->ch[1];
            return v;
        }

        // 后继，全称 successor
        Node *succ() {
            // 右子树的最左点
            Node *v = ch[1];
            while (v->ch[0]) v = v->ch[0];
            return v;
        }

        // 求一个节点的排名，即左子树大小
        int rank() {
            return ch[0] ? ch[0]->size : 0;
        }
    } *root;

    Comp comp;

    bool equal(const T &a, const T &b) {
        return !comp(a, b) && !comp(b, a);
    }

    Node *insert(const T &x, int bound) {
        // v 是一个指向指针的指针
        // 表示要插入到的位置
        // 如果 v 指向一个空指针
        // 那么就可以插入到这里
        //
        // fa 是新节点的父节点
        Node **v = &root, *fa = nullptr;
        while (*v != nullptr && !((*v)->bound == 0 && equal(x, (*v)->x))) { // 直到找到一个空位置，或者找到原有的值为 x 的节点
            fa = *v;    // 以上一次找到的不空的 v 作为父节点
            fa->size++; // 因为要在这棵子树下插入一个新的节点

            if (bound == -1) {
                v = &fa->ch[0];
            } else if (bound == 1) {
                v = &fa->ch[1];
            } else {
                // 根据大小向左右子树迭代
                if (fa->bound == -1) {
                    v = &fa->ch[1];
                } else if (fa->bound == 1) {
                    v = &fa->ch[0];
                } else if (comp(x, fa->x)) {
                    v = &fa->ch[0];
                } else {
                    v = &fa->ch[1];
                }
            }
        }

        if (*v != nullptr) {
            (*v)->cnt++;
        } else {
            (*v) = new Node(&root, fa, x, bound);
        }

        (*v)->splay();

        // 伸展之后，*v 这个位置存的可能不再是新节点（因为父子关系改变了，而 v 一般指向一个节点的某个子节点的位置）
        // 直接返回根
        return root;
    }

    Node *find(const T &x) {
        // 从根节点开始找
        Node *v = root;
        while (v != nullptr && (v->bound != 0 || !equal(x, v->x))) {
            if (v->bound == -1) {
                v = v->ch[1];
            } else if (v->bound == 1) {
                v = v->ch[0];
            } else if (comp(x, v->x)) {
                v = v->ch[0];
            } else {
                v = v->ch[1];
            }
        }

        if (v) v->splay();
        return v;
    }

public:
    Splay() : root(nullptr) {
        insert(T(), -1);
        insert(T(), 1);
    }

    void insert(const T &x) {
        insert(x, 0);
    }

    int size() {
        return root->size - 2;
    }

    int count(const T &x) {
        Node *v = find(x);
        if (v) return v->cnt;
        else return 0;
    }

    // 删除一个节点
    void erase(Node *v) {
        Node *pred = v->pred(), *succ = v->succ();
        pred->splay();
        succ->splay(pred); // 使后继成为前趋（根）的右儿子

        // 此时后继的左儿子即为要删除的节点 v，且 v 此时为叶子节点

        if (v->size > 1) {
            // 如果存在不止一个 x，只需将计数减一
            v->size--;
            v->cnt--;
        } else {
            delete succ->ch[0]; // 使用 delete 关键字（严格地说，new 和 delete 是运算符）释放 new 分配的内存
            succ->ch[0] = nullptr; // 不要忘记置空
        }

        // 删了节点，需要将大小减小
        succ->size--;
        pred->size--;
    }

    // 删除一个数
    void erase(const T &x) {
        Node *v = find(x);
        if (!v) return; // 没有找到要删的节点，直接返回（一般题目中不会出现这种情况）

        erase(v);
    }

    // 求一个数的前趋
    const T &pred(const T &x) {
        Node *v = find(x);
        if (v == nullptr) {
            v = insert(x, 0);
            const T &res = v->pred()->x;
            erase(v);
            return res;
        } else {
            return v->pred()->x;
        }
    }

    // 求一个数的后继
    const T &succ(const T &x) {
        Node *v = find(x);
        if (v == nullptr) {
            v = insert(x, 0);
            const T &res = v->succ()->x;
            erase(v);
            return res;
        } else {
            return v->succ()->x;
        }
    }

    // 求一个数的排名
    int rank(const T &x) {
        Node *v = find(x);
        if (v == nullptr) {
            v = insert(x, 0);
            // 此时 v 已经是根节点了，因为新插入插入节点会被伸展
            int res = v->rank(); // 因为有一个无穷小，所以不需要 +1
            erase(v);
            return res;
        } else {
            // 此时 v 已经是根节点了，因为查找到的节点会被伸展
            return v->rank();
        }
    }

    // 求第 k 小的数
    const T &select(int k) {
        Node *v = root;
        while (!(k >= v->rank() && k < v->rank() + v->cnt)) {
            if (k < v->rank()) {
                // 要查的第 k 比当前节点的排名小，需要继续在左子树中查询
                v = v->ch[0];
            } else {
                k -= v->rank() + v->cnt;
                v = v->ch[1];
            }
        }
        v->splay();
        return v->x;
    }
};

}
}
}

#endif // _LY_MENCI_DATASTRUCTURE_SPLAY_H
