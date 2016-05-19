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
			engine->GetModule<Meridian::ResourceManager>(2)->CreateAsset<Meridian::AssetTexture>("assets/textures/sky0.png");
			engine->GetModule<Meridian::ResourceManager>(2)->CreateAsset<Meridian::AssetTexture>("assets/textures/sky1.png");
			engine->GetModule<Meridian::ResourceManager>(2)->CreateAsset<Meridian::AssetTexture>("assets/textures/sky2.png");

			engine->GetModule<Meridian::ResourceManager>(2)->LoadResources();
		}
	);

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
	engine->AddModule<Meridian::ResourceManager>();

	engine->Load();
	engine->Run(Meridian::MeridianEngine::GameLoopMode::VARIABLE);
	engine->Unload();
}