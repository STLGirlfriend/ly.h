#ifndef _LY_MENCI_MATH_NUMBERTHEORY_H
#define _LY_MENCI_MATH_NUMBERTHEORY_H

#include <cmath>

namespace ly {
namespace Menci {
namespace Math {
namespace NumberTheory {

template <typename T>
inline T gcd(T a, T b) {
	return !b ? a : gcd(b, a % b);
}

template <typename T>
inline void exgcd(T a, T b, T &g, T &x, T &y) {
	if (!b) g = a, x = 1, y = 0;
	else exgcd(b, a % b, g, y, x), y = y - x * (a / b);
}

template <typename T>
inline T pow(T a, T b, T p) {
	T res = 1;
	for (; b; b = b / 2, a = a * a % p) if (b & 1) res = res * a % p;
	return res;
}

template <typename T>
inline T inv(T a, T p) {
    T g, x, y;
    exgcd(a, p, g, x, y);
    return ((x % p) + p) % p;
}

template <typename T, typename MapType>
inline T bsgs(T a, T b, T p) {
    if (a == 0) return b == 0 ? 1 : -1;

    MapType map;

    T m = ceil(sqrt(p)), t = 1;
    for (int i = 0; i < m; i++) {
        if (!map.count(t)) map[t] = i;
        t = t * a % p;
    }

    T k = inv(t, p), w = b;
    for (int i = 0; i < m; i++) {
        if (map.count(w)) return i * m + map[w];
        w = w * k % p;
    }

    return -1;
}

template <typename T, typename MapType>
inline T exbsgs(T a, T b, T p) {
    T t, c = 0;
    while ((t = gcd(a, p)) != 1) {
        if (b == 1) return c;
        if (b % t != 0) return -1;
        p /= t;
        b = b / t * inv(a / t, p) % p;
        c++;
    }

    T r = bsgs<T, MapType>(a, b, p);
    if (r == -1) return -1;
    else return r + c;
}

template <typename T>
inline bool isPrime(T x) {
	if (x > -2 && x < 2) return false;
	for (T i = 2; i * i <= x; i++) if (x % i == 0) return false;
	return true;
}

}
}
}
}

#endif // _LY_MENCI_MATH_NUMBERTHEORY_H
