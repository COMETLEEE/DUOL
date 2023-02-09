#pragma once
#include <queue>

/**

    @class   ReservablePriorityQueue
    @brief   priority Queue에 없는 reserve 기능을 제공하는 queue. 이게 SAFE한 방법인가 생각해봤을때는 잘 모르겠다.
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

