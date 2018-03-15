#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>

#include <nclgl\NCLDebug.h>
#include <nclgl\PerfTimer.h>
#include <ncltech\OcTree.h>
#include "EmptyScene.h"
#include "iostream"
#include "fstream"
#include "LanScene.h"
#include "MainMenu.h"
#include <nclgl\Audio\AudioFactory.h>
#include <nclgl\Audio\AudioEngineBase.h>
#include <nclgl\ResourceManager.h>


#include <ncltech\Memory Management\HeapFactory.h>
using namespace nclgl::Maths;

const Vector4 status_colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4 status_colour_header = Vector4(0.8f, 0.9f, 1.0f, 1.0f);

bool show_perf_metrics = false;
PerfTimer timer_total, timer_physics, timer_update, timer_render;
uint shadowCycleKey = 4;
GameTimer* clock_timer;
bool gameEnded = false;
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
	SceneManager::Instance()->EnqueueScene(new EmptyScene("Team Project"));

	SceneManager::Instance()->EnqueueScene(new LanScene("Lan Project"));
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

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P));
	//PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	//if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
	//	GraphicsPipeline::Instance()->SetVsyncEnabled(!GraphicsPipeline::Instance()->GetVsyncEnabled());

	uint sceneIdx = SceneManager::Instance()->GetCurrentSceneIndex();
	uint sceneMax = SceneManager::Instance()->SceneCount();

	/*if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Y))
	SceneManager::Instance()->JumpToScene((sceneIdx + 1) % sceneMax);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
	SceneManager::Instance()->JumpToScene((sceneIdx == 0 ? sceneMax : sceneIdx) - 1);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	SceneManager::Instance()->JumpToScene(sceneIdx);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
	OcTree::toggle();*/

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_G))
		show_perf_metrics = !show_perf_metrics;


	Vector3 pos;
	std::vector<Vector3> posList;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
	{
		
		pos = GameLogic::Instance()->getSoftPlayer(0)->getBottom()->Physics()->GetPosition();
		std::ofstream myfile;
		myfile.open("pos.txt", std::ios_base::app);
		cout << pos << "\n";

		if (myfile.is_open())
		{
			myfile << pos << "\n";
			myfile.close();
		}
	}
}


