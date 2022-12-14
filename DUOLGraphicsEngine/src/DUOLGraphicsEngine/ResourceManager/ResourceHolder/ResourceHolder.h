#pragma once
#include <functional>
#include <map>
#include <DUOLCommon/StringHelper.h>



class ResourceHolder
{

	struct ResourceStructure
	{
		//unsigned int _structureSize;

		//List, Map, Vector중 리소스 자료구조에 어떤걸 사용해야하는가?
		//런타임중 검색 효율. List와 Vector는 O(n), map은 O(log(n))
		//List		-> 삽입과 삭제가 용이, 데이터가 비선형적임.
		//vector	-> 삽입과 삭제가 비효율적일 수 있음, 데이터가 선형적, 검색이 느림
		//map		-> 검색 빠름, 데이터 선형적 but 리소스의 삽입과 삭제에 비효율적임(vector보다 더) 하지만 이는 "로딩"시간에 발생함.
		//고로 맵을 사용한다.

		//std::map<>;
		//std::function<void(void*)> _deleter;
	};


private:
	std::map<DUOLCommon::tstring, ResourceStructure> _resources;

};

