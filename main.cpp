#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-reserved-identifier"
#define _CRT_SECURE_NO_WARNINGS
#include <queue>

#include <cstdio>
#include <climits>
#include <vector>
#include <cmath>
#include <algorithm>

#define pii pair<int, int>
#define vvp vector<vector<pii>> 
#define vb vector<bool>
#define vii vector<pii>
#define INF INT_MAX
using namespace std;

template <class _Ty, class _Compare = less<typename vector<_Ty>::value_type>>
class Heap {
    using _Container        = vector<_Ty>;
    using const_reference   = typename _Container::const_reference;
    using _Iter             = typename _Container::iterator;
    using size_type         = typename _Container::size_type;
    using value_type        = typename _Container::value_type;
private:
    int left_child_idx(int a) { return (a << 1) + 1; }
    int right_child_idx(int a) { return (a << 1) + 2; }
    _Iter left_child(int a) {return c.begin() + left_child_idx(a);}
    _Iter left_child(_Iter a) {return left_child(distance(c.begin(), a));}
    _Iter right_child(int a) {return c.begin() + right_child_idx(a);}
    _Iter right_child(_Iter a) {return right_child(distance(c.begin(), a));}


	int _promote(int _hStop, int vacant, int h) {
		/*
		go down towart bigger child each step from _hStop to h.
        */
        int _finalStop_idx; // 내려갈 위치
        if (h <= _hStop) { // 내려갈만큼 내려갔을경우 현재위치를 리턴시킴
            _finalStop_idx = vacant;
        }
        else if (comp(*right_child(vacant),*left_child(vacant)) && right_child_idx(vacant) < c.size()) { // 우측이 더 작을 경
            iter_swap(c.begin() + vacant, right_child(vacant));
            _finalStop_idx = _promote(_hStop, right_child_idx(vacant), h - 1);
        }
        else{ //좌측 자식
            iter_swap(c.begin() + vacant, left_child(vacant));
            _finalStop_idx = _promote(_hStop, left_child_idx(vacant), h - 1);
        }
        return _finalStop_idx;
	}

	void _fixHeapFast(value_type K, int vacant,  int _h) {
        if (_h == 0) return;
        if (_h == 1) { // h가 1이하일경우
            if (left_child_idx(vacant) >= c.size())
                return; // h가 1이남았지만 해당노드의 자식이 없을수도있다.
            else if (comp(*right_child(vacant), K) && right_child_idx(vacant) < c.size()){
                _Iter smaller = (comp(*left_child(vacant), *right_child(vacant)) ? left_child(vacant) : right_child(vacant));
                iter_swap(c.begin() + vacant, smaller);
            }
            else if (comp(*left_child(vacant), K) && left_child_idx(vacant) < c.size()) {
                iter_swap(c.begin() + vacant, left_child(vacant));
            }
            return;
        }

        int hStop = _h / 2;
        int vacStop = _promote(hStop, vacant, _h); // 새로운 위치
        int vacParent = (vacStop-1) >> 1; // 새로 찾은 위치의 부모
        if (comp(K, *(c.begin() + vacParent))) {
            iter_swap(c.begin() + vacStop, c.begin() + vacParent);
            _bubbleUpHeap(vacant, K, vacParent);
        }
        else
            _fixHeapFast(K, vacStop,  hStop);
    }
    void _bubbleUpHeap(int root, value_type K, int vacant){
        if (vacant == root)
            *(c.begin() + vacant) = K;
        else {
            int parent = (vacant-1)>>1;
            if (comp(*(c.begin() + parent), K)) { // 부모 보다 크다면 자기위치를 찾은것
                *(c.begin() + vacant) = K;
            }
            else { // 부모 보다 작다면 버블업힙을 계속 실행
                iter_swap(c.begin() + vacant, c.begin() + parent);
                _bubbleUpHeap(root, K, parent);
            }
        }
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
    size_type size() const { return c.size(); }
    bool empty() const { return c.empty(); }
	const_reference top() const {
        /* require none-empty! */
        return c.front();
    }
	void push(const _Ty& e) {
        c.push_back(e);
        _bubbleUpHeap(e, c.size()-1);
    }
	void pop() {
        if (c.empty()) return;
        *(c.begin()) = c.back();
        c.pop_back();
        _fixHeapFast(c.front(), 0, static_cast<int>(log(c.size())+1));
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
struct comparePair2 {
    bool operator()(const pii& e1, const pii& e2) {
        return (e1.second == e2.second ? e1.first > e2.first : e1.second > e2.second);
    };
};

void prim(int start, const vvp& edge_list, vb& visited, int n) {
	vii tree;
	Heap<pii, comparePair> fringe;
    //priority_queue<pii, vii, comparePair2> fringe;
	int total_dist = 0;

	visited[start] = true;
	tree.push_back({start, 0});
	int curV = start;

	while (--n) { // n-1 번 반
		//  인접 정점을 fringe Set 에 추
		for (auto& adj : edge_list[curV]) 
			if (!visited[adj.first]) fringe.push(adj);

        // min of fringe Set
        int target, cur_cost;
        do {
            target = fringe.top().first;
            cur_cost = fringe.top().second;
            fringe.pop();
        } while(visited[target]);

		//
		visited[target] = true;
		tree.push_back({target, cur_cost});
		total_dist += cur_cost;
		curV = target;
	}
	printf("%d ", total_dist);
	for (auto& cur : tree) printf("{%d, %d} ", cur.first, cur.second);
	printf("remains(%ld)\n", fringe.size());
}

int main() {
	int n, m, q;
	if (scanf("%d%d%d", &n, &m, &q) != 3) {
		printf("error scanf!\n");
		return 1;
	};

	vvp data(n + 1); // edge list
	vb visited(n + 1, false);

	int s, e, c;
	while (m--) {
		if (scanf("%d%d%d", &s, &e, &c) != 3) {
			printf("error scanf during get data!\n");
			return 1;
		};
		data[s].push_back({ e, c });
		data[e].push_back({ s, c });
	}

	while (q--) {
		int startV = 0;
		if (scanf("\nP %d", &startV) != 1) {
			printf("error scanf during get start vertex!\n");
			return 1;
		};
		prim(startV, data, visited, n);
		fill(visited.begin(), visited.end(), false);  // reset dist vector for next time
	}

	return 0;
}

/* for local debugging */
int _main() {
    int n;
    Heap<int> pq;
    while(scanf("%d", &n)){
        if(n>0) pq.push(n);
        else{
            printf("top: %d\n", pq.top());
            pq.pop();
        }

    }
}

#pragma clang diagnostic pop