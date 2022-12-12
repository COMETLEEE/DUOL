#pragma once

#include "ImGuiRnedererBase.h"

class DockSpace : public Muscle::ImGuiRnedererBase
{
public:
	DockSpace(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~DockSpace() override = default;

private:
	void ShowFileMenu(); // 독스페이스의 상단 파일 메뉴를 보여주는 함수.

	void ShowDockSpace();

protected:
	/**
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;
};

