#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/EntityID.h"
#include "DUOLGraphicsLibrary/Core/Typedef.h"

namespace DUOLGraphicsLibrary
{
	/**
		@class   EntityBase
		@brief   그래픽스 내부 모든 오브젝트들의 인터페이스
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
	  @brief  런타임중의 RTTI를 효과적으로 사용하기 위해 만든다. rtti보다 빠르다!
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
