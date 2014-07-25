#ifndef GUIASSETS_H
#define GUIASSETS_H

#include "GUI.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include "Engine.h"

class editorGUI;
class World;
class AiEngine;

static struct buttonTextures{
	bool init;
	CanvasTexture onHover;
	CanvasTexture onClick;
	CanvasTexture onDefault;
	buttonTextures(){
		init = false;
	}
};

class draggableWindow:public gObj
{
public:
	bool drag;
	int xOffset;
	int yOffset;
	draggableWindow(int left, int top, int width, int height);
	virtual void onMouseDown();
	virtual void onMouseUp();
	virtual void onHover();
	virtual void offHover();
};

class tab:public gObj
{
public:
	editorGUI* container;
	tab(int left, int top, int width, int height, editorGUI* gui);
	tab* showTab();
};

class infoTab:public gObj
{
public:
};

class aiRenderTab:public tab
{
public:
	AEngine* ai;
	void generateGraphic();
	
};

class tabButton:public gObj
{
public:
	static buttonTextures texturepack;
	tab* target;
	tabButton(int top, int left, int width, int height, tab* tabTarget, std::string text);
	virtual void onHover();
	virtual void offHover();
	virtual void onMouseDown();
	virtual void onMouseUp();
};

class boolToggler:public gObj
{
public:
	bool* target;
	boolToggler(int x, int y, int width, int height, bool* target, std::string text);
	virtual void onMouseUp();
	virtual void onMouseDown();
};

class quitButton:public boolToggler
{
public:
	static buttonTextures texturepack;
	quitButton();
	virtual void onHover();
	virtual void offHover();
	virtual void onMouseDown();
	virtual void onMouseUp();
};

class GUIObj
{
public:
	renderData background;
	Cint height, width, left, top;
	GUIObj(){}
	virtual void onMouseDown(){}
	virtual void onMouseUp(){}
	virtual void onHover(){}
	virtual void offHover(){}
	virtual void render(){}
};

class CanvasButton:public GUIObj
{
public:
	renderData overlay;
	renderData text;
	void onMouseDown(){}
};

class GUI:public CanvasSystem
{
public:
	std::list<GUIObj*> canvasObjects;
	GUI();
	void handleEvents(SDL_Event* e);

};

class editorGUI: public CanvasSystem
{
public:
	std::list<SDL_Event> eventBuffer;
	std::list<renderData> renderBuffer;
	SDL_sem* renderSem;
	SDL_sem* eventSem;
	std::list<gObj*> children;
	editorGUI();
	virtual void handleMouse();
	virtual void getEvent(SDL_Event e){
		SDL_SemWait(eventSem);
		eventBuffer.push_back(e);
		SDL_SemPost(eventSem);
	}
	virtual void play(int timepassed);
	bool initialize();
	bool simulate();
	void hideAll(std::string cls);
	void addgObj( gObj* obj);
	void render(int x, int y);
};

#endif