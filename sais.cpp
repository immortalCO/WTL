#include <bits/stdc++.h>
#define RG register
#define For(i, l, r) for(RG int i = l, ___e = r; i <= ___e; ++i)
#define Rep(i, l, r) for(RG int i = l, ___e = r; i < ___e; ++i)

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

const int MaxN = 100010;	

namespace sais
{
	IL void induced_sort(RG const int N, RG const int M, RG const int *S, RG int *SA, RG const bool *T, RG const int *buc, RG int *lbuc, RG int *sbuc)
	{
		RG int tmp;
		for(RG int i = 1; i <= M; ++i) sbuc[i] = buc[i] - 1;
		for(RG int i = 0; i <= N; ++i) if(SA[i] > 0 && !T[tmp = SA[i] - 1]) SA[lbuc[S[tmp]]++] = tmp;
		for(RG int i = N; i >= 0; --i) if(SA[i] > 0 &&  T[tmp = SA[i] - 1]) SA[sbuc[S[tmp]]--] = tmp;
	}
	
	IL bool compare(RG const int *a, RG const int *b, RG int N)
	{
		while(N >= 8)
		{
			if(*(a    ) != *(b    )) return 1;
			if(*(a + 1) != *(b + 1)) return 1;
			if(*(a + 2) != *(b + 2)) return 1;
			if(*(a + 3) != *(b + 3)) return 1;
			if(*(a + 4) != *(b + 4)) return 1;
			if(*(a + 5) != *(b + 5)) return 1;
			if(*(a + 6) != *(b + 6)) return 1;
			if(*(a + 7) != *(b + 7)) return 1;
			a += 8; b += 8; N -= 8;
		}
		while(N--) if(*a++ != *b++) return 1;
		return 0;
	}
	
	char mem_buff[MaxN * 16 * 4], *mem_tot = mem_buff;
	#define Array(T, int, N) RG int *T = (int*) mem_tot; mem_tot += (N) * sizeof(int);
	
	void work(RG int N, RG const int M, RG const int* const S, RG int *SA)
	{
		--N;
		Array(T, bool, N + 1);
		Array(buc, int, M + 1);
		Array(lbuc, int, M + 1);
		Array(sbuc, int, M + 1);
		Array(lms, int, N + 1);
		Array(lab, int, N + 1);
		
		for(RG int i = N; i >= 0; --i)
		{
			T[i] = i == N || (S[i] < S[i + 1]) || (S[i] == S[i + 1] && T[i + 1]);
			++buc[S[i]];
		}
		lbuc[0] = sbuc[0] = 0;
		for(RG int i = 1; i <= M; ++i) {buc[i] += buc[i - 1]; lbuc[i] = buc[i - 1]; sbuc[i] = buc[i] - 1;}
		
		RG int Lms = 0;
		for(RG int i = 1; i <= N; ++i) (T[i] && !T[i - 1]) ? lms[Lms++] = i : 0;
		
		memset(SA, -1, (N + 1) << 2);
		for(RG int i = 0; i != Lms; ++i) SA[sbuc[S[lms[i]]]--] = lms[i];
		induced_sort(N, M, S, SA, T, buc, lbuc, sbuc);
		
		Array(len, int, N + 1);
		lms[Lms] = N + 1; for(RG int i = 0; i != Lms; ++i) len[lms[i]] = lms[i + 1] - lms[i] + 1;
		RG int M0 = 0;
		memset(lab, -1, (N + 1) << 2);
		for(RG int i = 1, p = -1, q = -1; i <= N; ++i)
			if((q = SA[i]) > 0 && T[q] && !T[q - 1])
			{
				if(p == -1 || len[p] != len[q] || compare(S + p, S + q, len[p])) ++M0;
				lab[q] = M0; p = q;
			}
		lab[N] = 0;
		
		Array(S0, int, Lms);
		Array(SA0, int, Lms + 1);
		for(RG int i = 0, pos = 0; i <= N; ++i) (lab[i] >= 0) ? S0[pos++] = lab[i] : 0;
		if(M0 + 1 == Lms) for(RG int i = 0; i != Lms; ++i) SA0[S0[i]] = i;
		else work(Lms, M0, S0, SA0);
		
		lbuc[0] = sbuc[0] = 0;
		for(RG int i = 1; i <= M; ++i) {lbuc[i] = buc[i - 1]; sbuc[i] = buc[i] - 1;}
		memset(SA, -1, (N + 1) << 2);
		for(RG int i = Lms; i--; ) SA[sbuc[S[lms[SA0[i]]]]--] = lms[SA0[i]];
		induced_sort(N, M, S, SA, T, buc, lbuc, sbuc);
	}
	
}
char s[MaxN]; int S[MaxN], SA[MaxN];
int Rk[MaxN], H[MaxN];

int main()
{
	using io::print;
	RG int N = io::gets(s); 
	Rep(i, 0, N) S[i] = s[i] - 'a' + 1;		

	sais::work(N + 1, 26, S, SA);
	For(i, 1, N) print(SA[i] + 1, " \n"[i == N]), Rk[SA[i]] = i;
	
	RG int k = 0, p, q;
	Rep(i, 0, N)
	{
		k ? --k : 0;
		p = Rk[i]; q = p - 1;
		p = SA[p]; q = SA[q];
		for(; S[p + k] == S[q + k]; ++k)
			;
		H[Rk[i]] = k;
	}
	For(i, 2, N) print(H[i], " \n"[i == N]);
}