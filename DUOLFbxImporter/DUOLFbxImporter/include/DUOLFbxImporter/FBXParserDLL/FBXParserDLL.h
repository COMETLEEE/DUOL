#pragma once

#ifdef DUOLFBXIMPORTER_EXPORTS
#define FBXIMPORTER_DLL __declspec(dllexport)
#else
#define FBXIMPORTER_DLL __declspec(dllimport)
#endif