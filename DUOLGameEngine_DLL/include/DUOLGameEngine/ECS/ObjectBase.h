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
	 * \brief ���ο��� ����ϴ� ������Ʈ Ÿ�� �ĺ��� enum class.
	 */
	enum class ObjectType
	{
		GameObject
		, Component
		, Resource
		, UI
	};

	/**
	 * \brief DUOLGameEngine���� �����Ǵ� ��� Ŭ������ �⺻ Ŭ�����Դϴ�.
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
		 * \brief ������Ʈ�� �ı��մϴ�. (GameObject, Component, Asset, ...)
		 * \param obj �ı��� ������Ʈ�Դϴ�.
		 * \param t �ش� �ð� ���� �ı��մϴ�.
		 */
		static void Destroy(ObjectBase* obj, float t = 0.0f);

		/**
		 * \brief ���� �ε�Ǵ��� �ı����� �ʵ��� �����մϴ�.
		 * \param target ������ ������Ʈ�Դϴ�.
		 */
		static void DontDestroyOnLoad(ObjectBase* obj);

		/**
		 * \brief ���� �ε�Ǹ� �ı��ǵ��� �����մϴ�.
		 * \param target ������ ������Ʈ�Դϴ�.
		 */
		static void DestroyOnLoad(ObjectBase* obj);

	protected:
		/**
		 * \brief �ش� ������Ʈ�� UUID�Դϴ�.
		 */
		DUOLCommon::UUID _uuid;

		/**
		 * \brief �ش� ������Ʈ�� �̸��Դϴ�.
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief �ش� ������Ʈ�� �� �ε� �� �ı� ���θ� ��Ÿ���ϴ�.
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
