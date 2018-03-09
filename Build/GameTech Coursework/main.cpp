#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>

#include <nclgl\NCLDebug.h>
#include <nclgl\PerfTimer.h>
#include <ncltech\OcTree.h>
#include "EmptyScene.h"
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <nclgl\ResourceManager.h>
#include "MainMenu.h"

#include <nclgl\GameLogic.h>

#include <ncltech\Memory Management\HeapFactory.h>
using namespace nclgl::Maths;

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_perf_metrics = false;
PerfTimer timer_total, timer_physics, timer_update, timer_render;
uint shadowCycleKey = 4;

int idx = 0;


// Program Deconstructor
//  - Releases all global components and memory
//  - Optionally prints out an error message and
//    stalls the runtime if requested.
void Quit(bool error = false, const std::string &reason = "") {
	//Release Singletons
	SceneManager::Release();
	PhysicsEngine::Release();
	GraphicsPipeline::Release();
	ResourceManager::Release();

	//Show console reason before exit
	if (error) {
		std::cout << reason << std::endl;
		system("PAUSE");
		exit(-1);
	}
}


// Program Initialise
//  - Generates all program wide components and enqueues all scenes
//    for the SceneManager to display
void Initialize()
{
	//Initialize Renderer
	GraphicsPipeline::Instance();

	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();

	//Enqueue All Scenes
	SceneManager::Instance()->EnqueueScene(new MainMenu("Main Menu"));
	SceneManager::Instance()->EnqueueScene(new EmptyScene("Team Project", idx = 1));

	// Move this once main menu is hooked up
	//AudioFactory::Instance()->GetAudioEngine()->PlaySound2D(SOUNDSDIR"Intro.wav", false);
}

// Print Debug Info
//  - Prints a list of status entries to the top left
//    hand corner of the screen each frame.
void PrintStatusEntries()
{
	//Print Engine Options
	NCLDebug::AddStatusEntry(status_colour_header, "NCLTech Settings");
	NCLDebug::AddStatusEntry(status_colour, "     Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused  " : "Enabled ");
	//NCLDebug::AddStatusEntry(status_colour, "     Monitor V-Sync: %s (Press V to toggle)", GraphicsPipeline::Instance()->GetVsyncEnabled() ? "Enabled " : "Disabled");
	NCLDebug::AddStatusEntry(status_colour, "");


	//Print Performance Timers
	NCLDebug::AddStatusEntry(status_colour, "     FPS: %5.2f  (Press G for %s info)", 1000.f / timer_total.GetAvg(), show_perf_metrics ? "less" : "more");
	if (show_perf_metrics)
	{
		timer_total.PrintOutputToStatusEntry(status_colour, "          Total Time     :");
		timer_update.PrintOutputToStatusEntry(status_colour, "          Scene Update   :");
		timer_physics.PrintOutputToStatusEntry(status_colour, "          Physics Update :");
		timer_render.PrintOutputToStatusEntry(status_colour, "          Render Scene   :");

	}
	NCLDebug::AddStatusEntry(status_colour, "");
	NCLDebug::AddStatusEntry(status_colour, "Bytes: " + std::to_string(HeapFactory::Instance()->PrintDebugInfo()));
	NCLDebug::AddStatusEntry(status_colour, "Peak Bytes: " + std::to_string(HeapFactory::Instance()->PrintPeakInfo()));
	//NCLDebug::AddStatusEntry(status_colour, "Net Allocation: " + std::to_string(HeapFactory::Instance()->PrintNetAllocInfo()));

}

// Process Input
//  - Handles all program wide keyboard inputs for
//    things such toggling the physics engine and 
//    cycling through scenes.
void HandleKeyboardInputs()
{

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P));
	//PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
	//	GraphicsPipeline::Instance()->SetVsyncEnabled(!GraphicsPipeline::Instance()->GetVsyncEnabled());

	uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
	uint sceneMax = SceneManager::Instance()->SceneCount();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y))
	SceneManager::Instance()->JumpToScene((sceneIdx + 1) % sceneMax);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
	SceneManager::Instance()->JumpToScene((sceneIdx == 0 ? sceneMax : sceneIdx) - 1);

	/*if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	SceneManager::Instance()->JumpToScene(sceneIdx);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
	OcTree::toggle();*/

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G))
		show_perf_metrics = !show_perf_metrics;



	


}


// Program Entry Point
int main()
{
	bool show_status_menu = false;
	//Initialize our Window, Physics, Scenes etc
	Initialize();
	//GraphicsPipeline::Instance()->SetVsyncEnabled(false);

	Window::GetWindow().GetTimer()->GetTimedMS();
	GameLogic::Instance()->setLevelIsLoaded(false);
	GameLogic::Instance()->setGameHasStarted(false);
	//Create main game-loop
	while (Window::GetWindow().UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		//Start Timing

		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?
																			//Update Performance Timers (Show results every second)
		timer_total.UpdateRealElapsedTime(dt);
		timer_physics.UpdateRealElapsedTime(dt);
		timer_update.UpdateRealElapsedTime(dt);
		timer_render.UpdateRealElapsedTime(dt);
		//std::cout << "\nFPS: " + std::to_string(1000.f / timer_total.GetAvg());
		//Print Status Entries
		//PrintStatusEntries();

		//Handle Keyboard Inputs
		HandleKeyboardInputs();


		timer_total.BeginTimingSection();

		//Update Scene
		timer_update.BeginTimingSection();
		SceneManager::Instance()->GetCurrentScene()->OnUpdateScene(dt);
		timer_update.EndTimingSection();

		//Update Physics	
		timer_physics.BeginTimingSection();
		PhysicsEngine::Instance()->Update(dt);
		timer_physics.EndTimingSection();
		PhysicsEngine::Instance()->DebugRender();

		//Render Scene
		timer_render.BeginTimingSection();
		GraphicsPipeline::Instance()->UpdateScene(dt);

		// Update Audio
		AudioFactory::Instance()->GetAudioEngine()->Update(dt);


		// Remove this once main menu is hooked up

		if (SceneManager::Instance()->GetCurrentSceneIndex() == 0)
		{
			GraphicsPipeline::Instance()->RenderMenu();
		}
		else
		{
			if (GameLogic::Instance()->levelIsLoaded())
			{
				GameLogic::Instance()->increaseTotalTime(dt);
				GraphicsPipeline::Instance()->RenderScene(dt);

			}
			//Print Status Entries
			if (show_status_menu)
			{
				NCLDebug::_ClearDebugLists();
				PrintStatusEntries();
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
				show_status_menu = !show_status_menu;
			}
		
			
		}




		{
			//Forces synchronisation if vsync is disabled
			// - This is solely to allow accurate estimation of render time
			// - We should NEVER normally lock our render or game loop!		
			//	glClientWaitSync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, NULL), GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}
		timer_render.EndTimingSection();



		//Finish Timing
		timer_total.EndTimingSection();
	}
	//HeapFactory::Instance()->PrintDebugInfo();
	//Cleanup
	Quit();
	//system("pause");
	return 0;
}