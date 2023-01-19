#pragma once

#if defined DUOLGAMEENGINEDLL_EXPORTS
#	define DUOL_GAMEENGINE_API __declspec(dllexport)
#else
#	define DUOL_GAMEENGINE_API __declspec(dllimport)
#endif

#if defined DUOLGAMEENGINEDLL_EXPORTS
#	define DUOL_GAMEENGINE_TEMPLATE_API
#else
#	define DUOL_GAMEENGINE_TEMPLATE_API extern
#endif