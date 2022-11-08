/**

	@file      Inspector.h
	@brief     일단 테스트 삼아서 만들어보는 클래스.. Imgui 처음 사용해본다..
	@details   ~
	@author    SinSeongHyeon
	@date      8.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

class Inspector : public Muscle::ImGuiRnedererBase
{
public:
	Inspector(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~Inspector() override = default;

private:
	std::shared_ptr<Muscle::ParticleRenderer> _myParticle;

protected:
	/**
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;

public:
	virtual void Start() override;

	void SetMyParticle(std::shared_ptr<Muscle::ParticleRenderer>& myParticle);
};

