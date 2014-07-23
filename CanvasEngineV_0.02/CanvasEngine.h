#ifndef CANVAS_ENGINE_H
#define CANVAS_ENGINE_H

#include <SDL.h>
#include <SDL_thread.h>
#include "Engine.h"
#include "GUI.h"
#include "GUIassets.h"
#include "World.h"

class CanvasEngine:public CanvasSystem
{
public:
	bool quit;
	SDL_Thread* worldThread;
	SDL_Thread* guiThread;
	SDL_Thread* eventThread;
	World* world;
	editorGUI* gui;
	CanvasEngine();
	~CanvasEngine();
	bool initialize();
	int WorldThread();
	int GUIThread();
	int PollEventThread();
	bool simulate();
	bool render();
	bool end();
};

#endif