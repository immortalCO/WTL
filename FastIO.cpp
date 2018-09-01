#include <cstdio>

namespace io
{
	#define RG register
	const int MaxBuff = 1 << 15;
	const int MaxOut = 1 << 24;
	char b[MaxBuff], *S = b, *T = b;
	#define getc() (S == T && (T = (S = b) + fread(b, 1, MaxBuff, stdin), S == T) ? 0 : *S++)
	#define O(x) __attribute__((optimize("-O"#x)))
	#define IL __inline__ __attribute__((always_inline))
	template<class Type> IL Type read()
	{
		RG char ch; RG Type ans = 0; RG bool neg = 0;
		while(ch = getc(), (ch < '0' || ch > '9') && ch != '-')		;
		ch == '-' ? neg = 1 : ans = ch - '0';
		while(ch = getc(), '0' <= ch && ch <= '9') ans = ans * 10 + ch - '0';
		return neg ? -ans : ans;
	}
	IL int gets(RG char *s)
	{
		RG char *iter = s;
		while(*iter = getc(), *iter == ' ' || *iter == '\n' || *iter == '\r')
			;
		while(*++iter = getc(), *iter && *iter != ' ' && *iter != '\n' && *iter != '\r')
			;
		*iter = 0;		
		return iter - s;
	}
	char buff[MaxOut], *iter = buff;
	template<class T> IL void print(RG T x, RG char ch = '\n')
	{
		static int stack[110]; RG int O = 0; RG char *iter = io::iter;
		if(!x)*iter++ = '0';
		else
		{
			(x < 0) ? x = -x, *iter++ = '-' : 1;
			for(; x; x /= 10) stack[++O] = x % 10;
			for(; O; *iter++ = '0' + stack[O--])
				;
		}
		*iter++ = ch, io::iter = iter;
	}
	IL void puts(RG const char *s) {while(*s) *iter++ = *s++;}
	struct Output {
		~Output() {fwrite(buff, 1, iter - buff, stdout), iter = buff;}
	}	output_hlpr;
}

int main() {
	int N = io::read<int>();
	long long M = io::read<long long>();
	char s[1111]; io::gets(s);
	io::print(2333);
	io::puts("6666");
}