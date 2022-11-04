#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;

	//todo

 /**

     @class   BufferArray
     @brief   인스턴싱에 사용될 버퍼묶음
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT BufferArray : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::BufferArray);
	protected:
		BufferArray(const UINT64& guid):
			EntityBase(guid)
		{
			
		}

	};

}
