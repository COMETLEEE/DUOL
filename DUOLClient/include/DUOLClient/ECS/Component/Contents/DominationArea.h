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
namespace DUOLClient
{
	
class DUOL_CLIENT_API DominationArea :public DUOLGameEngine::MonoBehaviourBase
{
public:
	DominationArea(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("DominationArea"));

	virtual ~DominationArea() override;

///**
// * \brief ����� ���� ũ��
// */
//	DUOLMath::Vector3 _areaExtents;

	/**
	 * \brief ���� �������� �ı��� ������ġ
	 */
	float _destroyIncreasePerMonster;

	/**
	* \brief ������ �ƽ� ��ȭ��
	*/
	float _maxClearPercent;

	/**
	 * \brief ������ ���� ��ȭ��
	 */
	float _currentClearPercent;

	/**
	 * \brief �÷��̾� ���� �ð�
	 */
	float _dominationTime;

	DUOLGameEngine::BoxCollider* _areaCollider;

	int _currentEnemyCountInZone;

	bool _playerInZone;

	/**
	 * \brief ���ɿ���
	 */
	bool _isCleared;


	bool IsIsCleared() const;

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

	virtual void OnFixedUpdate(float deltaTime) override;


	void OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;


	RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

	RTTR_REGISTRATION_FRIEND
};

}
