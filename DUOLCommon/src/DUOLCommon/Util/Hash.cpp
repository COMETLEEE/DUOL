#include "DUOLCommon/Util/Hash.h"

#include "DUOLCommon/StringHelper.h"
#include "DUOLCommon/Util/SpookyV2.h"


std::unordered_map<DUOLCommon::tstring, uint64, DUOLCommon::Hash> DUOLCommon::Hash::_hashTable;

uint64 DUOLCommon::Hash::Hash64(const DUOLCommon::tstring& message)
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

uint64 DUOLCommon::Hash::HashFunction(const DUOLCommon::tstring& message)
{
	//���� message���̶� seed�� ���� �ؽð��� �޶�����.
	//���⼭�� ������ seed ���� �����Ѵ�.
	constexpr uint64 seed = 0x1234567887654321;

	return SpookyHash::Hash64(message.data(), message.length(), seed);
}
