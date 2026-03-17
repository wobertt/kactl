/**
 * Author: Robert Chung
 * Date: 2026-03-17
 * Description: Add line segments and query maximum values on [0, M).
 * To query minimum values, change the four spots marked `chg'.
 * Time: O(\log N)
 * Status: tested on https://cses.fi/problemset/result/16592813/
 */
#pragma once
struct LiChao {
  struct Line {
    int m, b;
    int eval(int x) const { return m*x + b; }
  };
  vector<Line> v;
  int M;
  LiChao(int m) : v(4*m+1, {0, LLONG_MIN}), // chg to max
          M(m) {}
  void insert(int l, int r, Line line) { // add segment [l, r)
    auto f = [&](auto &&f, int node,
                 int ql, int qr, Line line) -> void {
      if (r <= ql || qr <= l) return;
      auto m = midpoint(ql, qr);
      if (l <= ql && qr <= r) {
        if (line.eval(m) > v[node].eval(m)) // chg to <
          swap(line, v[node]);
        if (qr-ql == 1) return;
        if (v[node].m > line.m) // chg to <
          f(f, 2*node + 1, ql, m, line);
        else
          f(f, 2*node + 2, m, qr, line);
        return;
      }
      f(f, 2*node + 1, ql, m, line);
      f(f, 2*node + 2, m, qr, line);
    };
    f(f, 0, 0, M, line);
  }
  int query(int x) { // find max value at point x
    assert(0 <= x && x < M);
    auto f = [&](auto &&f, int node,
                 int l, int r) -> int {
      if (l+1 == r) return v[node].eval(x);
      auto m = midpoint(l, r);
      return max(v[node].eval(x), // chg to min
        x < m ? f(f, 2*node + 1, l, m)
             : f(f, 2*node + 2, m, r));
    };
    return f(f, 0, 0, M);
  }
};