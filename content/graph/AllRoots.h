/**
 * Author: Shashank Bhatt
 * License: CC0
 * Description: 0-indexed reroot DP assuming associativity and commutativity of +.
 * operator+ combines children into a single representation.
 * promote() finds a child's contribution to its parent.
 * Time: O(N).
 * Status: Tested on https://cses.fi/problemset/result/16593762/ and https://cses.fi/problemset/result/16593328/.
 */
#pragma once

struct ART {
  int ans = 0; // max distance to another node
  ART operator+(const ART& o) const {
    return {max(ans, o.ans)};
  }
  ART promote(int x, int e) const {
    if (e == -1) return *this;
    return {ans+1}; // x = node idx, e = edge idx
  }
};
template<class T>
struct AllRoots {
  int n; vector<vector<pii>> adj;
  vector<vector<T>> suf; vector<T> res;
  AllRoots(vector<pii>& el) : n(sz(el)+1), adj(n), suf(n), res(n) {
    for (int ei = -1; auto [a, b] : el) { ei++;
      adj[a].pb({b,ei});
      adj[b].pb({a, ei});
    }
    for (auto &s : suf) s.resize(1);
  }
  T down(int x, int p = -1) {
    for (auto &c : adj[x])
      if(c.F == p) swap(c, adj[x][0]);
    for(int i = sz(adj[x])-1; i > -(p<0); --i)
      suf[x].pb(down(adj[x][i].F,x) + suf[x].back());
    return (p == -1 ? T{} : suf[x].back().promote(x,adj[x][0].S));
  }
  void up(int x, int p = -1) {
    rep(i, p != -1, sz(adj[x])) {
      auto[y,ei] = adj[x][i];
      suf[x].pop_back();
      res[y] = (res[x] + suf[x].back()).promote(x,ei);
      res[x] = res[x] + suf[y].back().promote(y,ei);
      up(y, x);
    }
  }
  static vector<T> calc(vector<pii>& el) {
    AllRoots ar(el);
    ar.down(0);
    ar.up(0);
    rep(i,0,ar.n) ar.res[i] = ar.res[i].promote(i,-1);
    return ar.res;
  }
};