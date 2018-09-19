//pb_ds 20160523
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;
#define RG register
#define set_file(File) freopen(File ".in", "r", stdin), freopen(File ".out", "w", stdout)
#define close_file() fclose(stdin), fclose(stdout)
#define For(i, a, b) for(RG int i = a, ___u = b; i <= ___u; ++i)
#define Dor(i, a, b) for(RG int i = b, ___d = a; i >= ___d; --i)
#define Rep(i, a, b) for(RG int i = a, ___u = b; i != ___u; ++i)
#define cmax(i, j) ((i) < (j) ? (i) = (j) : (i))
#define cmin(i, j) ((i) > (j) ? (i) = (j) : (i))
#define dmax(i, j) ((j) < (i) ? (i) : (j))
#define dmin(i, j) ((i) < (j) ? (i) : (j))
#define ddel(i, j) ((i) > (j) ? (i) - (j) : (j) - (i))
#define dabs(i) ((i) > 0 ? (i) : -(i))
#define dsqr(x) ((x) * (x))
#define lowbit(i) ((i) & -(i))
typedef unsigned uint;
typedef long long ll;
typedef unsigned long long ull;
typedef double db;
typedef long double ld;
 
namespace io
{
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
		RG char *iter = s, *S = io::S, *T = io::T;
		while(*iter = getc(), *iter == ' ' || *iter == '\n' || *iter == '\r')
			;
		while(*++iter = getc(), *iter && *iter != ' ' && *iter != '\n' && *iter != '\r')
			;
		*iter = 0;		
		io::S = S, io::T = T;
		return iter - s;
	}
	IL char get()
	{
		RG char ch;
		while(ch = getc(), ch == ' ' || ch == '\n' || ch == '\r')
			;
		return ch;
	}
	char buff[MaxOut], *iter = buff;
	template<class T> IL void P(RG T x, RG char ch = '\n')
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
	IL void output() {fwrite(buff, 1, iter - buff, stdout), iter = buff;}
}

#define pause() getchar()
#include <set>

namespace pb_ds
{	
	const int MaxN = 100010;
	struct Point
	{
		int x, y;
		IL bool operator < (RG const Point& p) const
		{
			return x < p.x || (x == p.x && y < p.y);
		}
		IL bool operator == (RG const Point& p) const
		{
			return x == p.x && y == p.y;
		}
		IL ll operator * (RG const Point& p) const
		{
			return (ll) x * p.y - (ll) y * p.x;
		}
		IL Point operator - (RG const Point &p) const
		{
			return (Point) {x - p.x, y - p.y};
		}
	} p[MaxN];
	IL ll value(RG const Point* a, RG const Point* b)
	{
		return (ll) (a->y + b->y) * ddel(a->x, b->x); 
	}

	IL uint random()
	{
		static uint A = 1 << 16 | 3, B = 33333331;
		static uint cur = 2341;
		return cur = cur * A + B;
	}

	int counter;

	namespace ConvexHull
	{
		struct Node *null, *node_tot, *node_lim;
		struct Result{Node *lef, *rig;} Skipped;
		struct Node
		{
			Node *lef, *rig;
			const Point *p, *l, *r;
			uint cnt;
			ll sum;
			IL void* operator new(RG size_t s) {return ++node_tot;}
			IL Node* copy() {return this > node_lim ? this : new Node(*this);}
			IL void init(RG const Point *_p) 
			{
				p = l = r = _p;
				lef = rig = null;
				cnt = 1, sum = 0;
			}
			IL void save()
			{
				cnt = 1, sum = 0;
				if(lef != null) l = lef->l, cnt += lef->cnt, sum += lef->sum + value(lef->r, p); else l = p;
				if(rig != null) r = rig->r, cnt += rig->cnt, sum += rig->sum + value(p, rig->l); else r = p;
			}	
			void print() //const
			{
				if(this == null) return;
				lef->print();
				printf("point (%d, %d)\n", p->x, p->y);
				rig->print();
			}
		} node_mem[MaxN * 130], *node_mov = null = node_tot = node_mem;
		Node *merge(RG Node *x, RG Node *y)
		{
			if(x == null) return y; 
			if(y == null) return x;
			RG Node *o;
			random() % (x->cnt + y->cnt) < x->cnt
				? (o = x->copy(), o->rig = merge(o->rig, y))
				: (o = y->copy(), o->lef = merge(x, o->lef));
			o->save();
			return o;
		}
		struct Data
		{
			Node *o; const Point *L, *R;
			IL bool lef_most() {return o->rig == null && !R;}
			IL bool rig_most() {return o->lef == null && !L;}
			IL Data go_lef() {return (Data) {o->lef, L, o->p};}
			IL Data go_rig() {return (Data) {o->rig, o->p, R};}
		};

