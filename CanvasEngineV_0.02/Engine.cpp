#include "Engine.h"
#include "CanvasEngine.h"
#include <SDL.h>
#include <SDL_thread.h>

extern bool quit;

extern SDL_Renderer* mainRenderer;

int CanvasSystem::coreCount = SDL_GetCPUCount();

int initWorldThread(void* data);
int initGUIThread(void* data);
int initPollEvent(void* data);



int initGUIThread(void* data)
{
	CanvasEngine* instance = (CanvasEngine*)data;
	while(!instance->quit)
	{
		instance->GUIThread();
	}
	return 1;
}

int initWorldThread(void* data)
{
	CanvasEngine* instance = (CanvasEngine*)data;
	while(!instance->quit)
	{
		instance->WorldThread();
	}
	return 1;
}

int initPollEvent(void* data)
{
	CanvasEngine* instance = (CanvasEngine*)data;
	while(!instance->quit)
	{
		instance->PollEventThread();
	}
	return 1;
}

bool CanvasSystem::initialize()
{
	return true;
}

bool CanvasSystem::simulate()
{
	return true;
}

bool CanvasSystem::end()
{
	return false;
}

bool CanvasSystem::decrementCore()
{
	coreCount--;
	return true;
}

bool CanvasSystem::incrementCore()
{
	coreCount++;
	return false;
}

CanvasEngine::CanvasEngine()
{
	world = NULL;
	gui = NULL;
	worldThread = NULL;
	guiThread = NULL;
	eventThread = NULL;
	quit = false;
}

bool CanvasEngine::initialize()
{
	if(gui!=NULL)
	{
		guiThread = SDL_CreateThread(initGUIThread, "GUIThread", (void*)this);
	}
	return true;
}

int CanvasEngine::WorldThread()
{
	world->simulate();
	return 0;
}

int CanvasEngine::GUIThread()
{
	gui->simulate();
	return 0;
}

bool CanvasEngine::simulate()
{
	timer.getDeltaTime();
	if(worldThread == NULL)
	{
		world->timer.set = false;
		world->timer.deltaTime = timer.deltaTime;
		world->simulate();
	}
	if(guiThread == NULL)
	{
		gui->timer.set = false;
		gui->timer.deltaTime = timer.deltaTime;
		gui->simulate();
	}
	return true;
}

int CanvasEngine::PollEventThread()
{
	SDL_Event e;
	while(SDL_PollEvent(&e)>0)
	{
		if(world!=NULL)
			world->getEvents(e);
		if(gui!=NULL)
		{
			printf("lol\n");
			gui->handleMouse();
		}
	}	
			printf("lol\n");
	return 1;
}

bool CanvasEngine::end()
{
	this->quit = false;
	return true;
}

bool CanvasEngine::render()
{
	int time = SDL_GetTicks();
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		if(gui!=NULL)
		{
			gui->getEvent(e);
		}
	}
	SDL_SetRenderDrawColor(mainRenderer,40,40,40,255);
	SDL_RenderClear(mainRenderer);
	gui->render(0,0);
	SDL_RenderPresent(mainRenderer);
	printf("%d\n", SDL_GetTicks()-time);
	return true;
}

