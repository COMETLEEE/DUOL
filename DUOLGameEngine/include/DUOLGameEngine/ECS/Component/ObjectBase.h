#pragma once
#include <memory>

#include "DUOLGameEngine/Util/Constants.h"
#include "DUOLGameEngine/API/ISerializable.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/UUID.h"

namespace DUOLGameEngine
{
	/**
	 * \brief DUOLGameEngine���� �����Ǵ� ��� Ŭ������ �⺻ Ŭ�����Դϴ�.
	 */
	class ObjectBase : public DUOLGameEngine::ISerializable
	{
	public:
		ObjectBase(const tstring& name);

		virtual ~ObjectBase() override;

		DEFINE_DEFAULT_COPY_MOVE(ObjectBase)

		operator bool();

		bool operator==(std::shared_ptr<ObjectBase> other) const;

		bool operator!=(std::shared_ptr<ObjectBase> other) const;

	public:
		/**
		 * \brief ������Ʈ�� �ı��մϴ�. (GameObject, Component, Asset, ...)
		 * \param obj �ı��� ������Ʈ�Դϴ�.
		 * \param t �ش� �ð� ���� �ı��մϴ�.
		 */
		static void Destroy(std::shared_ptr<ObjectBase> obj, float t = 0.0f);

		/**
		 * \brief ���� �ε�Ǵ��� �ı����� �ʵ��� �����մϴ�.
		 * \param target ������ ������Ʈ�Դϴ�.
		 */
		static void DontDestroyOnLoad(std::shared_ptr<ObjectBase> target);

		/**
		 * \brief ���� �ε�Ǹ� �ı��ǵ��� �����մϴ�.
		 * \param target ������ ������Ʈ�Դϴ�.
		 */
		static void DestroyOnLoad(std::shared_ptr<ObjectBase> target);

	private:
		/**
		 * \brief �ش� ������Ʈ�� �� �ε� �� �ı� ���θ� ��Ÿ���ϴ�.
		 */
		bool _isDontDestroyOnLoad;

		/**
		 * \brief �ش� ������Ʈ�� �̸��Դϴ�.
		 */
		tstring _name;

		/**
		 * \brief �ش� ������Ʈ�� UUID�Դϴ�.
		 */
		UUID _uuid;

	public:
		inline const tstring& GetName() const { return _name; }

		inline void SetName(const tstring& name) { _name = name; }
	};
}
