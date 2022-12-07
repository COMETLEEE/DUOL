/**

	@file      ImGuiRnedererBase.h
	@brief     ImGuiRenderBase... Imgui를 사용하는 객체는 이 Base 클래스를 상속받아서 제작하자
	@details   ~
	@author    SinSeongHyeon
	@date      8.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include "IComponents.h"
#include <functional>

namespace Muscle
{
	class ParticleRenderer;

	class ImGuiRnedererBase : public IComponents
	{
	protected:
		ImGuiRnedererBase(std::shared_ptr<GameObject> _GameObject);

		virtual ~ImGuiRnedererBase() = default;

	protected:
		std::function<void()> _renderingFunc;

	protected:
		/**
	 * \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	 */
		virtual void SetRenderingFunc() abstract;

	public:

		/**
		 * \brief 그리팩스 엔진으로 값을 보내는 역할을 한다.
		 */
		virtual void Render() override;
	};
}

