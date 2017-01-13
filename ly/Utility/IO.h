#ifndef __LY_UTILITY_IO_H
#define __LY_UTILITY_IO_H

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define _ALLOW_FALLBACK
#ifdef ONLINE_JUDGE
#undef _ALLOW_FALLBACK
#endif

namespace ly
{
namespace Utility
{

class InputStream
{
private:
#ifdef _ALLOW_FALLBACK
    const static int BUFFER_SIZE = 1024 * 1024 * 64;
    char buf[BUFFER_SIZE];
    bool fellback;
#endif
    const static int FLOAT_BUFFER_SIZE = 1024;
	char *p;
    size_t size, pos;
    bool finished;

#ifdef _ALLOW_FALLBACK
    void fallback()
    {
        fellback = true;
        reread();
    }

    void reread()
    {
        if (feof(stdin)) size = 0;
        else size = fread(buf, 1, BUFFER_SIZE, stdin);
        p = buf;
        pos = 0;
    }
#endif

    void map()
    {
        p = reinterpret_cast<char *>(mmap(0, size, PROT_READ, MAP_PRIVATE, fileno(stdin), 0));
    }

	void init()
    {
        int fd = fileno(stdin);

		struct stat sb;
        fstat(fd, &sb);

#ifdef _ALLOW_FALLBACK
        if (!S_ISREG(sb.st_mode))
        {
            fallback();
        }
        else
        {
            size = sb.st_size;
            map();
        }
#else
        size = sb.st_size;
        map();
#endif
	}

    char next()
    {
        if (eof())
        {
            finished = true;
            return EOF;
        }
#ifdef _ALLOW_FALLBACK
        if (fellback && pos == size) reread();
#endif
        return p[pos++];
    }

    template <typename T> InputStream &parseSigned(T &x)
    {
        register char ch;
        register bool sgn = false;

        while (ch = next(), !((ch >= '0' && ch <= '9') || ch == '-' || ch == EOF));

        if (ch == '-') sgn = true, ch = next();
        else if (ch == EOF) return *this;

        x = 0;
        do x = x * 10 + (ch - '0'); while (ch = next(), (ch >= '0' && ch <= '9'));

        if (sgn) x = -x;
        return *this;
    }

    template <typename T> InputStream &parseUnsigned(T &x)
    {
        register char ch;

        while (ch = next(), !((ch >= '0' && ch <= '9') || ch == EOF));
        if (ch == EOF) return *this;

        x = 0;
        do x = x * 10 + (ch - '0'); while (ch = next(), (ch >= '0' && ch <= '9'));

        return *this;
    }

    void tokenize(char *buf)
    {
        register size_t s = 0;
        register char ch;

        while (ch = next(), isspace(ch) && ch != EOF);
        if (ch == EOF)
        {
            *buf = '\0';
            return;
        }

        do buf[s++] = ch; while (ch = next(), !isspace(ch) && ch != EOF);
        buf[s] = '\0';
    }

    const char *floatTokenize()
    {
        static char buf[FLOAT_BUFFER_SIZE];
        tokenize(buf);
        return buf;
    }

    InputStream &parseFloat(float &x)
    {
        x = strtof(floatTokenize(), nullptr);
        return *this;
    }

    InputStream &parseDouble(double &x)
    {
        x = strtod(floatTokenize(), nullptr);
        return *this;
    }

    InputStream &parseLongDouble(long double &x)
    {
        x = strtold(floatTokenize(), nullptr);
        return *this;
    }

public:
    bool eof() const
    {
#ifdef _ALLOW_FALLBACK
        if (fellback)
        {
            return !size;
        }
#endif
        return pos == size || p[pos] == EOF;
    }

	InputStream(const char *file = nullptr) : p(nullptr)
	{
        if (file) freopen(file, "r", stdin);
		init();
	}

    template <typename T> InputStream &operator>>(T &x) {
        static_assert(sizeof(T) == -1, "InputStream doesn't support this type.");
        (void)x;
        return *this;
    }

    InputStream &operator>>(char *s) {
        tokenize(s);
        return *this;
    }

    operator bool() const
    {
        return !finished;
    }
};

template <> InputStream &InputStream::operator>><int>(int &x) { return parseSigned(x); }
template <> InputStream &InputStream::operator>><short>(short &x) { return parseSigned(x); }
template <> InputStream &InputStream::operator>><long long>(long long &x) { return parseSigned(x); }
template <> InputStream &InputStream::operator>><__int128>(__int128 &x) { return parseSigned(x); }

template <> InputStream &InputStream::operator>><unsigned int>(unsigned int &x) { return parseUnsigned(x); }
template <> InputStream &InputStream::operator>><unsigned short>(unsigned short &x) { return parseUnsigned(x); }
template <> InputStream &InputStream::operator>><unsigned long long>(unsigned long long &x) { return parseUnsigned(x); }
template <> InputStream &InputStream::operator>><unsigned __int128>(unsigned __int128 &x) { return parseUnsigned(x); }

template <> InputStream &InputStream::operator>><float>(float &x) { return parseFloat(x); }
template <> InputStream &InputStream::operator>><double>(double &x) { return parseDouble(x); }
template <> InputStream &InputStream::operator>><long double>(long double &x) { return parseLongDouble(x); }

}
}

std::ostream &operator<<(std::ostream &out, __int128 x)
{
    static char buf[128];
    register size_t s = 0;
    register bool sgn = false;
    if (x < 0) sgn = true, buf[s++] = '-', x = -x;
    do buf[s++] = x % 10 + '0'; while (x /= 10);
    std::reverse(buf + sgn, buf + s);
    buf[s++] = '\0';
    out << buf;
    return out;
}

std::ostream &operator<<(std::ostream &out, unsigned __int128 x)
{
    static char buf[128];
    register size_t s = 0;
    do buf[s++] = x % 10 + '0'; while (x /= 10);
    std::reverse(buf, buf + s);
    buf[s++] = '\0';
    out << buf;
    return out;
}

#endif // __LY_UTILITY_IO_H
