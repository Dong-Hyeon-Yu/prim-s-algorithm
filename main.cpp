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
         * 
         * - violation 이 발생하지 않을때까지 최대 O(logN)번 반복.
         * - for 문 안에서 vacant를 위로 올리고 다음 인덱스를 갱신하는 작업이 O(1)
         *     ==> 따라서 총 O(logN) 시간복잡도를 갖는다. 
         */
        if (c.empty()) return;

        int _hole_idx = vacant;
        for(
                int _hole_parent_idx = (_hole_idx - 1) >> 1;
                _hole_idx!=0 && comp(_Value, *(c.begin() + _hole_parent_idx));  /* Until No violation */
                _hole_parent_idx = (_hole_idx - 1) >> 1
                ){
            *(c.begin() + _hole_idx) = *(c.begin() + _hole_parent_idx); /* up hole and down value */
            _hole_idx  = _hole_parent_idx;
        }

        *(c.begin() + _hole_idx) = _Value;
    }
    void _downHeap(_Iter root) {
        /*
        * repeat downHeap until there is no violation
        * 
        * - 두 자식 중 작은 자식과, 자신을 비교하는데 O(1)
        * - 자식과 자신의 관계가 heap 조건을 만족하지 않으면 swap => 최대 O(logN) 반복
        *     ==> 따라서 총 O(logN)의 시간복잡도를 갖는다.
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
        /*
        *  push_back 연산      = O(1)
        *  _bubbleUpHeap 연산  = O(logN)
        * 
        *  총 O(logN) 시간복잡도
        */
        c.push_back(e);
        _bubbleUpHeap(e, c.size()-1);
    }
	void pop() {
        /*
        * 맨 뒤의 요소를 루트에 위치시킴 = O(1)
        * _downHeap 연산                 = O(logN)
        * 
        * 총 O(logN) 시간복잡도
        */
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
        // 현재 정점에 연결된 모든 정점 탐색하므로 deg(v)
		for (auto& adj : edge_list[curV]) 
			if (!visited[adj.first]) fringe.push(adj); // log(N)

        // find minimal cost vertex in fringe set 
        // top => O(1), pop => O(logN)
        // 이미 방문했던 정점을 잇는 간선이 fringe set 에 최대 O(M) 만큼 있을 수 있지만, 총 간선의 개수가 M 이므로 prim 알고리즘을 다 수행했을 때 밑의 while문 실행 회수는 M번을 넘을 수 없다.
        // 따라서 O(AverageDeg(v)logN) => O(logN) 으로 근사
        int target, cur_cost;
        do {
            target = fringe.top().first;
            cur_cost = fringe.top().second;
            fringe.pop();
        } while(visited[target]);

		// set visited & move into tree => O(1)
		visited[target] = true;
		tree.push_back(target);
		total_dist += cur_cost;
		curV = target;
	}
	printf("%d ", total_dist);
	for (auto& cur : tree) printf("%d ", cur);  // => O(N)
    printf("\n");

    /*
    * 따라서 N-1 번 while문을 반복할 때의 시간복잡도는 O(MlogN + NlogN + N) == O(MlogN + NlogN)
    * 최종 프림알고리즘의 시간복잡도는 O(MlogN + NlogN) 이다. 
    * sparse 한 형태의 그래프에서는 O(NlogN), dense 한 형태의 그래프에서는 O(MlogN) 근사할 수 있다.
    */
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
