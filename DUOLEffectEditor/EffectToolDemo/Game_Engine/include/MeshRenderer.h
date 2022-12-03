#pragma once

#include "RendererBase.h"
/// <summary>
/// 스태틱 메시와 스키닝 메시는 전달하는 정보가 다르다.
/// </summary>
/// 
/// 
namespace Muscle
{
	class MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer(std::shared_ptr<GameObject> _GameObject);
		~MeshRenderer();

	public:
		virtual void Start();
		virtual void Update(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
		virtual void Render(); // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더
	
	};

}