#include "DUOLClient/ECS/Component/Contents/DominationArea.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/GameObject.h"

DominationArea::DominationArea(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name)
{
}

DominationArea::~DominationArea()
{
}

void DominationArea::OnEnable()
{
}

void DominationArea::OnDisable()
{
}

void DominationArea::OnAwake()
{
	_areaCollider = GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>();
}

void DominationArea::OnStart()
{
	//GetTGetComponent<DUOLGameEngine::BoxCollider>();
}

void DominationArea::OnDestroy()
{
}

void DominationArea::OnUpdate(float deltaTime)
{
}
