#pragma once
#include <memory>

#include "DUOLGameEngine/Export_Engine.h"
#include "DUOLGameEngine/Util/Constants.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLCommon/Util/UUID.h"

#include <rttr/registration_friend>
#include <rttr/rttr_enable.h>

namespace DUOLGameEngine
{
	/**
	 * \brief 내부에서 사용하는 오브젝트 타입 식별용 enum class.
	 */
	enum class ObjectType
	{
		GameObject
		, Component
		, Resource
		, UI
	};

	/**
	 * \brief DUOLGameEngine에서 참조되는 모든 클래스의 기본 클래스입니다.
	 */
	class DUOL_GAMEENGINE_API ObjectBase
	{
	protected:
		ObjectType _objectType;

		ObjectBase(const DUOLCommon::tstring& name, ObjectType objectType);

	public:
		virtual ~ObjectBase();

		DEFINE_DEFAULT_COPY_MOVE(ObjectBase)

		operator bool();

		bool operator==(ObjectBase* other) const;

		bool operator!=(ObjectBase* other) const;

	public:
		/**
		 * \brief 오브젝트를 파괴합니다. (GameObject, Component, Asset, ...)
		 * \param obj 파괴할 오브젝트입니다.
		 * \param t 해당 시간 이후 파괴합니다.
		 */
		static void Destroy(ObjectBase* obj, float t = 0.0f);

		/**
		 * \brief 씬이 로드되더라도 파괴하지 않도록 설정합니다.
		 * \param target 적용할 오브젝트입니다.
		 */
		static void DontDestroyOnLoad(ObjectBase* obj);

		/**
		 * \brief 씬이 로드되면 파괴되도록 설정합니다.
		 * \param target 적용할 오브젝트입니다.
		 */
		static void DestroyOnLoad(ObjectBase* obj);

	protected:
		/**
		 * \brief 해당 오브젝트의 UUID입니다.
		 */
		DUOLCommon::UUID _uuid;

		/**
		 * \brief 해당 오브젝트의 이름입니다.
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief 해당 오브젝트의 씬 로드 시 파괴 여부를 나타냅니다.
		 */
		bool _isDontDestroyOnLoad;

	public:
		const DUOLCommon::tstring& GetName() const;

		void SetName(const DUOLCommon::tstring& name);

		const DUOLCommon::UUID& GetUUID() const;

		void* GetThis();

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE()
	};
}
