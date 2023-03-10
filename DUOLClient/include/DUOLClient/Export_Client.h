#pragma once

#if defined DUOLCLIENT_EXPORTS
#	define DUOL_CLIENT_API __declspec(dllexport)
#else
#	define DUOL_CLIENT_API __declspec(dllimport)
#endif

#if defined DUOLCLIENT_EXPORTS
#	define DUOL_CLIENT_TEMPLATE_API
#else
#	define DUOL_CLIENT_TEMPLATE_API extern
#endif
