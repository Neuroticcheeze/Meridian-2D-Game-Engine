#include "Engine\Managers\ModuleInput.hpp"
#include "Engine\Managers\ModuleGraphics.hpp"
#include "Engine\Core\Meridian.hpp"


void main()
{

	Meridian::MeridianEngine *engine = new Meridian::MeridianEngine();

	engine->HookToLoopEvent([]
		(Meridian::MeridianEngine * engine, const float & dt) 
		{
			const uvec2 & pos = engine->GetModule<Meridian::InputManager>(0)->GetMousePosition();
			printf("Mouse pos: [%u, %u]\n", pos.x, pos.y);
		}
	);

	/*Only need a module/manager for:
	
	Input
	Graphics
	Resource/Assets
	StateManager
	Networking
	AI
	*/
	engine->AddModule<Meridian::InputManager>();
	engine->AddModule<Meridian::GraphicsManager>();

	engine->Load();
	engine->Run(Meridian::MeridianEngine::GameLoopMode::VARIABLE);
	engine->Unload();
}