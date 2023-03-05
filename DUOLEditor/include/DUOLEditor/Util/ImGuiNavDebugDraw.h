#pragma once
#include <vector>

#include "DUOLGameEngine/Navigation/DebugUtils/DebugDraw.h"

#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLEditor
{
	enum class NavDebugDrawState
	{
		POINTS
		, LINES
		, TRIANGLES
		, QUADS
	};

	/**
	* \brief ImGui Nav-Debug draw implementation.
	*/
	class ImGuiNavDebugDraw : public duDebugDraw
	{
	private:
		std::vector<ImVec2> _vertex = {};

		NavDebugDrawState _currentNavDebugDrawState = NavDebugDrawState::LINES;

		DUOLMath::Matrix _viewProjMatrix = DUOLMath::Matrix::Identity;

		float _size = 1.f;

	public:
		/**
		 * \brief 디버그 데이터를 그리기 전에 View-Projection 매트릭스를 이용해 그려질 NDC 좌표 공간을 찾습니다.
		 * \param viewProj 
		 */
		void SetViewProjMatrix(const DUOLMath::Matrix& viewProj);

		void SetWindowPosition(const DUOLMath::Vector2& position);

		/**
		 * \brief Depth Buffer에 쓰기를 사용하는가 안 하는가.
		 * \param state true or false
		 */
		virtual void depthMask(bool state);

		/**
		 * \brief texture2D 사용하는가 안 하는가.
		 * \param state true or false
		 */
		virtual void texture(bool state);

		/**
		 * \brief
		 * \param prim 어떤 프리미티브 사용하는가
		 * \param size
		 */
		virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);

		/**
		 * \brief Vertex
		 * \param pos
		 * \param color
		 */
		virtual void vertex(const float* pos, unsigned int color);

		/**
		 * \brief Vertex
		 * \param x
		 * \param y
		 * \param z
		 * \param color
		 */
		virtual void vertex(const float x, const float y, const float z, unsigned int color);

		/**
		 * \brief Vertex
		 * \param pos
		 * \param color
		 * \param uv
		 */
		virtual void vertex(const float* pos, unsigned int color, const float* uv);

		/**
		 * \brief Vertex
		 * \param x
		 * \param y
		 * \param z
		 * \param color
		 * \param u
		 * \param v
		 */
		virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);

		/**
		 * \brief End-Up
		 */
		virtual void end();
	};
}
