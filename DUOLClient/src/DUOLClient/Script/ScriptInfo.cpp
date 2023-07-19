#include "DUOLClient/Script/ScriptInfo.h"

DUOLClient::ScriptInfo::ScriptInfo(DUOLCommon::tstring nowpath, DUOLCommon::tstring nextpath1, DUOLCommon::tstring nextpath2, float lifeTime,
	bool active, bool isshow) :
	_nowPath(nowpath), _nextPath{ nextpath1,nextpath2 }, _lifeTime(lifeTime), _isActive(active), _isShow(isshow)
{
}
