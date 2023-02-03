#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/EntityID.h"
#include "DUOLGraphicsLibrary/Core/Typedef.h"

namespace DUOLGraphicsLibrary
{
	/**
		@class   EntityBase
		@brief   �׷��Ƚ� ���� ��� ������Ʈ���� �������̽�
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSLIBRARY_EXPORT EntityBase
	{
	protected:
		EntityBase(const UINT64& guid) :
			_guid(guid)
		{
		}

		virtual  ~EntityBase() = default;

	private:
		UINT64 _guid;

	public:
		[[nodiscard]] UINT64 GetGUID() const
		{
			return _guid;
		}

		void SetGUID(UINT64 guid)
		{
			_guid = guid;
		}

		/**
	  @brief  ��Ÿ������ RTTI�� ȿ�������� ����ϱ� ���� �����. rtti���� ������!
	  @param  id -
	  @retval    -
		**/

		virtual bool IsInstanceOf(int id) const;
	};

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
}
