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
			// �������̱� ������ ����� �����͸� ������ ����.
			// => �׳� Primitive pointer ������ �����ϴ� ������ �սô� ..
			meshRenderer->_meshFilter = this;
		}
	}

	MeshFilter::~MeshFilter()
	{

	}
}