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
		 * \brief ����� �����͸� �׸��� ���� View-Projection ��Ʈ������ �̿��� �׷��� NDC ��ǥ ������ ã���ϴ�.
		 * \param viewProj 
		 */
		void SetViewProjMatrix(const DUOLMath::Matrix& viewProj);

		void SetWindowPosition(const DUOLMath::Vector2& position);

		/**
		 * \brief Depth Buffer�� ���⸦ ����ϴ°� �� �ϴ°�.
		 * \param state true or false
		 */
		virtual void depthMask(bool state);

		/**
		 * \brief texture2D ����ϴ°� �� �ϴ°�.
		 * \param state true or false
		 */
		virtual void texture(bool state);

		/**
		 * \brief
		 * \param prim � ������Ƽ�� ����ϴ°�
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
