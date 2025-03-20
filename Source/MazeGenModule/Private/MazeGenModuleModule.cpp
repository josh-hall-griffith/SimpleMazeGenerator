#include "MazeGenModuleModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FMazeGenModule, MazeGenModule);

void FMazeGenModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("MazeGenModule has started!"));
}

void FMazeGenModule::ShutdownModule()
{
}
