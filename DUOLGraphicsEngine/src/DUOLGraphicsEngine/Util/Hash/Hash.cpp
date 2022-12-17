#include "Hash.h"
#include "SpookyV2.h"

std::unordered_map<DUOLCommon::tstring, uint64, DUOLGraphicsEngine::Hash> DUOLGraphicsEngine::Hash::_hashTable;

uint64 DUOLGraphicsEngine::Hash::Hash64(const DUOLCommon::tstring& message)
{
	//���� message���̶� seed�� ���� �ؽð��� �޶�����.
	//���⼭�� ������ seed ���� �����Ѵ�.
	//auto result = _hashTable.find(message);

	//if (result == _hashTable.end())
	//{
	//	_hashTable.emplace(message, _hashTable.size());
	//}

	//auto ret1 = result->first;
	//auto ret2 = result->second;

	if (_hashTable.end() == _hashTable.find(message))
	{
		_hashTable.emplace(message, _hashTable.size());
	}

	return _hashTable.find(message)->second;
}

uint64 DUOLGraphicsEngine::Hash::HashFunction(const DUOLCommon::tstring& message)
{
	//���� message���̶� seed�� ���� �ؽð��� �޶�����.
	//���⼭�� ������ seed ���� �����Ѵ�.
	constexpr uint64 seed = 0x1234567887654321;

	return SpookyHash::Hash64(message.data(), message.length(), seed);
}
