#pragma once
#include "DUOLCommon/StringHelper.h"
#include <unordered_map>
typedef  unsigned __int64 uint64;
typedef  unsigned __int32 uint32;
typedef  unsigned __int16 uint16;
typedef  unsigned __int8  uint8;

namespace DUOLGraphicsEngine
{
	//�ؽ��Լ��δ� spookyHash V2�� ����Ѵ�
 /**

     @class   Hash
     @brief   64-bit Hash function. using SpookyHash V2
     @details ~
     @author  KyungMin Oh

 **/
	class Hash
	{
	public:
		static uint64 Hash64(const DUOLCommon::tstring& message);

		static std::unordered_map<DUOLCommon::tstring, uint64, Hash> _hashTable;

		std::size_t operator()(const DUOLCommon::tstring& message) const
		{
			return HashFunction(message);
		}
	private:
		static uint64 HashFunction(const DUOLCommon::tstring& message);
	};


	

}
