#pragma once


#if defined DUOLGRAPHICSLIBRARY_EXPORTS
#	define DUOLGRAPHICSLIBRARY_EXPORT __declspec(dllexport)
#else
#	define DUOLGRAPHICSLIBRARY_EXPORT __declspec(dllimport)
#endif

#if defined DUOLGRAPHICSLIBRARY_EXPORTS
#	define DUOLGRAPHICSLIBRARY_TEMPLATE_EXPORT
#else
#	define DUOLGRAPHICSLIBRARY_TEMPLATE_EXPORT extern
#endif