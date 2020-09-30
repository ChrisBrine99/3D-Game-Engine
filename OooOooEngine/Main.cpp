#include "Engine/Core/CoreEngine.h"
#include "Game/Game1.h"

int main(int argc, char* argv[]) {
	CoreEngine::GetInstance()->SetGameInterface(new Game1());
	if (!CoreEngine::GetInstance()->OnCreate("OooOoo Engine", 1280, 720)) { // Renderer couldn't initialize; exit the program
		DebugLogger::FatalError("OooOoo Engine has failed to start.", "Main.cpp", __LINE__);
		return 1;
	}
	// Run the main game loop
	CoreEngine::GetInstance()->Run();
	return 0;
}