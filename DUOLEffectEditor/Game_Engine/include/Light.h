#pragma once
#include "IComponents.h"
#include "export/RenderingData.h"

namespace Muscle
{

	class Light : public IComponents
	{
	public:
		Light(std::shared_ptr<GameObject> _GameObject);
		~Light();

	public:
		virtual void Start() override;

		virtual void Update() override; // ��� Object Udpate���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ������Ʈ ���ֱ�.

		virtual void Render() override; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

	public:
		MuscleGrapics::LightInfo _lightInfo;
	};
}