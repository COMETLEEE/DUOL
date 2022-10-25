#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"

namespace DUOLGraphicsLibrary
{

 /**

     @class   Resource
     @brief   ���������ο� ���ε��Ǵ� ���ҽ����� �θ�Ŭ�����Դϴ�. resource type�� ���� ���ҽ����� �����մϴ�
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Resource : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Resource);

	public:

		virtual ResourceType GetResourceType() abstract;

	protected:
		Resource() = default;

	};

}

