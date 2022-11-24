#pragma once

#ifdef GRAPHICSENGINE_EXPORTS
#define GRPHICHS_API __declspec(dllexport)
#else
#define GRPHICHS_API __declspec(dllimport)
#endif
