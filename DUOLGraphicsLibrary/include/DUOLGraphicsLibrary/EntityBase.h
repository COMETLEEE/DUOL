#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/EntityID.h"

namespace DUOLGraphicsLibrary
{
	/**
	 *그래픽스 내부 모든 오브젝트들의 인터페이스
	 **/

#define DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(ID)		\
	public:											\
		inline static int GetEntityID()				\
		{											\
			return static_cast<int>(ID);			\
		}											\
		bool IsInstanceOf(int id) const override	

#define DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(SELF, BASE)				\
	bool SELF::IsInstanceOf(int id) const								\
	{																	\
		return (id == SELF::GetEntityID() || BASE::IsInstanceOf(id));	\
	}

	class DUOLGRAPHICSLIBRARY_EXPORT EntityBase
	{
	public:

		// 런타임중의 RTTI를 효과적으로 사용하기 위해 만든다. rtti보다 빠르다!
		virtual bool IsInstanceOf(int id) const;
	};
}
