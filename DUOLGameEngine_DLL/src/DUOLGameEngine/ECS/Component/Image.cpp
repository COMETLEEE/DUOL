#include "DUOLGameEngine/ECS/Component/Image.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLGameEngine
{
	Image::Image(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name):
		BehaviourBase(owner, name)
		,_canvas(nullptr)
		,_orderInLayer(0)
		,_sprite()
	{
	}

	Image::~Image()
	{
	}

	void Image::OnUpdate(float deltaTime)
	{
		RectTransform* rectTranform = GetGameObject()->GetComponent<RectTransform>();

		_sprite._rect = rectTranform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());
		_sprite._offset = rectTranform->GetPivot();

		_canvas->DrawSprite(&_sprite, _orderInLayer);
	}
}
