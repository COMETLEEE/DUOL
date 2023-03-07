#pragma once
#include "DUOLGameEngine/Navigation/DebugUtils/DebugDraw.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class DebugManager;
}

namespace DUOLGameEngine
{
	/**
	 * \brief 
	 */
	struct NavDebugVertex
	{
		DUOLMath::Vector3 _position;

		unsigned int _vertexColor;
	};

	/**
	 * \brief NavDebugDraw 
	 */
	class NavDebugDrawDUOL : public duDebugDraw
	{
	public:
		NavDebugDrawDUOL();

		DUOLGameEngine::DebugManager* _debugManager;

		DUOLMath::Matrix _convertMatrix;

		void ConvertVertex();

		virtual void depthMask(bool state);
		virtual void texture(bool state);
		virtual void begin(duDebugDrawPrimitives primitive, float size = 1.0f);
		virtual void vertex(const float* pos, unsigned int color);
		virtual void vertex(const float x, const float y, const float z, unsigned int color);
		virtual void vertex(const float* pos, unsigned int color, const float* uv);
		virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
		virtual void end();
	};
}