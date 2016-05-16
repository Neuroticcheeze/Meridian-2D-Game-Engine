#include <glfw3.h>

#include "Engine\Core\Module\Managers\ModuleInput.hpp"
#include "Engine\Core\Meridian.hpp"

void main()
{
	Meridian::MeridianEngine *engine = new Meridian::MeridianEngine();

	engine->AddModule<Meridian::InputManager>();
	engine->AddModule<Meridian::InputManager>();
	engine->AddModule<Meridian::InputManager>();
	engine->AddModule<Meridian::InputManager>();
	engine->AddModule<Meridian::InputManager>();

	engine->Load();
	engine->Run();
	engine->Unload();
}