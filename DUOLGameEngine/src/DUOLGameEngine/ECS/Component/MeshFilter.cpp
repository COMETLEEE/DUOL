#include "DUOLGameEngine/ECS/Component/MeshFilter.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"

namespace DUOLGameEngine
{
	MeshFilter::MeshFilter(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		ComponentBase(owner, name)
		, _mesh(nullptr)
	{
		const std::shared_ptr<DUOLGameEngine::MeshRenderer> meshRenderer = 
			GetGameObject()->GetComponent<DUOLGameEngine::MeshRenderer>();

		if (meshRenderer != nullptr)
		{
			// 생성자이기 때문에 쉐어드 포인터를 던지지 못함.
			// => 그냥 Primitive pointer 던져서 참조하는 것으로 합시다 ..
			meshRenderer->_meshFilter = this;
		}
	}

	MeshFilter::~MeshFilter()
	{

	}
}