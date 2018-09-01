#include <bits/stdc++.h>

#define rep(i, x, y) for (int i = (x), _ = (y); i < _; ++i)
#define down(i, x, y) for (int i = (x) - 1, _ = (y); i >= _; --i)
#define fi first
#define se second
#define mp(x, y) make_pair(x, y)
#define pb(x) push_back(x)
#define bin(x) (1 << (x))
#define SZ(x) int((x).size())
#define LX_JUDGE

using namespace std;
typedef pair<int, int> pii;
typedef vector<int> Vi;
typedef long long ll;

template<typename T> inline bool upmax(T &x, T y) { return x < y ? (x = y, 1) : 0; }
template<typename T> inline bool upmin(T &x, T y) { return x > y ? (x = y, 1) : 0; }

namespace MATH_CAL {
	const int mod = 1e9 + 7;
	constexpr int add(int a, int b) { return a + b >= mod ? a + b - mod : a + b; }
	constexpr int sub(int a, int b) { return a - b < 0 ? a - b + mod : a - b; }
	constexpr int mul(int a, int b) { return (ll) a * b % mod; }
	inline int qpow(int x, int n) { int r = 1; for ( ; n; n /= 2, x = mul(x, x)) if (n & 1) r = mul(r, x); return r; }
	inline int mod_inv(int x) { return qpow(x, mod - 2); }
}

const int MAX_N = 1e7 + 10;

bool mark[MAX_N];
int prime[MAX_N], pcnt;
int mal[MAX_N];
int phi[MAX_N], sig0[MAX_N], tig0[MAX_N];

void sieve(int n) {
	phi[1] = sig0[1] = 1;
	rep (i, 2, n) {
		if (!mark[i]) {
			prime[pcnt++] = i;
			sig0[i] = 2;
			mal[i] = i;
			tig0[i] = 1;
		}
		rep (j, 0, pcnt) {
			int to = prime[j] * i;
			if (to >= n) break ;
			mark[to] = 1;

			if (i % prime[j] == 0) {
				mal[to] = mal[i] * prime[j];
				phi[to] = phi[i] * prime[j];
				tig0[to] = tig0[i] + 1;
				sig0[to] = sig0[i] / tig0[to] * (tig0[to] + 1);
				break ;
			} else {
				mal[to] = prime[j];
				phi[to] = phi[i] * (prime[j] - 1);
				tig0[to] = 2;
				sig0[to] = sig0[i] * 2;
			}
		}
	}
}

namespace ZHOU_SIEVE {
// Current usage: find sum of prime

const int MAX_M = 2e5 + 10;

int N;
int sump[MAX_M];			// MAY BE custom

ll val[MAX_M];
int tot, Lim;
int last[MAX_M], f[MAX_M];

inline int Find(int x) { return x < Lim ? x : tot - (N / x) + 1; }

inline int getInit(ll n) {
	// Some function of n
	return (ll) n * (n + 1) / 2;
}

void find_sum(int n) {
	N = n;
	tot = 0;
	for (int i = 1, p; i <= n; i = p + 1) {
		p = n / (n / i);
		val[++tot] = p;
	}
	for (Lim = 1; val[Lim] == Lim; ++Lim);

	for (int i = 1; i <= tot; ++i) {
		f[i] = getInit(val[i]);
	}

	int M = int(1e5);

	for (int i = 1; i <= M; ++i) sump[i] = sump[i - 1] + prime[i - 1];

	int lef = 1, cur = 1;
	
	for (int i = 0; i < M; ++i) {			
		// Function : f[i] -= f[i / p] * p;
		// f[x] = 1 when current prime > x
		// f[x / p] = 1 when curret prime^2 > x;
		int p = prime[i];
		ll p2 = (ll) p * p;

		while (cur <= tot and val[cur] < p2) {
			last[cur] = i;
			cur++;
		}
		while (lef <= tot and val[cur] < p) {
			f[lef] -= sump[i] - sump[last[lef]];
			lef++;
		}
		
		for (int j = tot; j >= cur; --j) {
			int to = Find(val[j] / p);
			int v = f[to];
			if (lef <= to and to < cur) {
				v -= sump[i] - sump[last[to]];
			}
			f[j] -= v * p;
		}
	}

	while (lef < cur) {
		f[lef] -= sump[M] - sump[last[lef]];
		lef++;
	}

	// f is what we want, use it!
}

}

int main() {
#ifdef LX_JUDGE
	freopen(".in", "r", stdin);
#endif
	return 0;
}
