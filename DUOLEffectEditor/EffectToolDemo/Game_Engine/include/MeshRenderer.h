#pragma once

#include "RendererBase.h"
/// <summary>
/// ����ƽ �޽ÿ� ��Ű�� �޽ô� �����ϴ� ������ �ٸ���.
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
		virtual void Update(); // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.
		virtual void Render(); // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����
	
	};

}