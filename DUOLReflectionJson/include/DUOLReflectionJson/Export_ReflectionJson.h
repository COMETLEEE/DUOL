#pragma once

#if defined DUOLREFLECTIONJSON_EXPORTS
#	define DUOL_REFLECTION_JSON_API __declspec(dllexport)
#else
#	define DUOL_REFLECTION_JSON_API __declspec(dllimport)
#endif

#if defined DUOLREFLECTIONJSON_EXPORTS
#	define DUOL_REFLECTION_JSON_TEMPLATE_API
#else
#	define DUOL_REFLECTION_JSON_TEMPLATE_API extern
#endif