#pragma once
#include <memory>

#include "DUOLGameEngine/Util/Constants.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/UUID.h"

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
	};

	/**
	 * \brief DUOLGameEngine���� �����Ǵ� ��� Ŭ������ �⺻ Ŭ�����Դϴ�.
	 */
	class ObjectBase
	{
	protected:
		ObjectType _objectType;

	public:
		ObjectBase(const DUOLCommon::tstring& name, ObjectType objectType);

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
		static void DontDestroyOnLoad(ObjectBase* target);

		/**
		 * \brief ���� �ε�Ǹ� �ı��ǵ��� �����մϴ�.
		 * \param target ������ ������Ʈ�Դϴ�.
		 */
		static void DestroyOnLoad(ObjectBase* target);

	private:
		/**
		 * \brief �ش� ������Ʈ�� �� �ε� �� �ı� ���θ� ��Ÿ���ϴ�.
		 */
		bool _isDontDestroyOnLoad;

		/**
		 * \brief �ش� ������Ʈ�� �̸��Դϴ�.
		 */
		DUOLCommon::tstring _name;

		/**
		 * \brief �ش� ������Ʈ�� UUID�Դϴ�.
		 */
		UUID _uuid;

	public:
		const DUOLCommon::tstring& GetName() const;

		void SetName(const DUOLCommon::tstring& name);

		const UUID& GetUUID() const;
	};
}