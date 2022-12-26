#pragma once


#if defined DUOLGRAPHICSENGINE_EXPORTS
#	define DUOLGRAPHICSENGINE_EXPORT __declspec(dllexport)
#else
#	define DUOLGRAPHICSENGINE_EXPORT __declspec(dllimport)
#endif

#if defined DUOLGRAPHICSENGINE_EXPORTS
#	define DUOLGRAPHICSENGINE_TEMPLATE_EXPORT
#else
#	define DUOLGRAPHICSENGINE_TEMPLATE_EXPORT extern
#endif