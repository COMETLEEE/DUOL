#pragma once
#include <functional>
#include <map>
#include <DUOLCommon/StringHelper.h>



class ResourceHolder
{

	struct ResourceStructure
	{
		//unsigned int _structureSize;

		//List, Map, Vector�� ���ҽ� �ڷᱸ���� ��� ����ؾ��ϴ°�?
		//��Ÿ���� �˻� ȿ��. List�� Vector�� O(n), map�� O(log(n))
		//List		-> ���԰� ������ ����, �����Ͱ� ��������.
		//vector	-> ���԰� ������ ��ȿ������ �� ����, �����Ͱ� ������, �˻��� ����
		//map		-> �˻� ����, ������ ������ but ���ҽ��� ���԰� ������ ��ȿ������(vector���� ��) ������ �̴� "�ε�"�ð��� �߻���.
		//��� ���� ����Ѵ�.

		//std::map<>;
		//std::function<void(void*)> _deleter;
	};


private:
	std::map<DUOLCommon::tstring, ResourceStructure> _resources;

};

