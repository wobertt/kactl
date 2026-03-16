/**
 * Author: Benjamin Qi, Oleksandr Kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://codeforces.com/blog/entry/53170, https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Trees%20(10)/HLD%20(10.3).h
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Takes as input the adjacency list (comment out (*) in dfsSz if no parent edges),
 * a list of initial values $v$, and the root. VALS\_EDGES being true means
 * that values are stored in the edges, as opposed to the nodes.
 * Time: O((\log N)^2)
 * Status: tested on https://cses.fi/problemset/result/16580118/
 */
#pragma once

#include "../data-structures/LazySegmentTree.h"

template <bool VALS_EDGES> struct HLD {
    int N, tim = 0;
    vector<vi> adj;
    vi par, siz, rt, pos;
    lztree tree;
    HLD(vector<vi> &adj, vector<T> v, int root)
        : N(sz(adj)), adj(adj), par(N, -1), siz(N, 1),
          rt(N,root),pos(N),tree(N) {
        dfsSz(root); dfsHld(root);
        rep(i,root,sz(v)) tree.set(pos[i], v[i]); // init vals
    }
	void dfsSz(int v) {
		for (int& u : adj[v]) {
			adj[u].erase(find(all(adj[u]), v)); // (*)
			par[u] = v;
			dfsSz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
		}
	}
	void dfsHld(int v) {
		pos[v] = tim++;
		for (int u : adj[v]) {
			rt[u] = (u == adj[v][0] ? rt[v] : u);
			dfsHld(u);
		}
	}
	template <class B> void process(int u, int v, B op) {
		for (;; v = par[rt[v]]) {
			if (pos[u] > pos[v]) swap(u, v);
			if (rt[u] == rt[v]) break;
			op(pos[rt[v]], pos[v] + 1);
		}
		op(pos[u] + VALS_EDGES, pos[v] + 1);
	}
    T get(int u) { // point set is similar
        return tree.get(pos[u]);
    }
	void modifyPath(int u, int v, U val) {
		process(u, v, [&](int l, int r) { tree.update(l, r, val); });
	}
    T queryPath(int u, int v) { // Modify depending on problem
        T res = idT; // unit
        process(u, v, [&](int l, int r) { // res = op(res, query)
                res = f(res, tree.query(l, r));
        });
        return res;
    }
    T querySubtree(int v) { // modifySubtree is similar
        return tree.query(pos[v] + VALS_EDGES, pos[v] + siz[v]);
    }
};
