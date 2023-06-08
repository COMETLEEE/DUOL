#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class BoxCollider;
}

/**
 * \brief �ΰ��� ���� A ���� ������ �������� ���� Ŭ�����Դϴ�.
 *
 *	��� -
 *		
 *
 */
class DUOL_CLIENT_API DominationArea :public DUOLGameEngine::MonoBehaviourBase
{
public:
	DominationArea(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("InGameUI"));

	virtual ~DominationArea() override;

/**
 * \brief ����� ���� ũ��
 */
	DUOLMath::Vector3 _areaExtents;

	/**
	 * \brief ���� �� ī��Ʈ
	 */
	float _generateCount;

	/**
	 * \brief ������ �ı���(��ȭ��)
	 */
	float _destroyPercent;

	DUOLGameEngine::BoxCollider* _areaCollider;

public:
	/**
 * \brief Behaviour�� ��� ���������� �� ȣ��˴ϴ�.
 */
	virtual void OnEnable() override;

	/**
	 * \brief Behaviour�� ��� �Ұ��������� �� ȣ��˴ϴ�.
	 */
	virtual void OnDisable() override;

	/**
 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
 */
	virtual void OnAwake() override;

	/**
	 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
	 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
	 */
	virtual void OnStart() override;

	/**
	 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
	 */
	virtual void OnDestroy() override;

	/**
	 * \brief �� �����ӿ� ȣ��˴ϴ�.
	 * \param deltaTime ������ �� �ð� �����Դϴ�.
	 */
	virtual void OnUpdate(float deltaTime) override;


	RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	RTTR_REGISTRATION_FRIEND
};

