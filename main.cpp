#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <climits>
#include <vector>

#define pii pair<int, int>
#define vvp vector<vector<pii>> 
#define vb vector<bool>
#define vi vector<int>
#define vii vector<pii>
#define INF INT_MAX
using namespace std;

//template <class _Ty, class _Pr = less<typename vector<_Ty>::value_type>>
//class Heap {
//private:
//	int _promote(int _hStop, int _cur_stop_idx, int h) {
//		/*
//		go down towart bigger child each step from _hStop to h.
//		*/
//
//		int _final_stop_idx;
//		if (h <= _hStop) 
//			_final_stop_idx = _cur_stop_idx;
//
//		if (comp(c[_cur_stop_idx << 1 + 2], c[_cur_stop_idx << 1 + 1])) {
//			c[_cur_stop_idx]	= c[_cur_stop_idx << 1 + 2];
//			_final_stop_idx		= _promote(_hStop, _cur_stop_idx << 1 * 2, h - 1);
//		}
//		else {
//			c[_cur_stop_idx]	= c[_cur_stop_idx << 1 + 1];
//			_final_stop_idx		= _promote(_hStop, _cur_stop_idx << 1 * 1, h - 1);
//		}
//		return _final_stop_idx;
//
//	};
//	void _fixHeapFast(const _Ty& _Value, int _hole_idx, int _h) {
//		if (_h == 0) { // need to process for _h==1 ? 
//			c[_hole_idx] = _Value;
//			return;
//		}
//		else {
//			int _h_half				= _h >> 1;
//			int _next_hole_idx		= _promote(_h_half, _hole_idx, _h);  // go down from h to h/2
//			int _next_hole_parent	= (hole_idx - 1) >> 1;
//
//			if (comp(_Value, c[stop_parent_idx])) {  /* violate min-heap */ 
//				c[_next_hole_idx] = c[_next_hole_parent];
//				_bubbleUpHeap(_Value, _next_hole_parent);
//			}
//			else 
//				_fixHeapFast(_Value, _next_hole_idx, _h_half);
//		}
//		
//	};
//	void _bubbleUpHeap(const _Ty& _Value, int _hole_idx) {
//		if (_hole_idx == 0) // root
//			c[0] = Value;
//		else {
//			int _parent = (_hole_idx - 1) >> 1;
//			if (comp(c[_hole_idx], _Value))
//				c[_hole_idx] = _Value;
//			else {
//				c[_hole_idx] = c[_parent];
//				_bubbleUpHeap(_Value, _parent);
//			}
//		}
//	};
//
//public:
//	explicit Heap(const _Pr& _Pred) : c(), comp(_Pred) {}
//
//	size_t size() const { return c.size(); }
//	_Ty top() const {return c.front() }
//	bool empty() const { return c.empty(); }
//	void push(const _Ty& e) {};
//	void pop() {};
//protected:
//	vector<_Ty> c{};
//	_Pr comp{};
//};
//
//struct comparePair {
//	bool operator()(const pii& lhs, const pii& rhs) {
//		return lhs.first < rhs.first;
//	};
//};

void prim(int start, const vvp& edge_list, vb& visited, int n) {
	vi tree;
	//Heap<pii> fringe(comparePair);
	vii fringe
	int total_dist = 0;

	visited[start] = true;
	tree.push_back(start);
	int curV = start;

	while (--n) { // n-1 번 반복
		// 인접 정점을 fringe Set 에 넣는다.
		for (auto& adj : edge_list[curV]) 
			if (!visited[adj.first]) fringe.push_back(adj);

		int target = INF;
		int cur_cost = INF;
		// fringe Set 중 최소값을 갖는 정점을 꺼낸다.
		for (auto& adj : fringe) {
			if (!visited[adj.first]) {
				if (adj.second < cur_cost) {
					target = adj.first;
					cur_cost = adj.second;
				}
				else if (adj.second == cur_cost)
					target = (target < adj.first ? target : adj.first); // 같은 cost 이면 낮은 번호 우선
			}
		}

		// 해당 정점을 Tree Set에 넣는다.
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