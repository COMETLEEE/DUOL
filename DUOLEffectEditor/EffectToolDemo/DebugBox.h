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
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;

	virtual void Update() override;

	// SkyBox를 설정한다.
	void SetSkyBox(std::shared_ptr<Muscle::SkyBox>& skyBox);


};

