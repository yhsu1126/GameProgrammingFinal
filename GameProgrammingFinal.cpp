#include "stdafx.h"
#include "GameProgrammingFinal.h"
using namespace std;

void FyMain(int argc, char **argv)
{
	FySetShaderPath("Data\\Shaders");
	OutputDebugString(_T("My output string.OAO OAO\n"));
	FnCharacter actor;
	// invoke the system
	FyInvokeFly(TRUE);
}