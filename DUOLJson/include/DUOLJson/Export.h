#pragma once


#if defined DUOLJSON_EXPORTS
#	define DUOLJSON_EXPORT __declspec(dllexport)
#else
#	define DUOLJSON_EXPORT __declspec(dllimport)
#endif

#if defined DUOLJSON_EXPORTS
#	define DUOLJSON_TEMPLATE_EXPORT
#else
#	define DUOLJSON_TEMPLATE_EXPORT extern
#endif