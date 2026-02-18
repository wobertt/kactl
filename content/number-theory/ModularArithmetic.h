/**
 * Author: Benjamin Qi
 * Source: https://github.com/bqi343/cp-notebook/blob/master/Implementations/content/number-theory%20(11.1)/Modular%20Arithmetic/ModIntShort.h
 * Description: Modular arithmetic. Assumes $MOD$ is prime.
 * Usage: mi a = MOD+5; inv(a); // 400000003
 */
#pragma once

template<int M> struct mint {
    static const int mod = M;
    int v;
    explicit operator int() const { return v; }
    mint():v(0) {}
    mint(ll _v):v(_v%M) { v += (v<0)*M; }
    mint& operator+=(mint o) {
        if ((v += o.v) >= M) v -= M;
        return *this; }
    mint& operator-=(mint o) {
        if ((v -= o.v) < 0) v += M;
        return *this; }
    mint& operator*=(mint o) {
        v = (ll)v*o.v%M; return *this; }
    friend mint pow(mint a, ll p) { assert(p >= 0);
        return p==0?1:pow(a*a,p/2)*(p&1?a:1); }
    friend mint inv(mint a) { assert(a.v != 0);
        return pow(a,M-2); }
    friend mint operator+(mint a, mint b) { return a += b; }
    friend mint operator-(mint a, mint b) { return a -= b; }
    friend mint operator*(mint a, mint b) { return a *= b; }
    friend istream& operator>>(istream& is, mint& a) { // &
        return is >> a.v; }
    friend ostream& operator<<(ostream& os, mint a) { // no &
        return os << a.v; }
};
using mi = mint<0>; // change to something else
