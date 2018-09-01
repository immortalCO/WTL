#include <bits/stdc++.h>

#define rep(i, x, y) for (int i = (x); i < (y); ++i)
#define down(i, x, y) for (int i = (x) - 1; i >= (y); --i)
#define mp(x, y) make_pair(x, y)
#define pb(x) push_back(x)
#define bin(x) (1 << (x))
#define fi first
#define se second
#define LX_JUDGE

namespace BGG {

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> Vi;

template<typename T> inline bool upmax(T &x, T y) { return x < y ? (x = y, 1) : 0; }
template<typename T> inline bool upmin(T &x, T y) { return x > y ? (x = y, 1) : 0; }

const int MAX_N = bin(18) + 1;

namespace MATH_CAL {
	const int mod = 998244353, G = 3;
	constexpr int add(int a, int b) { return a + b >= mod ? a + b - mod : a + b; }
	constexpr int sub(int a, int b) { return a - b < 0 ? a - b + mod : a - b; }
	constexpr int mul(int a, int b) { return (ll) a * b % mod; }
	inline int qpow(int x, int n) { int r = 1; for ( ; n; n /= 2, x = mul(x, x)) if (n & 1) r = mul(r, x); return r; }
	inline int mod_inv(int x) { return qpow(x, mod - 2); }
} using namespace MATH_CAL;

namespace FFT {
	inline int length(int n) { int T = 1; while (T < n) T <<= 1; return T; }

	int w[2][MAX_N * 2];

	void _fft(int a[], int n, int w[]) {
		for (int i = 0, j = 0; i < n; ++i) {
			if (i < j) swap(a[i], a[j]);
			for (int l = n / 2; (j ^= l) < l; l >>= 1);
		}
		int *r = w + 2;
		for (int i = 2, m = 1; i <= n; r += i, m = i, i *= 2) {
			for (int j = 0; j < n; j += i) for (int k = 0; k < m; ++k) {
				int t = mul(r[k], a[j + m + k]);
				a[j + m + k] = sub(a[j + k], t);
				a[j + k] = add(a[j + k], t);
			}
		}
	}

	void init(int n) {
		int a = qpow(G, (mod - 1) / n), b = mod_inv(a);
		w[0][n] = w[1][n] = 1;
		rep (i, n + 1, n + n) {
			w[0][i] = mul(w[0][i - 1], a);
			w[1][i] = mul(w[1][i - 1], b);
		}
		down (i, n, 1) {
			w[0][i] = w[0][i << 1];
			w[1][i] = w[1][i << 1];
		}
	}
	void dft(int a[], int n) {
		_fft(a, n, w[0]);
	}
	void idft(int a[], int n) {
		_fft(a, n, w[1]);
		int r = mod_inv(n);
		rep (i, 0, n) a[i] = mul(a[i], r);
	}

	void convol(int a[], int n, int b[], int m, int c[]) {
		static int ta[MAX_N], tb[MAX_N];
		int L = length(n + m - 1);

		memcpy(ta, a, n << 2); memset(ta + n, 0, (L - n) << 2);
		memcpy(tb, b, m << 2); memset(tb + m, 0, (L - m) << 2);

		dft(ta, L); dft(tb, L);
		rep (i, 0, L) c[i] = mul(ta[i], tb[i]);
		idft(c, L);
	}

	void poly_inv(int a[], int n, int b[]) {
		if (n == 1) {
			b[0] = mod_inv(a[0]);
			return ;
		}
		static int c[MAX_N];
		int m = (n + 1) / 2;
		poly_inv(a, m, b);

		int L = length(n * 2);
		memcpy(c, a, n << 2); memset(c + n, 0, (L - n) << 2);
		memset(b + m, 0, (L - m) << 2);

		dft(b, L); dft(c, L);
		rep (i, 0, L) b[i] = mul(b[i], sub(2, mul(b[i], c[i])));
		idft(b, L);
	}

	void BTrans(int f[], int fac[], int inv[], int n) {
		static int ta[MAX_N];

		int T = 1;
		while (T < n * 2) T <<= 1;

		rep (i, 0, n) f[i] = mul(f[i], inv[i]);

		rep (i, 0, n) {
			ta[i] = inv[i];
			(i & 1) && (ta[i] = sub(0, ta[i]));
		}

		convol(f, n, ta, n, f);

		rep (i, 0, n) f[i] = mul(f[i], fac[i]);
	}
}


}
