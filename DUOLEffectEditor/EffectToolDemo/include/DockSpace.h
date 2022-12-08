#pragma once

#include "ImGuiRnedererBase.h"

class DockSpace : public Muscle::ImGuiRnedererBase
{
public:
	DockSpace(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~DockSpace() override = default;

private:
	void ShowDockSpace();

protected:
	/**
	* \brief �� �Լ��� �׷��Ƚ��� �Ѱ��� �Լ� ��ü�� ��������.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;
};

