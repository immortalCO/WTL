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

const int MAX_N = bin(18) + 1;
const int inf = 0x3f3f3f3f;

template<int MAX_M, int SIGMA> 
class Suffix_Tree {
private :
	int nxt[MAX_M][SIGMA], link[MAX_M];
	int fpos[MAX_M], len[MAX_M];
	int S[MAX_M], N;
	int poolCur, now, pos;

	int make_node(int _pos, int _len) {
		int o = poolCur++;
		fpos[o] = _pos;
		len[o] = _len;
		return o;
	}
	void go_edge() {
		while (pos > len[nxt[now][S[N - pos]]]) {
			now = nxt[now][S[N - pos]];
			pos -= len[now];
		}
	}

	void extend(int c) {
		S[N++] = c;
		pos++;
		int last = 0;
		while (pos > 0) {
			go_edge();
			int edge = S[N - pos];
			int &v = nxt[now][edge];
			int t = S[fpos[v] + pos - 1];
			if (v == 0) {
				v = make_node(N - pos, inf);
				link[last] = now;
				last = 0;
			} else if (t == c) {
				link[last] = now;
				return ;
			} else {
				int u = make_node(fpos[v], pos - 1);
				nxt[u][c] = make_node(N - 1, inf);
				nxt[u][t] = v;
				fpos[v] += pos - 1;
				len[v] -= pos - 1;
				v = u;
				link[last] = u;
				last = u;
			}
			now == 0 ? pos-- : now = link[now];
		}
	}

public :
	void build(int str[], int N) {
		rep (i, 0, N) extend(str[i]);
		// DO SOMETHING ELSE
		// TODO
	}
};

int main() {
#ifdef LX_JUDGE
	freopen(".in", "r", stdin);
#endif
	return 0;
}
