#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/EntityID.h"

namespace DUOLGraphicsLibrary
{
	/**
	 *�׷��Ƚ� ���� ��� ������Ʈ���� �������̽�
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

		// ��Ÿ������ RTTI�� ȿ�������� ����ϱ� ���� �����. rtti���� ������!
		virtual bool IsInstanceOf(int id) const;
	};
}
