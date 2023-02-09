#include "DUOLGameEngine/ECS/Component/Text.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLGameEngine
{
	Text::Text(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name):
		BehaviourBase(owner, name)
	{
		
	}

	Text::~Text()
	{
	}

	void Text::OnUpdate(float deltaTime)
	{
		RectTransform* rectTranform = GetGameObject()->GetComponent<RectTransform>();

		_textBox._rect = rectTranform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());

		auto pivot = rectTranform->GetPivot();
		auto rect = rectTranform->GetRect();

		_textBox._rect.right -= rect.z * pivot.x;
		_textBox._rect.left	 -= rect.z * pivot.x;

		_textBox._rect.top    -= rect.w * pivot.y;
		_textBox._rect.bottom -= rect.w * pivot.y;

		_canvas->DrawTexts(&_textBox, _orderInLayer);
	}
}
