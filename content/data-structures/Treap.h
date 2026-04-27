/**
 * Author: Robert Chung
 * Date: 2026-04-27
 * Source: folklore
 * Description: Zero-indexed implicit treap. Change the 6 'todo' items
 * when using.
 * Time: $O(\log N)$
 * Status: untested
 */
#pragma once


namespace Treap {
    struct Node;
    using ptr = Node*;
    
    // todo: max number of nodes
    constexpr int MAX_SZ = 200'000;
    // todo: value type
    using T = int;
    constexpr T idT{0};
    // todo: update type
    using U = bool;
    constexpr U idU{false};
    
    struct Node {
        Node *l, *r;
        int siz;
        unsigned long long pri; // pri is max heap
        T val, agg;
        U u;
        explicit Node(T val=idT) : l(nullptr), r(nullptr), siz(1), pri(rng()),
            val(val), agg(val), u(idU) {}
    };
    
    // todo: combine values 'a' and 'b'
    T t_combine(T a, T b) { return a+b; }
    // todo: combine updates 'f' and 'g' (f compose g)
    U u_combine(U f, U g) { return f^g; }
    // todo: apply update 'u' to node 'a'
    void _apply_upd(ptr a, U u) {
        if (!a) return;
        swap(a->l, a->r);
        a->u = u_combine(u, a->u);
    }
    

    Node arr[MAX_SZ]; int _last = 0;
    int _getsiz(ptr a) { return a ? a->siz : 0; }
    // fix values for parent by pulling from children
    ptr _pull(ptr a) {
        // assert(a != nullptr);
        a->siz = 1 + _getsiz(a->l) + _getsiz(a->r);
        a->agg = t_combine(t_combine(
                        a->l ? a->l->agg: idT,
                        a->val),
                        a->r ? a->r->agg : idT);
        return a;
    }
    // propagate values down to children
    void _push(ptr a) {
        // assert(a != nullptr);
        if (a->u == idU) return;
        _apply_upd(a->l, a->u);
        _apply_upd(a->r, a->u);
        a->u = idU;
    }
    // concatenate sequences A and B
    [[nodiscard]] ptr merge(ptr a, ptr b) {
        if (!a) return b;
        if (!b) return a;
        _push(a); _push(b);
        if (a->pri > b->pri) {
            a->r = merge(a->r, b);
            return _pull(a);
        }
        b->l = merge(a, b->l);
        return _pull(b);
    }
    // split into [0, k) and [k, inf)
    [[nodiscard]] pair<ptr, ptr> split(ptr root, int k) {
        if (!root) return {};
        _push(root);
        if (_getsiz(root->l) >= k) {
            auto [l, r] = split(root->l, k);
            root->l = r;
            return {l, _pull(root)};
        }
        auto [l, r] = split(root->r, k - _getsiz(root->l) - 1);
        root->r = l;
        return {_pull(root), r};
    }
    // split into [0, l), [l, r), [r, inf)
    [[nodiscard]] array<ptr, 3> split3(ptr root, int l, int r) {
        assert(l <= r);
        auto [r1, r23] = split(root, l);
        auto [r2, r3] = split(r23, r-l);
        return {r1, r2, r3};
    }
    // range query a[l..r) using t_combine
    [[nodiscard]] T range_query(ptr &a, int l, int r) {
        assert(0 <= l && l <= r && r <= _getsiz(a));
        auto [a1, a2, a3] = split3(a, l, r);
        T ans = a2->agg;
        a = merge(merge(a1, a2), a3);
        return ans;
    }
    // range apply 'u' to a[l..r)
    [[nodiscard]] ptr range_apply(ptr a, int l, int r, U u) {
        assert(0 <= l && l <= r && r <= _getsiz(a));
        if (l == r) return a;
        auto [a1, a2, a3] = split3(a, l, r);
        _apply_upd(a2, u);
        return merge(merge(a1, a2), a3);
    }
    [[nodiscard]] ptr build(const vector<T> &vals) {
        vector<unsigned long long> pris(sz(vals));
        for (auto &x : pris) x = rng();
        sort(rall(pris)); int ind = 0;

        auto f = [&](auto f, int l, int r) -> ptr {
            if (l == r) return {};
            int mid = midpoint(l, r);
            ptr node = &(arr[_last++] = Node(vals[mid]));
            node->pri = pris[ind++];

            node->l = f(f,  l, mid);
            node->r = f(f, mid+1, r);
            return _pull(node);
        };
        return f(f, 0, sz(vals));
    }
}
using namespace Treap;