#include "DUOLEditor/Util/ImGuiNavDebugDraw.h"


namespace DUOLEditor
{
	void ImGuiNavDebugDraw::SetViewProjMatrix(const DUOLMath::Matrix& viewProj)
	{
		_viewProjMatrix = viewProj;
	}

	void ImGuiNavDebugDraw::depthMask(bool state)
	{
	}

	void ImGuiNavDebugDraw::texture(bool state)
	{

	}

	void ImGuiNavDebugDraw::begin(duDebugDrawPrimitives prim, float size)
	{
		switch (prim)
		{
			case duDebugDrawPrimitives::DU_DRAW_POINTS:
			{
				_currentNavDebugDrawState = NavDebugDrawState::POINTS;
			}

			case duDebugDrawPrimitives::DU_DRAW_LINES:
			{
				_currentNavDebugDrawState = NavDebugDrawState::LINES;
			}

			case duDebugDrawPrimitives::DU_DRAW_TRIS:
			{
				_currentNavDebugDrawState = NavDebugDrawState::TRIANGLES;
			}

			case duDebugDrawPrimitives::DU_DRAW_QUADS:
			{
				_currentNavDebugDrawState = NavDebugDrawState::QUADS;
			}
		}

		_size = size;
	}

	void ImGuiNavDebugDraw::vertex(const float* pos, unsigned color)
	{

	}

	void ImGuiNavDebugDraw::vertex(const float x, const float y, const float z, unsigned color)
	{

	}

	void ImGuiNavDebugDraw::vertex(const float* pos, unsigned color, const float* uv)
	{
		// 그냥 라인 드로우랑 
	}

	void ImGuiNavDebugDraw::vertex(const float x, const float y, const float z, unsigned color, const float u,
		const float v)
	{

	}

	void ImGuiNavDebugDraw::end()
	{
		auto drawList = ImGui::GetWindowDrawList();;

		switch (_currentNavDebugDrawState)
		{
			case NavDebugDrawState::POINTS:
			{
				

				// drawList->AddRectFilled()
			}

			case NavDebugDrawState::LINES:
			{
				// drawList->
			}

			case NavDebugDrawState::TRIANGLES:
			{
				
			}

			case NavDebugDrawState::QUADS:
			{
				
			}
		}

		_vertex.clear();
	}
}