// Program Entry Point
int main()
{
	bool show_status_menu = false;
	bool showEndScreen = false;
	clock_timer = new GameTimer();
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
				if (GameLogic::Instance()->gameHasStarted() && !GameLogic::Instance()->getIsGamePaused()) {
					GameLogic::Instance()->increaseActualGameTime(dt);
				}
				GameLogic::Instance()->increaseTotalTime(dt);
				GraphicsPipeline::Instance()->RenderScene(dt);
			}
			NCLDebug::_ClearDebugLists();
			if (GameLogic::Instance()->getCurrentTime() > 0) {
				GameLogic::Instance()->setCurrentTime((int)(GameLogic::Instance()->getSeconds() - GameLogic::Instance()->getActualGameTime()));
				NCLDebug::AddTimer(nclgl::Maths::Vector4(1.f, 1.f, 1.f, 1.f), std::to_string(GameLogic::Instance()->getCurrentTime() / 60) + ":" + std::to_string((GameLogic::Instance()->getCurrentTime() % 60) / 10) + std::to_string((GameLogic::Instance()->getCurrentTime() % 60) % 10));
			}
			else if (!gameEnded) {
				showEndScreen = true;
				PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			}
			//Print Status Entries
			if (show_status_menu)
			{
				//NCLDebug::_ClearDebugLists();
				PrintStatusEntries();
			}
			if (showEndScreen) {
				gameEnded = true;
				
				int j = GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers();
				int l = GameLogic::Instance()->getNumAIPlayers()+ GameLogic::Instance()->getNumSoftPlayers() + GameLogic::Instance()->getNumNetPlayers();

				int winningPlayer = 0;
				int aiWinningPlayer = 0;

				float maxScore = -1.0f;
				for (int i = 0; i < l; i++) {
					if ((*GameLogic::Instance()->getPaintPerc())[i] > maxScore) {
						maxScore = (*GameLogic::Instance()->getPaintPerc())[i];
						winningPlayer = i;
					}
				}
				NCLDebug::_ClearDebugLists();
				if (GameLogic::Instance()->getNumNetPlayers() > 0) {
					if (winningPlayer == 0)NCLDebug::AddHUD2(GameLogic::Instance()->getSoftPlayer(0)->getColour(), "You win!");
					else {
						NCLDebug::AddHUD2(GameLogic::Instance()->getNetPlayer(winningPlayer -1)->getColour(), "Player "+ to_string(winningPlayer +1)+" WINS!");
					}
				}
				else {
					if (winningPlayer < j)
					{
						int tag = GameLogic::Instance()->getSoftPlayer(winningPlayer)->getBall()->softball[winningPlayer]->GetTag();
						if (tag == 2)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getSoftPlayer(winningPlayer)->getColour(), "Player 1 WINS!");
						}
						else if (tag == 3)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getSoftPlayer(winningPlayer)->getColour(), "Player 2 WINS!");
						}
						else if (tag == 4)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getSoftPlayer(winningPlayer)->getColour(), "Player 3 WINS!");
						}
						else if (tag == 5)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getSoftPlayer(winningPlayer)->getColour(), "Player 4 WINS!");
						}
					}
					else
					{
						aiWinningPlayer = winningPlayer - j;
						int tag = GameLogic::Instance()->getAIPlayer(aiWinningPlayer)->GetTag();
						if (tag == 2)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getAIPlayer(winningPlayer)->getColour(), "AI Player 1 WINS!");
						}
						else if (tag == 3)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getAIPlayer(winningPlayer)->getColour(), "AI Player 2 WINS!");
						}
						else if (tag == 4)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getAIPlayer(winningPlayer)->getColour(), "AI Player 3 WINS!");
						}
						else if (tag == 5)
						{
							NCLDebug::AddHUD2(GameLogic::Instance()->getAIPlayer(winningPlayer)->getColour(), "AI Player 4 WINS!");
						}
						NCLDebug::AddHUD2(GameLogic::Instance()->getAIPlayer(aiWinningPlayer)->getColour(), "AI Player " + to_string(aiWinningPlayer + 1) + " WINS!");
					}
				}

				NCLDebug::AddHUD(nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Press 1 to Restart");
				NCLDebug::AddHUD(nclgl::Maths::Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Press 2 to Exit");
				//PrintStatusEntries();
				if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1))
				{
					showEndScreen = !showEndScreen;
					GameLogic::Instance()->clearGameLogic();
					GraphicsPipeline::Instance()->clearGraphicsPipeline();
					PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
					SceneManager::Instance()->JumpToScene("Team Project");
					
					GameLogic::Instance()->setIsGamePaused(false);
					GameLogic::Instance()->setLevelIsLoaded(false);
					GameLogic::Instance()->setGameHasStarted(false);
					GameLogic::Instance()->setTotalTime(0.0f);
					GameLogic::Instance()->setActualGameTime(0.0f);
					GameLogic::Instance()->setCurrentTime(GameLogic::Instance()->getSeconds());
					gameEnded = false;
				}
				if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2))
				{
					showEndScreen = !showEndScreen;
					GameLogic::Instance()->clearGameLogic();
					GraphicsPipeline::Instance()->clearGraphicsPipeline();
					PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
					SceneManager::Instance()->JumpToScene("Main Menu");
					
					GameLogic::Instance()->setIsGamePaused(false);
					GameLogic::Instance()->setLevelIsLoaded(false);
					GameLogic::Instance()->setGameHasStarted(false);
					GameLogic::Instance()->setTotalTime(0.0f);
					GameLogic::Instance()->setActualGameTime(0.0f);
					GameLogic::Instance()->setCurrentTime(GameLogic::Instance()->getSeconds());
					gameEnded = false;
				}
			}
			if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
				show_status_menu = !show_status_menu;
			}
			

			/*else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_5)) {
				showEndScreen = !showEndScreen;
				PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());
			}*/
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