#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#define rep(i, x, y) for (int i = (x), _ = (y); i <= _; ++i)
#define down(i, x, y) for (int i = (x), _ = (y); i >= _; --i)
#define x first
#define y second

using namespace std;
typedef long long ll;
typedef long long LL;

template<typename T> inline void upmax(T & x, T y) { x < y ? x = y : 0; }
template<typename T> inline void upmin(T & x, T y) { x > y ? x = y : 0; }

namespace MATH_CAL {
	const int mod = 998244353;
	constexpr int add(int a, int b) { return (a + b) >= mod ? a + b - mod : a + b; }
	constexpr int sub(int a, int b) { return (a - b) < 0 ? a - b + mod : a - b; }
	constexpr int mul(int a, int b) { return (ll) a * b % mod; }
	inline void Add(int &a, int b) { (a += b) >= mod ? a -= mod : 0; }
	inline void Sub(int &a, int b) { (a -= b) < 0 ? a += mod : 0; }
	inline int qpow(int x, int n) { int r = 1; for ( ; n; n /= 2, x = mul(x, x)) if (n & 1) r = mul(x, r); return r; }
} using namespace MATH_CAL;

const int inf = 0x3f3f3f3f ;
const int MAX_N = 1e5 + 10;

namespace linkCutTree {
	int deg[MAX_N];
	int ch[MAX_N][2], fa[MAX_N], up[MAX_N];
	int val[MAX_N], sum[MAX_N][2], len[MAX_N], rev[MAX_N];

#define ls (ch[o][0])
#define rs (ch[o][1])

	inline void Rev(int o) {
		if (o) {
			swap(ch[o][0], ch[o][1]); 
			swap(sum[o][0], sum[o][1]);
			rev[o] ^= 1;
		}
	}
	inline void push_down(int o) {
		if (rev[o]) {
			Rev(ch[o][0]); Rev(ch[o][1]);
			rev[o] = 0;
		}
	}
	void push_up(int o) {
		push_down(o);
		sum[o][0] = add(sum[ls][0], mul(add(sum[rs][0], val[o]), mul(len[ls], deg[o])));
		sum[o][1] = add(sum[rs][1], mul(add(sum[ls][1], val[o]), mul(len[rs], deg[o])));
		len[o] = mul(deg[o], mul(len[ls], len[rs]));
	}

	void rotate(int x) {
		int y = fa[x], d = ch[y][0] == x;
		fa[ ch[y][d ^ 1] = ch[x][d] ] = y;
		ch[ fa[x] = fa[y] ][ ch[fa[y]][1] == y ] = x;
		fa[ ch[x][d] ] = x;
		push_up(y);
	}

	int _up(int x) {
		int d = fa[x] ? _up(fa[x]) : up[x];
		push_down(x);
		return d;
	}

	void splay(int x) {
		up[x] = _up(x);

		while (fa[x]) {
			int y = fa[x], z = fa[y];
			if (z) {
				rotate((ch[z][0] == y) == (ch[y][0] == x) ? y : x);
			}
			rotate(x);
		}

		push_up(x);
	}

	void access(int x) {
		int v = x;
		int y = 0;
		while (x) {
			splay(x);

			fa[ ch[x][1] ] = 0;
			up[ ch[x][1] ] = x;

			int s = sum[ ch[x][1] ][0];
			Add(val[x], s);
			Sub(val[x], sum[y][0]);

			ch[x][1] = y;
			fa[y] = x;
			push_up(x);

			y = x;
			x = up[x];
		}
		splay(v);
	}

	inline int getR(int x) {
		access(x);
		while (ch[x][0]) {
			push_down(x);
			x = ch[x][0];
		}
		push_down(x);
		return x;
	}

	void makeR(int x) {
		access(x);
		Rev(x);
	}

	bool add_edge(int x, int y) {
		if (getR(x) == getR(y)) return 0;

		makeR(x); makeR(y);
		deg[x]++; deg[y]++;

		up[x] = y;
		push_up(x);

		Add(val[y], sum[x][0]);
		push_up(y);

		return 1;
	}

	inline bool cut_edge(int x, int y) {
		if (getR(x) != getR(y)) return 0;
		if (x == y) return 0;

		makeR(x);
		access(y);

		int f = ch[y][0];
		while (ch[f][1]) {
			push_down(f);
			f = ch[f][1];
		}
		push_down(f);

		makeR(f);
		access(y);

		deg[f]--; deg[y]--;

		up[ ch[y][0] ] = fa[ ch[y][0] ] = 0;
		ch[y][0] = 0;
		push_up(y);

		splay(f);
		push_up(f);

		return 1;
	}

	int query(int x) {
		makeR(x);
		if (deg[x] == 0) return 0;
		int r = qpow(deg[x], mod - 2);
		return mul(2, mul(r, sub(mul(sum[x][0], r), 1)));
	}

	void INIT(int n) {
		for (int i = 1; i <= n; ++i) val[i] = 1;
		len[0] = 1;
	}
}

int main()
{
#ifdef LX_JUDGE
	freopen("in.txt", "r", stdin);
#endif
	freopen(".in", "r", stdin);

	using namespace linkCutTree;
	int n, m;
	scanf("%d%d", &n, &m);
	INIT(n);

	for (int i = 1; i < n; ++i) {
		int x, y;
		scanf("%d%d", &x, &y);
		add_edge(x, y);
	}

	for (int i = 1; i <= m; ++i) {
		int opt, x, y;
		scanf("%d%d", &opt, &x);
		if (opt <= 2) scanf("%d", &y);
		if (opt == 1) {
			if (!add_edge(x, y)) {
				puts("-1");
			}
		} else if (opt == 2) {
			if (!cut_edge(x, y)) {
				puts("-1");
			}
		} else {
			int res = query(x);
			printf("%d\n", res);
		}
	}

	return 0;
}
