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

#define pause() getchar()

namespace pb_ds
{	
	const int MaxN = 1000010;
	const ll inf = 1e18;


	struct Data;
	struct LightData;
	struct SelfData;

	struct LightData {
		ll sum;
		IL void add(const Data& D);
		IL void del(const Data& D);
	}	NULL_LIGHT_DATA = {0ll};
	struct Data {
		ll a, b;
		IL Data operator + (const Data& R);
	}	NULL_DATA = {0ll, inf};
	struct SelfData {
		Data val;
		IL Data connect(const LightData& L);
	};

	IL Data Data::operator + (const Data& R) {
		return (Data) {dmin(inf, R.a + a), dmin(R.b + a, b)};
	}
	IL Data SelfData::connect(const LightData& L) {
		return (Data) {val.a + L.sum, val.b};
	}
	IL void LightData::add(const Data& D) {sum += dmin(D.a, D.b);}
	IL void LightData::del(const Data& D) {sum -= dmin(D.a, D.b);}

	struct Node *null;
	struct Node
	{
		Node *fa, *son[2];
		bool rev;
		Data sum[2];
		SelfData self;
		LightData light;

		IL void init() {
			fa = null; son[0] = null; son[1] = null; rev = 0;
			sum[0] = NULL_DATA; sum[1] = NULL_DATA;
			light = NULL_LIGHT_DATA;
		}
		IL Node* expose() {access(); splay(0); return this;}

		IL void save() {
			Data tmp = self.connect(light);
			sum[0] = son[0]->sum[0] + tmp + son[1]->sum[0];
			sum[1] = son[1]->sum[1] + tmp + son[0]->sum[1];
		}
		IL void do_rev()
		{
			rev ^= 1;
			swap(son[0], son[1]);
			swap(sum[0], sum[1]);
		}
		IL void down() 
		{
			if(!rev) return;
			if(son[0] != null) son[0]->do_rev();
			if(son[1] != null) son[1]->do_rev();
			rev = 0;
		}
		IL bool type() {return fa->son[1] == this;}
		IL bool check() {return fa->son[type()] == this;}
		IL void rotate()
		{
			RG Node *f = fa; RG bool d = type();
			(fa = f->fa), f->check() ? fa->son[f->type()] = this : 0;
			(f->son[d] = son[!d]) != null ? son[!d]->fa = f : 0;
			(son[!d] = f)->fa = this, f->save();
		}
		inline void preview() {if(check()) fa->preview(); down();}
		IL void splay(RG bool need = 1)
		{
			if(need) preview();
			for(; check(); rotate()) if(fa->check())
				(type() != fa->type()) ? rotate() : fa->rotate();
			save();
		}
		IL void access()
		{
			for(RG Node *i = this, *j = null; i != null; i = (j = i)->fa)
			{
				i->splay();
				if(i->son[1] != null) i->light.add(i->son[1]->sum[0]);
				if(j != null) i->light.del(j->sum[0]);
				i->son[1] = j;
				i->save();
			}
		}
		IL Node* find_l() {
			RG Node *p = this;
			while(p->down(), p->son[0] != null) p = p->son[0]; 
			p->splay(0);
			return p;
		}
		IL void make_root() {expose()->do_rev();}
		IL void link(RG Node *o) {
			make_root();
			(fa = o)->expose()->light.add(sum[0]);
			o->save();
		}
		IL void cut(RG Node *o) {
			make_root(), o->access(), splay(0), son[1] = o->fa = null, save();
		}
		IL Data subtree_data() {
			access();
			return self.connect(light);
		}
	} mem[MaxN];

	IL void cut(RG int x, RG int y) {mem[x].cut(mem + y);}
	IL void link(RG int x, RG int y) {mem[x].link(mem + y);}
	int deg[MaxN];

	IL void main()
	{
		null = mem;
		null->init();

		RG int (*F)() = io::read<int>;
		RG ll (*G)() = io::read<ll>;
		RG int n = F();
		For(i, 1, n) {mem[i].init(); mem[i].self.val.b = G(); mem[i].save();}
		For(i, 2, n) {
			RG int x = F(), y = F();
			++deg[x]; ++deg[y];
			link(x, y);
		}
		For(i, 2, n) if(deg[i] == 1) {
			mem[i].expose()->self.val.a = inf; mem[i].save();
		}
		mem[1].make_root();
		RG char cmd[3];
		For(i, 1, F()) {
			io::gets(cmd);
			if(cmd[0] == 'Q') {
				io::print((mem[F()].subtree_data() + (Data) {inf, 0ll}).b);
			} else {
				RG int p = F();
				mem[p].expose()->self.val.b += G();
				mem[p].save();
			}
		}
	}
}

int main()
{
	//set_file("light");
	pb_ds::main();
}
