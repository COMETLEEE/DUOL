#pragma once

#if defined DUOLSOUND_EXPORTS
	#define DUOL_SOUND_API __declspec(dllexport)
#else
	#define DUOL_SOUND_API __declspec(dllimport)
#endif

#if defined DUOLSOUND_EXPORTS
#	define DUOL_SOUND_TEMPLATE_API
#else
#	define DUOL_SOUND_TEMPLATE_API extern
#endif