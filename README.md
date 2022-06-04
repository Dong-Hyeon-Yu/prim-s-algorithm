# prim-s-algorithm 과제
: 후보 노드 중 최소값을 뽑아야하는데, 최소값을 뽑아주는 구현체를 아래와 같이 다르게 구현.

1. `unorderedArray` 브랜치 
- 배열에 단순히 push_back.
- 탐색에 O(n) 

2. `heap` 브랜치
- 평범한 heap 구현.
- 힙 클래스를 한번 stl generic 처럼 만들어보려고 했음.
- O(logN)


3. `fastHeap` 브랜치
- 힙 클래스를 한번 stl generic 처럼 만들어보려고 했음.
- 구현 실패. 테스트 케이스 통과 못함. 왜 그런지 모름.
- O(loglogN)
