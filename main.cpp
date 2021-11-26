#define _CRT_SECURE_NO_WARNINGS

#include <functional>
#include <cstdio>
#include <vector>
#include <cmath>

#define pii pair<int, int>
#define vvp vector<vector<pii>> 
#define vb vector<bool>
#define vi vector<int>
#define vii vector<pii>
using namespace std;

template <class _Ty, class _Compare = less<typename vector<_Ty>::value_type>>
class Heap {
    using _Container        = vector<_Ty>;
    using const_reference   = typename _Container::const_reference;
    using _Iter             = typename _Container::iterator;
    using size_type         = typename _Container::size_type;
    using value_type        = typename _Container::value_type;
private:
    int left_child_idx(_Iter a) { return (distance(c.begin(), a) << 1) + 1; }
    int right_child_idx(_Iter a) { return (distance(c.begin(), a) << 1) + 2; }
    _Iter left_child(_Iter a) { return c.begin() + left_child_idx(a); }
    _Iter right_child(_Iter a) { return c.begin() + right_child_idx(a); }


    int _promote(int _nextStop, _Iter vacant, int cur_h) {
		/*
		go down towart bigger child each step from _hStop to h.
        */
        int _finalStop_idx; // 내려갈 위치
        if (cur_h <= _nextStop) { // 내려갈만큼 내려갔을경우 현재위치를 리턴시킴
            _finalStop_idx = distance(c.begin(), vacant);
        }
        else if (comp(*left_child(vacant), *right_child(vacant))) { // 좌측이 더 작을 경
            swap(*vacant, *left_child(vacant));
            _finalStop_idx = _promote(_nextStop, left_child(vacant), cur_h - 1);
        }
        else{ //우측 자식
            swap(*vacant, *right_child(vacant));
            _finalStop_idx = _promote(_nextStop, right_child(vacant), cur_h - 1);
        }
        return _finalStop_idx;
	}

	void _fixHeapFast(_Iter vacant, int _h) {
        if (_h <= 1) {
            _Iter smaller;
            if (left_child_idx(vacant) >= c.size()) return; /* leaf node */
            else if (right_child_idx(vacant) >= c.size()) smaller = left_child(vacant); /* one child */
            else smaller = (comp(*left_child(vacant), *right_child(vacant)) ? left_child(vacant) : right_child(vacant)); /* two children */

            if (!comp(*vacant, *smaller)) swap(*vacant, *smaller);

            return;
        }

        int nextStopHeight = _h / 2;
        int vacStop_idx = _promote(nextStopHeight, vacant, _h); // 새로운 위치
        _Iter vacStop = c.begin() + vacStop_idx;
        _Iter vacParent = c.begin() + ((vacStop_idx - 1) >> 1); // 새로 찾은 위치의 부모ㄴ
        if (comp(*vacStop, *vacParent)) {
            swap(*vacStop, *vacParent);
            _bubbleUpHeap(*vacParent, ((vacStop_idx - 1) >> 1));
        }
        else
            _fixHeapFast(vacStop, nextStopHeight);
    }
	void _bubbleUpHeap(value_type _Value, int vacant) {
        /*
         * repeat bubble up until there is not min-heap violation
         */
        if (c.empty()) return;

        int _hole_idx = vacant;
        for(
                int _hole_parent_idx = (_hole_idx - 1) >> 1;
                _hole_idx!=0 && comp(_Value, *(c.begin() + _hole_parent_idx));
                _hole_parent_idx = (_hole_idx - 1) >> 1
                ){
            *(c.begin() + _hole_idx) = *(c.begin() + _hole_parent_idx);/* up hole and down value */
            _hole_idx  = _hole_parent_idx;
        }

        *(c.begin() + _hole_idx) = _Value;
    }

public:
	explicit Heap(const _Compare& _Pred) : c(), comp(_Pred) {}
    Heap() = default;;

	const_reference top() const {
        return c.front();
    }
	void push(const _Ty& e) {
        c.push_back(e);
        _bubbleUpHeap(e, c.size()-1);
    }
	void pop() {
        *(c.begin()) = c.back();
        c.pop_back();
        _fixHeapFast(c.begin(), static_cast<int>(log(c.size()) + 1));
    }
protected:
	vector<_Ty> c{};
    _Compare comp;
};

struct comparePair {
    bool operator()(const pii& e1, const pii& e2) {
        return (e1.second == e2.second ? e1.first <= e2.first : e1.second < e2.second);
    };
};

void prim(int start, const vvp& edge_list, vb& visited, int n) {
    vi tree;
    Heap<pii, comparePair> fringe;
    int total_dist = 0;

    visited[start] = true;
    tree.push_back(start);
    int curV = start;

    while (--n) {
        //  add adjacent vertex to fringe set
        for (auto& adj : edge_list[curV])
            if (!visited[adj.first]) fringe.push(adj);

        // find minimal cost vertex in fringe set
        int target, cur_cost;
        do {
            target = fringe.top().first;
            cur_cost = fringe.top().second;
            fringe.pop();
        } while (visited[target]);

        // set visited & move into tree
        visited[target] = true;
        tree.push_back(target);
        total_dist += cur_cost;
        curV = target;
    }
    printf("%d ", total_dist);
    for (auto& cur : tree) printf("%d ", cur);
    printf("\n");
}

int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);

    vvp data(n + 1); // adjacent list
    vb visited(n + 1, false);

    int s, e, c;
    while (m--) {
        scanf("%d%d%d", &s, &e, &c);
        data[s].push_back({ e, c });
        data[e].push_back({ s, c });
    }

    while (q--) {
        int startV = 0;
        scanf("\nP %d", &startV);
        prim(startV, data, visited, n);
        fill(visited.begin(), visited.end(), false);  // reset dist vector for next time
    }

    return 0;
}
