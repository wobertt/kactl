/**
 * Author: Lucian Bicsi
 * Date: 2015-06-25
 * License: GNU Free Documentation License 1.2
 * Source: csacademy
 * Description: Transform to a basis with fast convolutions of the form
 * $\displaystyle c[z] = \sum\nolimits_{z = x \oplus y} a[x] \cdot b[y]$,
 * where $\oplus$ is one of AND, OR, XOR. The size of $a$ must be a power of two.
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

void FST(vi& a, bool inv) {
	for (int n = sz(a), step = 1; step < n; step *= 2) {
		for (int i = 0; i < n; i += 2 * step) rep(j,i,i+step) {
			int &u = a[j], &v = a[j + step]; tie(u, v) =
				// inv ? pii(v - u, u) : pii(v, u + v); // AND /// include-line
				// inv ? pii(v, u - v) : pii(u + v, u); // OR /// include-line
				pii(u + v, u - v);                   // XOR
		}
	}
	if (inv) for (int& x : a) x /= sz(a); // XOR
}
vi conv(vi a, vi b) {
	FST(a, 0); FST(b, 0);
	rep(i,0,sz(a)) a[i] *= b[i];
	FST(a, 1); return a;
}
int psgn(int x) { return __builtin_popcount(x)&1 ? -1 : 1; }
// a[i] += val (for transformed a)
void point_add(vi &a, int i, int val) {
    rep(j,0,sz(a))
        // a[j] += val * ((i|j) == sz(a)-1); // AND /// include-line
        // a[j] += val * ((i&j) == 0); // OR /// include-line
        a[j] += val * psgn(i&j); // XOR
}
// return a[i] (for transformed a)
int point_eval(const vi &a, int i) {
    int res = 0;
    rep(j,0,sz(a))
        // res += a[j] * ((i&j) == 0) * psgn(sz(a) - 1 - (i|j)); // AND /// include-line
        // res += a[j] * ((i|j)==sz(a)-1) * psgn(i&j); // OR /// include-line
        res += a[j] * psgn(i&j); // XOR
    res /= sz(a); // XOR
    return res;
}