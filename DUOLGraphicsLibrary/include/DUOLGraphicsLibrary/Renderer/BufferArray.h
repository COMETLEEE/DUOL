#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;

	//todo

 /**

     @class   BufferArray
     @brief   �ν��Ͻ̿� ���� ���۹���
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
