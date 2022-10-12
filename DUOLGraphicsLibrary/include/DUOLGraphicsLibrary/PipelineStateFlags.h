#pragma once

namespace DUOLGraphicsLibrary
{
	struct Viewport
	{
		inline Viewport(int left, int top, int width, int height, float minDepth, float maxDepth) :
			_left(left)
			, _top(top)
			, _width(width)
			, _height(height)
			, _minDepth(minDepth)
			, _maxDepth(maxDepth)
		{

		}

		int _left;

		int _top;

		int _width;

		int _height;

		float _minDepth;

		float _maxDepth;
	};
}
