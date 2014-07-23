#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>



class CanvasTimer
{
public:
	CanvasTimer(){
		set = true;
		deltaTime = 0; 
		lastFrameTime = 0;
	}
	bool beginFrame();
	bool set;
	int deltaTime;
	int lastFrameTime;
	int getDeltaTime()
	{
		if(set)
		{
			deltaTime = SDL_GetTicks()-lastFrameTime;
			lastFrameTime = SDL_GetTicks();
		}
		return deltaTime;
	}
	int setDeltaTime(int delta)
	{
		deltaTime = delta;
	}
};

class CanvasSystem{
public:
	static int coreCount;
	CanvasTimer timer;
	CanvasSystem(){}
	virtual bool initialize();
	bool decrementCore();
	bool incrementCore();
	virtual bool simulate();
	virtual bool end();
};



#endif