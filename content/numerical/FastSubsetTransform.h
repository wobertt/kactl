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

using vl = vector<ll>;
void FST(vl& a, bool inv) {
    for (int n = sz(a), step = 1; step < n; step *= 2) {
        for (int i = 0; i < n; i += 2 * step) rep(j,i,i+step) {
            ll &u = a[j], &v = a[j + step]; tie(u, v) =
                // inv ? pair(v - u, u) : pair(v, u + v); // AND /// include-line
                // inv ? pair(v, u - v) : pair(u + v, u); // OR /// include-line
                pair(u + v, u - v);                   // XOR
        }
    }
    if (inv) for (ll& x : a) x /= sz(a); // XOR
}
vl conv(vl a, vl b) {
    FST(a, 0); FST(b, 0);
    rep(i,0,sz(a)) a[i] *= b[i];
    FST(a, 1); return a;
}
int psgn(int x) { return __builtin_popcount(x)&1 ? -1 : 1; }
// a[i] += val (for transformed a)
void point_add(vl &a, int i, ll val) {
    rep(j,0,sz(a))
        // a[j] += val * ((i|j) == sz(a)-1); // AND /// include-line
        // a[j] += val * ((i&j) == 0); // OR /// include-line
        a[j] += val * psgn(i&j); // XOR
}
// return a[i] (for transformed a)
ll point_eval(const vl &a, int i) {
    ll res = 0;
    rep(j,0,sz(a))
        // res += a[j] * ((i&j) == 0) * psgn(sz(a)-1-(i|j)); // AND /// include-line
        // res += a[j] * ((i|j) == sz(a)-1) * psgn(i&j); // OR /// include-line
        res += a[j] * psgn(i&j); // XOR
    res /= sz(a); // XOR
    return res;
}