		db m;
		const Point *p;
		template<class Comp> struct Hull	//Comp表示小于等于
		{
	 		static IL bool pig(RG const Point &l, RG const Point &p, RG const Point& r)
			{
				return Comp::comp((r - p) * (p - l), 0);
			}
			static IL int lef_val(RG const Data& d)
			{
				RG const Point *l = d.o->lef != null ? d.o->lef->r : d.L;
				RG const Point *r = d.o->rig != null ? d.o->rig->l : d.R;
				if(l && pig(*l, *(d.o->p), *p)) return 0;
				return (r && !pig(*(d.o->p), *r, *p)) + 1;
			}
			static IL int rig_val(RG const Data& d)
			{
				RG const Point *l = d.o->lef != null ? d.o->lef->r : d.L;
				RG const Point *r = d.o->rig != null ? d.o->rig->l : d.R;
				if(r && pig(*(d.o->p), *r, *p)) return 0;	
				return (l && !pig(*l, *(d.o->p), *p)) + 1;
			}
			static Result T_merge(RG Data L, RG Data R)
			{
				p = R.o->p; RG int A = lef_val(L);
				p = L.o->p; RG int B = rig_val(R);
				if(!A && !B) return T_merge(L.go_lef(), R.go_rig());
				if(!A) return T_merge(L.go_lef(), R);
				if(!B) return T_merge(L, R.go_rig());
				if(A == 1 && B == 1) {L.o = L.o->copy(); R.o = R.o->copy(); L.o->rig = R.o->lef = null; L.o->save(); R.o->save(); return (Result) {L.o, R.o};}
				RG Result res;
				RG bool k;
				if(B == 1) k = 1; else if(A == 1) k = 0;
				else
				{
					RG const Point &p11 = *L.o->p, &p12 = *(L.o->rig != null ? L.o->rig->l : L.R);
					RG const Point &p21 = *R.o->p, &p22 = *(R.o->lef != null ? R.o->lef->r : R.L);
					RG const db &x11 = p11.x, &y11 = p11.y, &x12 = p12.x, &y12 = p12.y;
					RG const db &x21 = p21.x, &y21 = p21.y, &x22 = p22.x, &y22 = p22.y;
					RG db c = (x11*(x21*(y12-y22)+x22*(y21-y12))+x12*(x21*(y22-y11)+x22*(y11-y21)))/((x11-x12)*(y21-y22)-(x21-x22)*(y11-y12));
					//Thanks to Mathematica
					k = c < m;
				}
				if(k) {res = T_merge(L.go_rig(), R); L.o = L.o->copy(); L.o->rig = res.lef; res.lef = L.o; L.o->save();}
				else  {res = T_merge(L, R.go_lef()); R.o = R.o->copy(); R.o->lef = res.rig; res.rig = R.o; R.o->save();}
				return res;
			}
			
			Node *T;
			IL void init() {T = null;}
			IL Hull operator + (RG const Hull& H) const
			{
				if(T == null) return H;
				if(H.T == null) return *this;
				node_lim = node_tot;
				m = 0.5 * (T->r->x + H.T->l->x);
				RG Result res = T_merge((Data) {T}, (Data) {H.T});
				return (Hull) {merge(res.lef, res.rig)};
			}
		};
	}
	struct LE{template<class A, class B> static IL bool comp(RG const A& a, RG const B& b) {return a <= b;}};
	struct GE{template<class A, class B> static IL bool comp(RG const A& a, RG const B& b) {return a >= b;}};

