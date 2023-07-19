#pragma once
#include "DUOLCommon/StringHelper.h"

namespace DUOLClient
{
	class ScriptInfo
	{
	public:
		ScriptInfo(DUOLCommon::tstring _nowPath, DUOLCommon::tstring nextpath1, DUOLCommon::tstring nextpath2, float lifeTime, bool active, bool isshow);

	public:

		DUOLCommon::tstring _nowPath;
		DUOLCommon::tstring _nextPath[2];
		float _lifeTime;
		bool _isActive;

		bool _isShow;
	};
}