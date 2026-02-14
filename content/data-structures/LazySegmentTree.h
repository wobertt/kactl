/**
 * Author: Joseph Durie
 * Date: 2020-10-11
 * Description: Remember that idT and idU must be idempotent
 * Time: O(\log N)
 */
#pragma once

struct lztree {
	typedef ll T; T idT = {}; // node type
	typedef ll U; U idU = {}; // update type
	// combining segtree nodes a and b
	T f(T a, T b) { return a + b; }
	// applying updates f and g (f compose g)
	U g(U f, U g) { return f + g; }
	// applying update f to segtree node a
	T h(U f, T a) { return a + f; }
	int n;
	vector<T> t;
	vector<U> d;
	lztree(int n) : n(n), t(2*n, idT), d(n, idU) {}
	void calc(ll p) { t[p] = h(d[p], f(t[p * 2], t[p * 2 + 1])); }
	void apply(ll p, U v) {
		t[p] = h(v, t[p]);
		if(p < n) d[p] = g(v, d[p]);
	}
	void push(ll p) {
		p += n;
		for(int s = 24; s > 0; s--) { // need tree height <= 24
			ll i = p >> s;
			if(d[i] != idU) {
				apply(i * 2, d[i]);
				apply(i * 2 + 1, d[i]);
				d[i] = idU;
			}
		}
	}
	void set(ll p, T v) { // point set arr[p] = v
		push(p);
		t[p += n] = v;
		while(p > 1) calc(p /= 2);
	}
	T get(ll p) { // return arr[p]
        push(p);
        return t[p + n];
    }
	void update(ll l, ll r, U v) { // range update [l, r) using v
		push(l), push(r - 1);
		bool cl = false, cr = false;
		for(l += n, r += n; l < r; l /= 2, r /= 2) {
			if(cl) calc(l - 1);
			if(cr) calc(r);
			if(l & 1) apply(l++, v), cl = true;
			if(r & 1) apply(--r, v), cr = true;
		}
		for(--l; r; l /= 2, r /= 2) {
			if(cl) calc(l);
			if(cr) calc(r);
		}
	}
	T query(ll l, ll r) { // range query [l, r)
		push(l), push(r - 1);
		T resl = idT, resr = idT;
		for(l += n, r += n; l < r; l /= 2, r /= 2) {
			if(l & 1) resl = f(resl, t[l++]);
			if(r & 1) resr = f(t[--r], resr);
		}
		return f(resl, resr);
	}
};