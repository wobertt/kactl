/**
 * Author: Yunzhe Qiao
 * Date: 2026-03-03
 * License: None
 * Description: Avoid pitfalls - swap u,v if u > v. Put end time = t-1 if an edge is erased at time t.
 * Handle edges that were never removed.
 * Status: tested on https://codeforces.com/gym/100551/problem/A, cses.fi/problemset/task/2133/
 */
#pragma once

#include "UnionFindRollback.h"

struct DynConn {
    int T; // DynConn(T) works over times [0..T]
    vector<vector<pii>> tree;
    vector<int> queries; // change query type as needed
    vi ans; // 1 = Yes, 0 = No, -1 = "no query at this time"
    DynConn(int t) : T(t+2), tree(4*T), queries(4*T, -1),
        ans(T, -1) {}
    // [ql, qr] = [start time, end time] of edge
    void add_edge(int ql, int qr, pii edge) {
        if (ql > qr) return;
        auto f = [&](auto f, int node, int l, int r) -> void {
            if (ql > r || qr < l) return;
            if (ql <= l && r <= qr) {
                tree[node].push_back(edge);
                return;
            }
            int mid = (l+r) / 2;
            f(f, 2*node, l, mid);
            f(f, 2*node + 1, mid+1, r);
        };
        f(f, 1, 0, T);
    }
    void add_query(int time) { // add params as needed
        queries[time] = 0;
    }
    void solve(auto& dsu) {
        auto f = [&](auto f, int node, int l, int r) -> void {
            int t = dsu.time();
            for (auto [a, b] : tree[node]) dsu.join(a, b);
            if (l == r) {
                if (queries[l] != -1) { // change as needed
                    ans[l] = sz(dsu.e)-1 - dsu.time() / 2;
                }
            } else {
                int mid = (l + r) / 2;
                f(f, 2*node, l, mid);
                f(f, 2*node + 1, mid + 1, r);
            }
            dsu.rollback(t);
        };
        f(f, 1, 0, T);
    }
};