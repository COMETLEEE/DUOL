#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Material;
}

namespace DUOLClient
{
class DUOL_CLIENT_API CautionBorder : public DUOLGameEngine::BehaviourBase
{
public:
	CautionBorder(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CautionBorder"));

	virtual ~CautionBorder() override;

public:

	/**
 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
 */
	virtual void OnUpdate(float deltaT) override;

	virtual void OnAwake() override;

	virtual void OnStart() override;

	void SetBorderOnlyMesh(bool value);

	std::vector<DUOLGameEngine::GameObject*> _meshObjs;

	DUOLGameEngine::Material* _upperline;

	DUOLGameEngine::Material* _lowerline;

	DUOLGameEngine::Material* _cautionline;

	int _upperLineSpeed = 10;

	int _lowerLineSpeed = 10;

	int _cautionSpeed = 5;

	RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

	RTTR_REGISTRATION_FRIEND
};
}
