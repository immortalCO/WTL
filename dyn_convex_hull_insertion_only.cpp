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
        while(ch = getc(), (ch < '0' || ch > '9') && ch != '-')        ;
        ch == '-' ? neg = 1 : ans = ch - '0';
        while(ch = getc(), '0' <= ch && ch <= '9') ans = ans * 10 + ch - '0';
        return neg ? -ans : ans;
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
///*lgg*/cout << "output = " << x << endl;
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

namespace pb_ds
{    
    const int MaxN = 100010, MAX_N = MaxN;
    struct Point
    {
        int x, y, i;
        IL bool operator < (RG const Point& p) const
        {
            return i < p.i;
            // return x < p.x || (x == p.x && y < p.y);
        }
        IL bool operator == (RG const Point& p) const
        {
            return x == p.x && y == p.y;
        }
        IL ll operator * (RG const Point& p) const
        {
            return (ll) x * p.y - (ll) y * p.x;
        }
        IL ll dot(RG const Point& p) const
        {
            return (ll) x * p.x + (ll) y * p.y;
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

    struct Node *null, *node_tot, *node_lim;
    struct Result{Node *lef, *rig;} Skipped;
    struct Node
    {
        Node *lef, *rig;
        const Point *p, *l, *r;
        uint cnt;
        IL void* operator new(RG size_t s) {return ++node_tot;}
        IL Node* copy() {return this > node_lim ? this : new Node(*this);}
        IL void init(RG const Point *_p) 
        {
            p = l = r = _p;
            lef = rig = null;
            cnt = 1;
        }
        IL void save()
        {
            cnt = 1;
            if(lef != null) l = lef->l, cnt += lef->cnt; else l = p;
            if(rig != null) r = rig->r, cnt += rig->cnt; else r = p;
        }    
        void print() const
        {
            if(this == null) return;
            lef->print();
            printf("point (%d, %d)\n", p->x, p->y);
            rig->print();
        }    
    } node_mem[MaxN * 20];
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

    int cho_i;
    ll cho_v;
    pair<int, ll> temp[MAX_N];

    template<class Comp> struct Hull    //Comp表示小于等于
    {
        Comp comp;
        Node *T;
        const Point *p;
        IL void init() {T = null;}
         IL bool pig(RG const Point &l, RG const Point &p, RG const Point& r)
        {
            return comp((r - p) * (p - l), 0);
        }
        Result T_split(RG Node *o, RG const Point *L, RG const Point *R)
        {
            if(o == null) return L && R && pig(*L, *p, *R)
                ? Skipped
                : (Result) {null, null};
            if(o->p->x == p->x) return comp(p->y, o->p->y)
                ? Skipped
                : (Result) {o->lef, o->rig};
            if(p->x < o->p->x)
            {
                RG Result res = T_split(o->lef, L, o->p);
                if(!res.lef) return Skipped;
                RG Node *O = o->copy();
                O->lef = res.rig;
                res.rig = O;
                O->save();
                return res;
            }
            else
            {
                RG Result res = T_split(o->rig, o->p, R);
                if(!res.lef) return Skipped;
                RG Node *O = o->copy();
                O->rig = res.lef;
                res.lef = O;
                O->save();
                return res;
            }
        }
        Node *T_proc_lef(RG Node *o, RG const Point *L, RG const Point *R)
        {
            RG const Point *l = o->lef != null ? o->lef->r : L;
            RG const Point *r = o->rig != null ? o->rig->l : R;
            if(l && pig(*l, *(o->p), *p)) return T_proc_lef(o->lef, L, 0);    //The whole except o->lef
            RG Node *O = o->copy();
            O->rig = (r && !pig(*(o->p), *r, *p)) ? T_proc_lef(O->rig, o->p, R) : null;
            O->save();
            return O;
        }
        Node *T_proc_rig(RG Node *o, RG const Point *L, RG const Point *R)
        {
            RG const Point *l = o->lef != null ? o->lef->r : L;
            RG const Point *r = o->rig != null ? o->rig->l : R;
            if(r && pig(*(o->p), *r, *p)) return T_proc_rig(o->rig, 0, R);
            RG Node *O = o->copy();
            O->lef = (l && !pig(*l, *(o->p), *p)) ? T_proc_rig(o->lef, L, o->p) : null;
            O->save();
            return O;
        }
        void T_query(RG Node *o) {
            RG ll tmp = p->dot(*o->p);
            if(tmp < cho_v) {cho_v = tmp; cho_i = o->p->i;}
            if(o->lef == null && o->rig == null) return;
            if(o->lef == null || o->rig == null) return T_query(o->lef != null ? o->lef : o->rig);
            T_query(p->dot(*o->lef->r) <= tmp ? o->lef : o->rig);
        }
        IL void query(RG const Point& _p) {
///*bgg*/printf("query (%d, %d)\n", _p.x,_p.y);
            if(T == null) return;
            p = &_p;
            T_query(T);
        }
        IL void insert(RG const Point& _p)
        {
            // node_lim = node_tot;
            p = &_p;
///*bgg*/printf("insert (%d, %d)\n", p->x, p->y);
            RG Result res = T_split(T, 0, 0);
            if(!res.lef) return;
///*lgg*/puts("inserted");
            if(res.lef != null) res.lef = T_proc_lef(res.lef, 0, 0);
            if(res.rig != null) res.rig = T_proc_rig(res.rig, 0, 0);
            RG Node *o = new Node; o->init(p);
            T = merge(merge(res.lef, o), res.rig);
///*lgg*/T->print();
        }
    };
    struct GE{template<class A, class B> IL bool operator() (RG const A& a, RG const B& b) const {return a >= b;}};
    Hull<GE> low;

    int f[MAX_N];
    int g(RG int i) {return i == f[i] ? i : f[i] = g(f[i]);}
    

    IL void main()
    {
        null = node_tot = node_mem;
        null->lef = null->rig;
        null->cnt = 0;
        null->p = null->l = null->r = 0;

        RG int (*F)() = io::read<int>;
        RG char (*G)() = io::get;
        For(i, 1, F())
        {
            RG int n = F(), N = n;
            For(i, 1, n) p[i] = (Point) {F(), F(), f[i] = i};
            ll ans = 0;
            for(int L = n - 1; L; ) {
///*bgg*/printf("new round L = %d\n", L);
                For(i, 1, n) p[i].i = g(p[i].i);
                sort(p + 1, p + n + 1);
                low.init();
                node_tot = node_mem;
                node_lim = node_mem;
                for(int l = 1, r; l <= n; l = r + 1) {
                    r = l;
                    while(p[l].i == p[r + 1].i) ++r;
                    cho_i = -1;
                    cho_v = 1e18;
                    For(i, l, r) low.query(p[i]);
                    temp[p[l].i] = make_pair(cho_i, cho_v);
                    For(i, l, r) low.insert(p[i]);
                }
                std::reverse(p + 1, p + n + 1);
                low.init();
                node_tot = node_mem;
                node_lim = node_mem;
                for(int l = 1, r; l <= n; l = r + 1) {
                    r = l;
                    while(p[l].i == p[r + 1].i) ++r;
                    cho_i = temp[p[l].i].first;
                    cho_v = temp[p[l].i].second;
                    For(i, l, r) low.query(p[i]);
                    if(g(p[l].i) != g(cho_i)) {
///*bgg*/printf("connect %d ~ %d v = %lld\n", g(p[l].i), cho_i, cho_v);
                        f[g(p[l].i)] = g(cho_i);
                        --L;
                        ans += cho_v;
                    }
                    For(i, l, r) low.insert(p[i]);
                }

            }
            io::P(ans);
        }
    }
}

int main()
{
    //set_file("defense");
    pb_ds::main();
    io::output();
}
/*
4 2 1
2
1 2
3 2
5
2
1 1
2
1 2
2

6.47
5.84
4.47
*/