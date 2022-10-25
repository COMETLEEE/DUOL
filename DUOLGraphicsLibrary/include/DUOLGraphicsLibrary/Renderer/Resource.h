#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"

namespace DUOLGraphicsLibrary
{

 /**

     @class   Resource
     @brief   파이프라인에 바인딩되는 리소스들의 부모클래스입니다. resource type을 통해 리소스들을 구별합니다
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

