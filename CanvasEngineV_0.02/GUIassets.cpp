#include "GUI.h"
#include "GUIassets.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_X;
extern int SCREEN_Y;
extern SDL_Window* mainWindow;
extern SDL_Renderer* mainRenderer;
extern bool quit;

draggableWindow::draggableWindow(int left, int top, int width, int height):gObj(top, left, width, height)
{
	setColor(255,255,255,0);
	Class = "draggable";
	drag = false;
}

void draggableWindow::onMouseDown()
{
	drag = true;
	int x;
	int y;
	SDL_GetMouseState(&x, &y);
	xOffset = x;
	yOffset = y;
}

void draggableWindow::onHover()
{
	if(drag)
	{
		int velX;
		int velY;
		SDL_GetMouseState(&velX, &velY);
		int xChange = velX - xOffset;
		int yChange = velY - yOffset;
		SDL_SetWindowPosition(mainWindow, SCREEN_X + xChange, SCREEN_Y + yChange);
		SDL_GetWindowPosition(mainWindow, &SCREEN_X, &SCREEN_Y);
	}
}

void draggableWindow::offHover()
{
	if(drag)
	{
		int velX;
		int velY;
		SDL_GetMouseState(&velX, &velY);
		int xChange = velX - xOffset;
		int yChange = velY - yOffset;
		SDL_SetWindowPosition(mainWindow, SCREEN_X + xChange, SCREEN_Y + yChange);
		SDL_GetWindowPosition(mainWindow, &SCREEN_X, &SCREEN_Y);
	}
}

void draggableWindow::onMouseUp()
{
	drag = false;
}

tab::tab(int left, int top, int width, int height, editorGUI* gui):gObj(top, left, width, height)
{
	container = gui;
	Class = "tab";
	this->setImage("img/button/dark.png");
	position = ABSOLUTE;
}

tab* tab::showTab()
{
	container->hideAll(Class);
	this->show = true;
	return this;
}

buttonTextures tabButton::texturepack;

tabButton::tabButton(int top, int left, int width, int height, tab* tabTarget, std::string text):gObj(top, left, width, height)
{
	if(!texturepack.init)
	{
		texturepack.onClick.loadImage("img/button/yellowDark.png");
		texturepack.onHover.loadImage("img/button/yellow.png");
		texturepack.onDefault.loadImage("img/button/grey.png");
	}
	target = tabTarget;
	fontHeight = 25;
	SDL_Color grey = {40, 40, 40};
	ctextTexture.texture.textColor = grey;
	setText(text);
	position = RELATIVE_LEFT;
	Class = "tabButton";
}

void tabButton::offHover()
{
	if(target->show)
	{
		backgroundTexture.texture = texturepack.onClick;
	} else {
		backgroundTexture.texture = texturepack.onDefault;
	}
}

void tabButton::onHover()
{
	backgroundTexture.texture = texturepack.onHover;
}
void tabButton::onMouseDown()
{
	backgroundTexture.texture = texturepack.onClick;
	target->showTab();
}

void tabButton::onMouseUp()
{
	printf("clicked\n");
	backgroundTexture.texture = texturepack.onHover;
}

boolToggler::boolToggler(int top, int left, int width, int height, bool* target, std::string text):gObj(top, left, width, height)
{
	this->target = target;
	SDL_Color grey = {40, 40, 40};
	ctextTexture.texture.textColor = grey;
	setText(text);
}

void boolToggler::onMouseUp()
{
	if(mousedown)
	{
		*target = !*target;
	}
}

void boolToggler::onMouseDown()
{

}

buttonTextures quitButton::texturepack;

quitButton::quitButton():boolToggler(0, SCREEN_WIDTH-50, 50, 40, &quit, "X")
{
	if(!texturepack.init)
	{
		texturepack.onClick.loadImage("img/button/yellowDark.png");
		texturepack.onHover.loadImage("img/button/yellow.png");
		texturepack.onDefault.loadImage("img/button/grey.png");
	}
	fontHeight = 25;
}

void quitButton::offHover()
{

	backgroundTexture.texture = texturepack.onHover;

}

void quitButton::onHover()
{
	backgroundTexture.texture = texturepack.onClick;
}
void quitButton::onMouseDown()
{
	backgroundTexture.texture = texturepack.onHover;
}

void quitButton::onMouseUp()
{
	if(mousedown)
	{
		*target = !*target;
	}
}

editorGUI::editorGUI()
{
	renderSem = SDL_CreateSemaphore(1);
	eventSem = SDL_CreateSemaphore(1);
	draggableWindow* drag = new draggableWindow(0,0,SCREEN_WIDTH,40);
	addgObj(drag);
	gObj* logo = new gObj(0, 0, 300, 40);
	logo->setImage("img/logo/logo.png");
	addgObj(logo);
	quitButton* qButton = new quitButton();
	addgObj(qButton);
	tab* firstTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addgObj(firstTab);
	tabButton* firstTabButton = new tabButton(180, 0, 100, 30, firstTab, "home");
	addgObj(firstTabButton);
	tab* secondTab = new tab(0, 40, SCREEN_WIDTH, 140, this);
	addgObj(secondTab);
	tabButton* secondTabButton = new tabButton(180, 100, 100, 30, secondTab, "debug");
	addgObj(secondTabButton);
	firstTab->showTab();
	gObj* partitioner = new gObj(178, 0, SCREEN_WIDTH, 2);
	partitioner->setImage("img/button/yellow.png");
	addgObj(partitioner);
	gObj* partitioner2 = new gObj(39, 0, SCREEN_WIDTH, 2);
	partitioner2->setImage("img/button/yellowDark.png");
	addgObj(partitioner2);

}

void editorGUI::handleMouse()
{
	while(eventBuffer.size() > 0)
	{
		SDL_SemWait(eventSem);
		SDL_Event e = eventBuffer.front();
		eventBuffer.pop_front();
		SDL_SemPost(eventSem);
		int relativeY = 0;
		int relativeX = 0;

		for(std::list<gObj*>::iterator it = children.begin(); it!=children.end(); it++)
		{
			if((*it)->position == RELATIVE)
			{
				(*it)->childHandleMouse(0 , 0, &e);

			}
			else if((*it)->position == RELATIVE_LEFT)
			{
				(*it)->childHandleMouse(0, 0, &e);
				relativeX += (*it)->width.v;
			} else (*it)->childHandleMouse(0, 0, &e);
		}

	}
}

bool editorGUI::initialize()
{
	return true;
}

bool editorGUI::simulate()
{

	timer.getDeltaTime();
	handleMouse();
	play(timer.deltaTime);
	return true;
}

void editorGUI::hideAll(std::string cls)
{

	for(std::list<gObj*>::iterator it = children.begin(); it != children.end(); it++)
	{

		if((*it)->Class == cls)
		{
			(*it)->show = false;
		}
	}

}

void editorGUI::addgObj(gObj* obj)
{
	children.push_back(obj);
}

void editorGUI::play(int timepassed)
{
	SDL_SemWait(renderSem);
	renderBuffer.clear();
	for(std::list<gObj*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->gatherRenderData();
		(*it)->render(&renderBuffer);
	}
	SDL_SemPost(renderSem);
}

void editorGUI::render(int x, int y)
{
		SDL_SemWait(renderSem);
		for(std::list<renderData>::iterator it = renderBuffer.begin(); it!=renderBuffer.end(); it++)
		{
			(*it).render();
		}
		SDL_SemPost(renderSem);
}