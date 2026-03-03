/**
 * Author: Mihnea Brebenel
 * Date: 2026-03-03
 * License: None
 * Source: https://usaco.guide/adv/BCC-2CC
 * Description: Given an undirected adj. list `g', 0 or 1 indexed (`s'),
 * computes a block-cut tree. `id' maps nodes in `g' to nodes in `t'.
 * Time: $O(N+M)$
 * Status: tested at https://cses.fi/problemset/task/1705/
 */
#pragma once

auto blockCutTree = [](vector<vi> &g, int s) {
    int n = sz(g), time = 0; vector<bool> is_cutpoint(n);
    vector<vi> comps; vi stk, num(n), low(n), id(n);
    // find BCCs
    auto dfs = [&](auto dfs, int u, int p) -> void {
        num[u] = low[u] = ++time;
        stk.pb(u);
        for (int x : g[u]) {
            if (x == p) continue;
            if (num[x]) low[u] = min(low[u], num[x]);
            else {
                dfs(dfs, x, u);
                low[u] = min(low[u], low[x]);
                if (low[x] >= num[u]) {
                    is_cutpoint[u] = num[u] > 1 || num[x] > 2;
                    comps.pb({u});
                    while (comps.back().back() != x) {
                        comps.back().pb(stk.back());
                        stk.pop_back();
                    }
                }
            }
        }
    };
    dfs(dfs, s, -1);
    // build block-cut tree
    vector<vi> t(1); int nid = 0;
    rep(u,s,n)
        if (is_cutpoint[u]) id[u] = nid++, t.pb({});
    for (auto &comp : comps) {
        int node = nid++;
        t.pb({});
        for (int u : comp)
            if (!is_cutpoint[u]) id[u] = node;
            else t[node].pb(id[u]), t[id[u]].pb(node);
    }
    return tuple(t, is_cutpoint, id);
};