	multiset<int> heap[MaxN], empty;
	struct Data
	{
		ConvexHull::Hull<LE> upp; 
		ConvexHull::Hull<GE> low;
		IL void draw(RG const Data& A, RG const Data& B)
		{
			upp = A.upp + B.upp;
			low = A.low + B.low;
		}
		IL ll val() {return upp.T->sum - low.T->sum;}
	} T[262144], null_data;

	struct Input{bool c; Point p;} inp[MaxN];	
	int hash[MaxN];
	template<class T> 
	IL int find(RG T *h, RG int r, RG const T& v)
	{
		RG int l = 1, m;
		while(1)
		{
			m = (l + r) >> 1;
			if(h[m] == v) return m;
			h[m] < v ? l = m + 1 : r = m - 1;
		}
	}

	int qx, qv;
	bool qt;

	template<class T> IL void reinit(T &t)
	{
		t.~T();
		new (&t) T;
	}

	Point p_low[MaxN], p_upp[MaxN];
	void build(RG int i, RG int l, RG int r)
	{
		T[i] = null_data;
		if(l == r) reinit(heap[l]);
		else
		{
			RG int m = (l + r) >> 1;
			build(i << 1, l, m);
			build(i << 1 | 1, m + 1, r);
		}
	}
	void modify(RG int i, RG int l, RG int r)
	{
		if(l == r)
		{
			if(qt) heap[l].insert(qv); else heap[l].erase(heap[l].find(qv));
			if(heap[l].empty()) T[i] = null_data;
			else
			{
				p_upp[l] = (Point) {hash[l], *heap[l].rbegin()};
				T[i].upp.T = new ConvexHull::Node;
				T[i].upp.T->init(p_upp + l);
				p_low[l] = (Point) {hash[l], *heap[l].begin()};
				T[i].low.T = new ConvexHull::Node;
				T[i].low.T->init(p_low + l);
			}
		}
		else
		{
			RG int m = (l + r) >> 1;
			(qx <= m)
				? modify(i << 1, l, m)
				: modify(i << 1 | 1, m + 1, r);
			T[i].draw(T[i << 1], T[i << 1 | 1]);
		}
	}

	IL void main()
	{
		RG int (*F)() = io::read<int>;
		RG char (*G)() = io::get;
		For(_, 1, F())
		{
			ConvexHull::node_tot = ConvexHull::node_mem;
			null_data.upp.T = ConvexHull::null;
			null_data.low.T = ConvexHull::null;
			RG int n = F();
			For(i, 1, n)
			{
				inp[i] = (Input) {G() == '+', (Point) {F(), F()}};
				p[i] = inp[i].p;
			}
			sort(p + 1, p + n + 1);
			RG int nP = unique(p + 1, p + n + 1) - p - 1;
			RG int nH = 0; 
			For(i, 1, nP) if(!nH || p[i].x != hash[nH])
				hash[++nH] = p[i].x;
			build(1, 1, nH);
			RG int pid;
			RG ll ans;
			For(i, 1, n)
			{
				pid = find(p, nP, inp[i].p);
				qt = inp[i].c;
				qx = find(hash, nH, p[pid].x);
				qv = p[pid].y;
				modify(1, 1, nH);

				ans = T[1].val();
				io::P(ans >> 1, '.');
				*io::iter++ = (ans & 1) ? '5' : '0';
				*io::iter++ = '\n';
			}
		}
	}
}

int main()
{
	//set_file("equal");
	pb_ds::main();
	io::output();
}
/*

*/