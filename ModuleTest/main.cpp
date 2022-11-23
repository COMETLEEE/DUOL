#include <crtdbg.h>
//#include "BTTest.h"
#include "PhysicsTest.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//BTTestCode();

	PhysicsTestCode();

	return 0;
}