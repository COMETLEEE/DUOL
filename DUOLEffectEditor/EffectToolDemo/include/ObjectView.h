/**

	@file      ObjectView.h
	@brief     오브젝트 뷰어의 렌더링과 구현.
	@details   ~
	@author    SinSeongHyeon
	@date      7.12.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once


#include "ImGuiRnedererBase.h"
#include <memory>

struct ImColor;

class ObjectView : public Muscle::ImGuiRnedererBase
{
public:
	ObjectView(std::shared_ptr<Muscle::GameObject> _gameObject);

	virtual ~ObjectView() override = default;

private:
	void ItemRowsBackground(float lineHeight, const ImColor& color);

	void DrawTree_AllObject();

	void ShowObject(const std::shared_ptr<Muscle::GameObject>& gameObject);

	std::shared_ptr<Muscle::GameObject> _parents;

	std::shared_ptr<Muscle::GameObject> _child;
protected:
	/**
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;
public:
	virtual void Start() override;
};

