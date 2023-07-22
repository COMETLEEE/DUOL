#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLClient/ECS/Component/Enemy/EnemySpawnTrigger.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Scrollbar;
	class MeshRenderer;
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

	constexpr float DOMINATION_BORDER_EMISSIVE_POWER = 7.f;

class DUOL_CLIENT_API DominationArea :public DUOLGameEngine::MonoBehaviourBase
{
public:
	DominationArea(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("DominationArea"));

	virtual ~DominationArea() override;

	bool IsCleared() const;

	void SetTotallyClear(bool value);

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

	void OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

private:
	/**
	 * \brief ���� �������� �ı��� ������ġ
	 */
	float _destroyIncreasePerMonster;

	//  �����Ӵ� �ƽ��ı���
	float _destroyIncreasePerFrame;

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

	/**
	 * \brief ����Ʈ �ı��� ������
	 */
	float _defaultDestroyIncrease;

	/**
	* \brief ����Ʈ �ı��� ������
	*/
	float _monsterSpawnBelowCurrentClear;



	/**
	* \brief Ŭ��������
	*/
	float _clearPercent;

	DUOLGameEngine::GameObject* _uiGameObject;

	DUOLGameEngine::Scrollbar* _dominationGauge;

	DUOLGameEngine::BoxCollider* _areaCollider;

	int _currentEnemyCountInZone;

	bool _playerInZone;

	/**
	 * \brief ���ɿ���
	 */
	bool _isCleared;

	bool _isTotallyCleared;


	DUOLGameEngine::MeshRenderer* _dominationBorder;

	DUOLMath::Vector3 _destroyColor;

	DUOLMath::Vector3 _clearColor;

	EnemyGroupController* _enemyGroupControl;


	RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	RTTR_REGISTRATION_FRIEND
};

}
