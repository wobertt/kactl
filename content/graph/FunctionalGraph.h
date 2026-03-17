/**
 * Author: Aeren
 * Date: 2026-03-16
 * License: None
 * Source: https://github.com/programming-team-code/programming_team_code/blob/main/graphs/uncommon/functional_graph_processor.hpp
 * Description: 
 * Time: O(N)
 * Status: tested on https://cses.fi/problemset/result/16581553/, https://cses.fi/problemset/result/16581641/
 */
#pragma once
//   // 0 <= a[i] < n
//   auto [root_of, cycles, childs] = func_graph(a);
//   auto [cyc_id, cyc_pos] = root_of[v];
//   int root = cycles[cyc_id][cyc_pos];
//   bool is_on_cycle = (v == root);

//  root = first reachable node on cycle from v
//  t[v].childs = forest of reversed edges not in cycles
struct func_graph {
  vector<pii> root_of;
  vector<vi> cycles, childs;
  func_graph(const vi& a):
    root_of(sz(a)), childs(sz(a)) {
    vi vis(sz(a));
    rep(i,0,sz(a)) {
      if (!vis[i]) {
        int u = i;
        for (; !vis[u]; u = a[u]) vis[u] = 1;
        if (vis[u] == 1)
          for (cycles.pb({}); vis[u] == 1; u = a[u]) {
            root_of[u] = {sz(cycles) - 1,
                          sz(cycles.back())};
            cycles.back().pb(u);
            vis[u] = 2;
          }
        for (int v = i; vis[v] == 1; v = a[v]) {
          root_of[v] = root_of[u];
          childs[a[v]].pb(v);
          vis[v] = 2;
        }
      }
    }
  }
};