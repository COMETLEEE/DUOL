#include "DUOLGameEngine/Navigation/DebugImplement/NavDebugDrawDUOL.h"

#include "DUOLGameEngine/Manager/DebugManager.h"

namespace DUOLGameEngine
{
	NavDebugDrawDUOL::NavDebugDrawDUOL() 
	{
		_debugManager = DUOLGameEngine::DebugManager::GetInstance().get();

		const auto pitch = -90.0f * DUOLMath::PI / 180.0f;

		const auto yaw = 180.0f * DUOLMath::PI / 180.0f;

		DUOLMath::Quaternion q = DUOLMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.f);

		_convertMatrix = DUOLMath::Matrix::Identity;

		_convertMatrix *= DUOLMath::Matrix::CreateFromQuaternion(q);
	}

	void NavDebugDrawDUOL::ConvertVertex()
	{
		auto& beforeVer = _debugManager->_currentModeVertices->back();

		std::swap(beforeVer._position.y, beforeVer._position.z);

		beforeVer._position = DUOLMath::Vector3::Transform(beforeVer._position, _convertMatrix);
	}

	void NavDebugDrawDUOL::depthMask(bool state)
	{
		_debugManager->_isDepthOn = state;
	}

	void NavDebugDrawDUOL::texture(bool state)
	{
		// Texture 미구현
	}

	void NavDebugDrawDUOL::begin(duDebugDrawPrimitives primitive, float size)
	{
		switch (primitive)
		{
			case DU_DRAW_POINTS:
			{
				_debugManager->_isDepthOn ?
					_debugManager->_currentModeVertices = &_debugManager->_navPointVertices : _debugManager->_currentModeVertices = &_debugManager->_navPointVerticesDepthOff;

				break;
			}

			case DU_DRAW_LINES:
			{
				_debugManager->_isDepthOn ?
					_debugManager->_currentModeVertices = &_debugManager->_navLineVertices : _debugManager->_currentModeVertices = &_debugManager->_navLineVerticesDepthOff;

				break;
			}

			case DU_DRAW_TRIS:
			{
				_debugManager->_isDepthOn ?
					_debugManager->_currentModeVertices = &_debugManager->_navTriangleVertices : _debugManager->_currentModeVertices = &_debugManager->_navTriangleVerticesDepthOff;

				break;
			}

			case DU_DRAW_QUADS:
			{
				_debugManager->_isDepthOn ?
					_debugManager->_currentModeVertices = &_debugManager->_navQuadVertices : _debugManager->_currentModeVertices = &_debugManager->_navQuadVerticesDepthOff;

				break;
			}
		};

		// Size 미구현
	}

	void NavDebugDrawDUOL::vertex(const float* pos, unsigned color)
	{
		_debugManager->_currentModeVertices->push_back({ DUOLMath::Vector3(pos), color });

		ConvertVertex();
	}

	void NavDebugDrawDUOL::vertex(const float x, const float y, const float z, unsigned color)
	{
		_debugManager->_currentModeVertices->push_back({ DUOLMath::Vector3(x, y, z), color });

		ConvertVertex();
	}

	void NavDebugDrawDUOL::vertex(const float* pos, unsigned color, const float* uv)
	{
		_debugManager->_currentModeVertices->push_back({ DUOLMath::Vector3(pos), color });

		// UV 미구현
		ConvertVertex();
	}

	void NavDebugDrawDUOL::vertex(const float x, const float y, const float z, unsigned color, const float u,
		const float v)
	{
		_debugManager->_currentModeVertices->push_back({ DUOLMath::Vector3(x, y, z), color });

		// UV 미구현
		ConvertVertex();
	}

	void NavDebugDrawDUOL::end()
	{
		// 딱히 할 일 없음. 포장하고 끝임.
	}

	NavDebugDrawDUOL_Poly::NavDebugDrawDUOL_Poly() :
		NavDebugDrawDUOL()
	{
	}

	void NavDebugDrawDUOL_Poly::begin(duDebugDrawPrimitives primitive, float size)
	{
		switch (primitive)
		{
			case DU_DRAW_POINTS:
			{
				_debugManager->_currentModeVertices = &_debugManager->_navPolyPointVertices;

				break;
			}

			case DU_DRAW_LINES:
			{
				_debugManager->_currentModeVertices = &_debugManager->_navPolyLineVertices;

				break;
			}

			case DU_DRAW_TRIS:
			{
				_debugManager->_currentModeVertices = &_debugManager->_navPolyTriangleVertices;

				break;
			}

			case DU_DRAW_QUADS:
			{
				//

				break;
			}
		};
	}
}