#include <bits/stdc++.h>
using namespace std;
template <typename T>
struct _ptrCntr{
    T v; int c;
    _ptrCntr(const T& _v):v(_v){ c = 0;}
};
template <typename T>
struct Sptr{
    _ptrCntr<T> *p;
    T* operator->(){ return &p->v; }
    T& operator* (){ return  p->v; }
    operator _ptrCntr<T>*(){ return p; }
    Sptr& operator = (const Sptr<T>& t){
        if (p && !--p->c) delete p;
        (p = t.p) && ++p->c;
        return *this;
    }
    Sptr(_ptrCntr<T> *t = 0) : p(t){ p && ++p->c; }
    Sptr(const Sptr& t) : p(t.p){ p  && ++p->c; }
    ~Sptr(){ if (p && !--p->c) delete p;}
};
template <typename T>
inline Sptr<T> _new(const T& u){
    return Sptr<T>(new _ptrCntr<T>(u));
}
#define PNN pair<Sptr<Node>, Sptr<Node> >
#define MP make_pair
#define F first
#define S second
const int MAXK = 5e4 + 5;
int d;
struct PersistentTreap{
    struct Node{
        Sptr<Node> l, r;
        char c; int sz;
        Node(char _c = 0) : l(NULL), r(NULL){
            c = _c, sz = 1;
        }
    };
    Sptr<Node> ver[MAXK];
    int verCnt;
    PersistentTreap(){ verCnt = 0; }
    inline int size(Sptr<Node> &u){
        return u ? u->sz : 0;
    }
    inline Sptr<Node> pull(Sptr<Node> &u){
        u->sz = 1 + size(u->l) + size(u->r);
        return u;
    }
    inline Sptr<Node> copy(Sptr<Node> &u){
        return _new(*u);
    }
    Sptr<Node> merge(Sptr<Node> &T1, Sptr<Node> &T2){
        if (!T1 || !T2) return T1 ? T1 : T2;
        Sptr<Node> res;
        if (rand() % (size(T1) + size(T2)) < size(T1)){
            res = copy(T1);
            res->r = merge(T1->r, T2);
        }else{
            res = copy(T2);
            res->l = merge(T1, T2->l);
        }
        return pull(res);
    }
    PNN split(Sptr<Node> &T, int k){
        if (!T) return MP(Sptr<Node>(NULL), Sptr<Node>(NULL));
        Sptr<Node> res = copy(T);
        if (size(T->l) < k){
            PNN tmp = split(T->r, k - 1 - size(T->l));
            res->r = tmp.F;
            return MP(pull(res), tmp.S);
        }else{
            PNN tmp = split(T->l, k);
            res->l = tmp.S;
            return MP(tmp.F, pull(res));
        }
    }
    void insert(char *s, int p){
        verCnt++, ver[verCnt] = ver[verCnt-1];
        PNN tmp = split(ver[verCnt], p);
        for (int i = 0 ; s[i] ; i++){
            Sptr<Node> target = _new(Node(s[i]));
            tmp.F = merge(tmp.F, target);
        }
        ver[verCnt] = merge(tmp.F, tmp.S);
    }
    void remove(int p, int c){
        verCnt++, ver[verCnt] = ver[verCnt-1];
        PNN tmp_1 = split(ver[verCnt], p-1);
        PNN tmp_2 = split(tmp_1.S, c);
        ver[verCnt] = merge(tmp_1.F, tmp_2.S);
    }
    void query(int v, int p, int c){
        PNN tmp_1 = split(ver[v], p-1);
        PNN tmp_2 = split(tmp_1.S, c);
        Print(tmp_2.F); cout << '\n';
    }
    void Print(){
        for (int i = 0 ; i <= verCnt ; i++){
            cout << "Treap " << i << ":\n";
            Print(ver[i]);
            cout << "\n\n";
        }
    }
    void Print(Sptr<Node> &u){
        if (!u) return ;
        Print(u->l);
        cout << u->c;
        if (u->c == 'c') d++;
        Print(u->r);
    }
};
const int MAXLEN = 1e6 + 5;
int main(){
    srand(time(NULL));
    d = 0;
    Sptr<PersistentTreap> sol = _new(PersistentTreap());
    int n; cin >> n; while (n--){
        int op; cin >> op;
        char input[MAXLEN];
        int v, p, c;
        switch (op){
            case 1:
                cin >> p >> input; p -= d;
                sol->insert(input, p);
                break;
            case 2:
                cin >> p >> c; p -= d, c -= d;
                sol->remove(p, c);
                break;
            case 3:
                cin >> v >> p >> c;
                v -= d, p -= d, c -= d;
                sol->query(v, p, c);
                break;
        }
    }
}
