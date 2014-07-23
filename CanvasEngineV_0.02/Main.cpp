#include "Engine.h"
#include "CanvasEngine.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GUI.h"
#include "GUIassets.h"
#include <string>
#include "physicsShape.h"




SDL_Window* mainWindow = NULL;
SDL_Renderer* mainRenderer = NULL;
TTF_Font* CanvasFont = NULL;
SDL_sem* renderLock ;

int SCREEN_WIDTH = 1680;
int SCREEN_HEIGHT = 1020;
int SCREEN_X = 0;
int SCREEN_Y = 0;

int lastFrameTime = 0;
int deltaTime = 0;
int avgFrameTime = 0;
int frames = 0;
int totalFrameTime = 0;

bool quit = false;
bool CanvasDebug = true;

static int GetDeltaTime()
{
	int DeltaTime = SDL_GetTicks() - lastFrameTime;
	if (DeltaTime > 33)
		DeltaTime = 33;
	deltaTime = DeltaTime;
	return deltaTime;
}

static void setFrame()
{
	GetDeltaTime();
	lastFrameTime = SDL_GetTicks();
}

bool Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL init error %s\n", SDL_GetError());
		return false;
	}
	else
	{
		mainWindow = SDL_CreateWindow("Canvas Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS);
		if(mainWindow == NULL)
		{
			printf("SDL create window error %s\n", SDL_GetError());
			return false;
		}
		else{
			SDL_GetWindowSize(mainWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
			SDL_GetWindowPosition(mainWindow, &SCREEN_X, &SCREEN_Y);
			mainRenderer = SDL_CreateRenderer( mainWindow, -1, SDL_RENDERER_ACCELERATED);
			if(mainRenderer == NULL)
			{
				printf("SDL_CreateRenderer error %s\n", SDL_GetError());
				return false;
			}
				else{
					SDL_SetRenderDrawColor( mainRenderer, 255, 255, 255, 255 );
					int imgflags = IMG_INIT_PNG;
					if(!(IMG_Init(imgflags) & imgflags))
					{
						printf("IMG_Init faild %s\n", IMG_GetError());
						return false;
					}

					if(TTF_Init() == -1)
					{
						printf("SDL_ttf initialization error %s\n", TTF_GetError());
						return false;
					}
					else
					{
						CanvasFont = TTF_OpenFont("font/Tahoma.ttf", 72);
						if(CanvasFont == NULL)
						{
							printf("TTF_OpenFont failed %s\n", TTF_GetError());
							return false;
						}
						else
						{
							printf("init success\n");
						}
					}
				}
			}
	}
	return true;
}

void close()
{
	SDL_DestroyRenderer(mainRenderer);
	mainRenderer == NULL;
	SDL_DestroyWindow(mainWindow);
	mainWindow == NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	
	if(Init())
	{
		physicsEngine engine;
		physicsCircle* circle = new physicsCircle(20);
		physicsBody body(circle, 200, 40);
		body.setOrient(0.2);
		physicsPolygon* poly = new physicsPolygon();
		poly->setBox(100, 100);
		poly->setOrient(0);
		physicsPolygon* poly2 = new physicsPolygon();
		poly2->setBox(20, 20);
		poly2->setOrient(1);
		physicsBody otherBod(poly, 200, 400);
		physicsBody newBod(poly2, 200,100);
		physicsBody othernewBod(poly2, 200, 0 );
		othernewBod.material.dynamicFriction = 0.3;
		othernewBod.material.staticFriction = 0.1;
		newBod.setOrient(1);
		otherBod.setStatic();
		physicsPolygon* pol3 = new physicsPolygon();
		Vector2D vec[3];
		vec[0].set(-30, -30);
		vec[1].set(30,-30);
		vec[2].set(30, 30);
		pol3->set(vec, 3);
		physicsBody a(pol3, 200, 250);
				physicsBody b(pol3, 200, 250);
		physicsBody c(pol3, 200, 250);
		engine.addObject(&body);
		engine.addObject(&otherBod);
		engine.addObject(&newBod);
		engine.addObject(&othernewBod);
		engine.addObject(&a);
		engine.addObject(&b);
		engine.addObject(&c);
		while(!quit)
		{
			SDL_Event e;
			while(SDL_PollEvent(&e))
			{
			}
			SDL_SetRenderDrawColor( mainRenderer, 255, 255, 255, 255 );
			SDL_RenderClear(mainRenderer);
			engine.simulate();
			engine.render();
			SDL_RenderPresent(mainRenderer);
		}
		/*
		editorGUI* editor = new editorGUI();
		CanvasEngine* engine = new CanvasEngine();
		engine->gui = editor;
		engine->initialize();
		while(!quit)
		{
			engine->render();
		}
		*/
	}
	
	/*
	if(Init())
	{
		gObj* firstDiv = new gObj(SCREEN_HEIGHT-400, SCREEN_WIDTH-400, 400, 400);
		firstDiv->setText("hi");
		SDL_Color black = {0 , 0, 0};
		firstDiv->ctextTexture.textColor = black;
		editorGUI* editor = new editorGUI();
		firstDiv->setColor(255, 255, 255, 255);
		editor->initialize();
		while(!quit)
		{
			SDL_Event e;
			while(SDL_PollEvent(&e)>0)
			{
				if(e.type == SDL_QUIT)
				{
					quit = true;
				}
				editor->handleMouse(&e);
			}

			SDL_SetRenderDrawColor(mainRenderer, 40, 40, 40, 255);
			SDL_RenderClear(mainRenderer);
			setFrame();
			std::string text = std::to_string( deltaTime);
			char* alter = const_cast<char*>(text.c_str());
			firstDiv->setText(text);
			char* textc = const_cast<char*>(text.c_str());
			printf(strcat(textc, "\n"));
			firstDiv->play(deltaTime);
			editor->play(deltaTime);
			editor->render(0,0);
			firstDiv->render(0,0);
			SDL_RenderPresent(mainRenderer);
		}
	}
	*/
	close();
	return 1;
}