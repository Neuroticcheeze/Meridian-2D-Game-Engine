#include "Engine\Managers\ModuleInput.hpp"
#include "Engine\Managers\ModuleGraphics.hpp"
#include "Engine\Managers\ModuleResource.hpp"
#include "Engine\Assets\AssetTexture.hpp"
#include "Engine\Core\Meridian.hpp"


void main()
{

	Meridian::MeridianEngine *engine = new Meridian::MeridianEngine();

	engine->HookToPreEvent([]
		(Meridian::MeridianEngine * engine)
		{
			//auto resourceManager = engine->GetResourceManager();
			//resourceManager->CreateAsset<Meridian::AssetTexture>("test_texture", resourceManager->GetPath("test.png"));
			//resourceManager->CreateAsset<Meridian::AssetTexture>("tree", resourceManager->GetPath("tree.png"));

			//Meridian::AssetTexture * test = nullptr;
			//resourceManager->GetAsset("test_texture", &test);

			//resourceManager->SaveResources();
			//resourceManager->LoadResources();
		}
	);

	engine->HookToLoopEvent([]
		(Meridian::MeridianEngine * engine, const float & dt) 
		{
			//const uvec2 & pos = engine->GetInputManager()->GetMousePosition();
			//printf("Mouse pos: [%u, %u]\n", pos.x, pos.y);

			if (engine->GetInputManager()->GetKeyState(GLFW_KEY_A) == Meridian::InputManager::KeyState::DOWN) printf("down\n");
			if (engine->GetInputManager()->GetKeyState(GLFW_KEY_A) == Meridian::InputManager::KeyState::UP) printf("up\n");
			if (engine->GetInputManager()->GetKeyState(GLFW_KEY_A) == Meridian::InputManager::KeyState::TAP) printf("tap\n");
			if (engine->GetInputManager()->GetKeyState(GLFW_KEY_A) == Meridian::InputManager::KeyState::RELEASE) printf("release\n");
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

	engine->Load();
	engine->Run(Meridian::MeridianEngine::GameLoopMode::VARIABLE);
	engine->Unload();
}