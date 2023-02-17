#pragma once

#include "ImGuiRnedererBase.h"

namespace Muscle
{
	class SkyBox;
}

class DebugBox : public Muscle::ImGuiRnedererBase
{
public:
	DebugBox(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~DebugBox() override = default;

private:
	std::shared_ptr<Muscle::ParticleRenderer> _selectedParticle;
	std::shared_ptr<Muscle::SkyBox> _skyBox;

protected:
	/**
	* \brief �� �Լ��� �׷��Ƚ��� �Ѱ��� �Լ� ��ü�� ��������.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;

	virtual void Update() override;

	// SkyBox�� �����Ѵ�.
	void SetSkyBox(std::shared_ptr<Muscle::SkyBox>& skyBox);


};

