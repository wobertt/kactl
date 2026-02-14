/**
 * Author: Benjamin Qi, 12tqian
 * Date: 2026-01-30
 * Source: https://12tqian.github.io/cp-library/library/graphs/flows/min-cost-max-flow.hpp.html
 *
 * Description: Minimum-cost maximum flow, assumes no negative cycles. 
 	* It is possible to choose negative edge costs such that the first 
 	* run of Dijkstra is slow, but this hasn't been an issue in the past. 
 	* Edge weights $\ge 0$ for every subsequent run. To get flow through 
 	* original edges, assign ID's during \texttt{ae}.
 * Time: Ignoring first run of Dijkstra, $O(FM\log M)$ 
 	* if caps are integers and $F$ is max flow. */
#pragma once
template <class F, class C> // F = flow type, C = cost type
struct MCMF {
    struct Edge { int to; F flow, cap; C cost; };

    int n;
    vector<C> p, dist;
    vector<int> pre;
    vector<Edge> edges;
    vector<vector<int>> adj;

    const C INF = numeric_limits<C>::max() / 4;

    MCMF(int n) : n(n), p(n), dist(n), pre(n), adj(n) {}

    void ae(int u, int v, F cap, C cost) {
        assert(cap >= 0);
        adj[u].push_back(sz(edges));
        edges.push_back({v, 0, cap, cost});
        adj[v].push_back(sz(edges));
        edges.push_back({u, 0, 0, -cost});
    }

    bool path(int s, int t) {
        for (int i = 0; i < n; i++)
            dist[i] = INF;
        using T = pair<C, int>;
        priority_queue<T, vector<T>, greater<>> todo;
        todo.push({dist[s] = 0, s});
        while (!todo.empty()) {
            T x = todo.top();
            todo.pop();
            if (x.first > dist[x.second])
                continue;
            for (auto& eid : adj[x.second]) {
                const Edge& e = edges[eid];
                if (e.flow < e.cap && dist[e.to]
					> x.first + e.cost + p[x.second] - p[e.to]) {
                    dist[e.to] = x.first + e.cost + p[x.second] - p[e.to];
                    pre[e.to] = eid;
                    todo.push({dist[e.to], e.to});
                }
            }
        }
        return dist[t] != INF;
    }

    pair<F, C> calc(int s, int t) {
        assert(s != t);
        for (int unused = 0; unused < n; unused++)
            for (int eid = 0; eid < (int)edges.size(); eid++) {
                const Edge& e = edges[eid];
                if (e.cap)
                    p[e.to] = min(p[e.to], p[edges[eid ^ 1].to] + e.cost);
            }
        F totflow = 0;
        C totcost = 0;
        while (path(s, t)) {
            for (int i = 0; i < n; i++)
                p[i] = min(p[i] + dist[i], INF);
            F df = numeric_limits<F>::max() / 4;
            for (int x = t; x != s; x = edges[pre[x] ^ 1].to) {
                const Edge& e = edges[pre[x]];
                df = min(df, e.cap - e.flow);
            }
            totflow += df;
            totcost += (p[t] - p[s]) * df;
            for (int x = t; x != s; x = edges[pre[x] ^ 1].to)
                edges[pre[x]].flow += df, edges[pre[x] ^ 1].flow -= df;
        }
        return {totflow, totcost};
    }
};