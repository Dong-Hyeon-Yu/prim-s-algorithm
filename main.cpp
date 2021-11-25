#define _CRT_SECURE_NO_WARNINGS

#include <functional>  // for template struct less<_Ty>
#include <cstdio>
#include <vector>

#define pii pair<int, int>
#define vvp vector<vector<pii>> 
#define vb vector<bool>
#define vi vector<int>
#define vii vector<pii>
using namespace std;

// generic class Heap
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
    _Iter left_child(_Iter a) { return c.begin() + ((distance(c.begin(), a) << 1) + 1); }
    _Iter right_child(_Iter a) { return c.begin() + ((distance(c.begin(), a) << 1) + 2); }

	void _bubbleUpHeap(value_type _Value, int vacant) {
        /*
         * repeat bubble up until there is not min-heap violation
         */
        if (c.empty()) return;

        int _hole_idx = vacant;
        for(
                int _hole_parent_idx = (_hole_idx - 1) >> 1;
                _hole_idx!=0 && comp(_Value, *(c.begin() + _hole_parent_idx));  /* Until Not violation */
                _hole_parent_idx = (_hole_idx - 1) >> 1
                ){
            *(c.begin() + _hole_idx) = *(c.begin() + _hole_parent_idx); /* up hole and down value */
            _hole_idx  = _hole_parent_idx;
        }

        *(c.begin() + _hole_idx) = _Value;
    }
    void _downHeap(_Iter root) {
        /*
        * repeat downHeap until there is not min-heap violation
        */
        _Iter smaller;
        if (left_child_idx(root) >= c.size()) return; /* leaf node! */
        else if (right_child_idx(root) >= c.size()) smaller = left_child(root); /* only one child */
        else smaller = comp(*left_child(root), *right_child(root)) ? left_child(root) : right_child(root); /* have two children */

        /* heap violation !! */
        if (!comp(*root, *smaller)) { 
            swap(*root, *smaller);
            _downHeap(smaller);
        }
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
        _downHeap(c.begin());
    }
protected:
	vector<_Ty> c{};
    _Compare comp;
};

struct comparePair {
	bool operator()(const pii& e1, const pii& e2) {
		return (e1.second == e2.second ? e1.first < e2.first : e1.second < e2.second);
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
        } while(visited[target]);

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
