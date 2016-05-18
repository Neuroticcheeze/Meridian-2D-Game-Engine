#include <glfw3.h>

#include "Engine\Core\Module\Managers\ModuleInput.hpp"
#include "Engine\Core\Meridian.hpp"


void main()
{

	Meridian::MeridianEngine *engine = new Meridian::MeridianEngine();

	/*Only need a module/manager for:
	
	Input
	Graphics
	Resource/Assets
	StateManager
	Networking
	AI
	*/
	for (int n = 0; n < 6; ++n)
	engine->AddModule<Meridian::InputManager>();
	
	//__if_not_exists(e) { static_assert(false, "'e' does not exist."); };

	engine->Load();
	engine->Run(Meridian::MeridianEngine::GameLoopMode::VARIABLE);
	engine->Unload();
}