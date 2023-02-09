#pragma once
#include <queue>

/**

    @class   ReservablePriorityQueue
    @brief   priority Queue�� ���� reserve ����� �����ϴ� queue. �̰� SAFE�� ����ΰ� �����غ������� �� �𸣰ڴ�.
    @details ~
    @tparam  T - 
    @author  KyungMin Oh

**/
template <typename T, typename compare = std::greater<typename std::vector<T>::value_type>>
class ReservablePriorityQueue : public std::priority_queue<T, std::vector<T>, compare>
{
public:
	typedef typename std::priority_queue<T>::size_type size_type;

	ReservablePriorityQueue(size_type capacity = 0){ Reserve(capacity); }

public:
	void Reserve(size_type capacity) { this->c.reserve(capacity); }

    void Clear() { this->c.clear(); }

    const std::vector<T>& GetVector() { return this->c; } ;
};

