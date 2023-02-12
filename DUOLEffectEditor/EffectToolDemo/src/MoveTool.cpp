#include "MoveTool.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "KeyBoard.h"
#include "IGameEngine.h"
#include "Camera.h"
#include "Commands.h"
#include "CTime.h"
#include "EffectEditorManager.h"

MoveTool::MoveTool(std::shared_ptr<Muscle::GameObject> _GameObject) : IComponents(_GameObject), _isSelected(false)
{
	_scale = 60.0f;

	auto mesh = AddComponent<Muscle::MeshRenderer>();

	mesh->_renderingData->_objectInfo._meshName = "Box";

	mesh->_renderingData->_shaderInfo._shaderName.push_back("NoLightOrthoGraphics");
}

MoveTool::~MoveTool()
{
}

void MoveTool::SetType(int num)
{
	_type = num;

	switch (_type)
	{
	case 1: //x
		_direction = DUOLMath::Vector3(1.0f, 0.0f, 0.0f);

		GetComponent<Muscle::MeshRenderer>()->_renderingData->_materialInfo._color = DUOLMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f) + _direction;

		break;
	case 2: //y
		_direction = DUOLMath::Vector3(0.0f, 1.0f, 0.0f);

		GetComponent<Muscle::MeshRenderer>()->_renderingData->_materialInfo._color = DUOLMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f) + _direction;
		break;
	case 3: //z
		_direction = DUOLMath::Vector3(0.0f, 0.0f, 1.0f);

		GetComponent<Muscle::MeshRenderer>()->_renderingData->_materialInfo._color = DUOLMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f) + _direction;
		break;
	default:
		break;
	}

	GetGameObject()->GetTransform()->SetPosition(_direction * (_scale * 0.5f));

	GetGameObject()->GetTransform()->SetScale(DUOLMath::Vector3(0.2f, 0.2f, 0.2f) * _scale + _direction * _scale);
}

void MoveTool::Start()
{
}

void MoveTool::Update()
{
	if (_isSelected)
	{
		auto _MouseMoveUpdate = [&]() {
			DUOLMath::Vector2 currentPos = Muscle::KeyBoard::Get()->GetPos();
			if ((Muscle::KeyBoard::Get()->KeyPress(VK_LBUTTON)))
			{
				currentPos = currentPos - _lastPos;

				float length = currentPos.Length();

				float _signed;

				switch (_type)
				{
				case 1: //x
					_signed = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetRight().Dot(_direction);

					length = currentPos.x > 0 ? length : -length;

					length = _signed > 0 ? length : -length;
					break;
				case 2: //y
					_signed = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetUp().Dot(_direction);

					length = currentPos.y > 0 ? -length : length;

					length = _signed > 0 ? length : -length;
					break;
				case 3: //z
					_signed = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetRight().Dot(_direction);

					length = currentPos.x > 0 ? length : -length;

					length = _signed > 0 ? length : -length;
					break;
				default:
					break;
				}
				if (EffectEditorManager::Get().GetSelectedObject())
					EffectEditorManager::Get().GetSelectedObject()->GetTransform()->SetWorldPosition(EffectEditorManager::Get().GetSelectedObject()->GetTransform()->GetWorldPosition() + _direction * length * Muscle::CTime::GetDeltaTime());
			}
			_lastPos = Muscle::KeyBoard::Get()->GetPos();
		};
		_MouseMoveUpdate();
	}

}

void MoveTool::Select()
{
	_isSelected = true;
	_lastPos = Muscle::KeyBoard::Get()->GetPos();
	_startPos = EffectEditorManager::Get().GetSelectedObject()->GetTransform()->GetWorldPosition();
}

void MoveTool::UnSelect()
{
	_isSelected = false;
	EXCUTE(new ObjectTranslateCommand(EffectEditorManager::Get().GetSelectedObject(), EffectEditorManager::Get().GetSelectedObject()->GetTransform()->GetWorldPosition()));
}