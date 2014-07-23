#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>



class CanvasTexture{
public:
	//texture
	SDL_Texture* texture;
	//pixel data
	void* pixel;
	//the pitch of the texture
	int pitch;
	//the width and height of the texture
	int width;
	int height;
	//Color of the text during load text
	SDL_Color textColor;
	//constructor, can take a string to preload image
	CanvasTexture();
	CanvasTexture(std::string img);
	//lock the texture for writing
	bool lockTexture();
	//unlock the texture for reading
	bool unlockTexture();
	//load a image as texture
	bool loadImage(std::string img);
	//load a text as a non streamable text data
	bool loadText(std::string text);
	//colorkey
	bool colorKey(SDL_Color key, SDL_Color to);
};

class renderData{
public:
	//texture
	CanvasTexture texture;
	//clip from texture
	SDL_Rect clip;
	//renderCopy destination
	SDL_Rect obj;
	renderData(){
		obj.w = 0;
	}
	//renders to mainRenderer
	void render();
};

enum DIV_VARIABLES
{
	TOP,
	LEFT,
	WIDTH,
	HEIGHT,
	OPACITY,
	RELATIVE,
	ABSOLUTE,
	FIXED,
	RELATIVE_LEFT
};

enum TEXT_TYPE
{
	CENTERED_TEXT,
	RIGHT_TEXT,
	LEFT_TEXT
};
//Animates a int value based on destination value and time.
class intAnimation
{
public:
	intAnimation(int* edit, int coordinate, int time);
	int coordinate;
	int origin;
	int* v;
	int time;
	int tpassed;
	bool play(int timepassed);
};

//CanvasInt, used for int values that are animatable.
class Cint{
public:
	int v;
	Cint();
	Cint(int value);
	void play(int timpassed);
	void animate(int value, int time);
	void delay(int time);
	bool operator= (int other);
	std::list<intAnimation> animations;
};



class gObj
{
public:
	enum 
	{
		MOUSEDOWN = 1 << 0,
		MOUSEUP = 1 << 1,
		ONHOVER = 1 << 2,
		OFFHOVER = 1 << 3
	};
	SDL_Rect renderRect;
	bool update;
	gObj(int top, int left, int width, int height);
	~gObj();
	bool setImage(std::string img);
	void setColor(int r, int g, int b, int a);
	bool setText(std::string text);
	//pushes events into eventBuffer
	virtual void handleMouse(SDL_Event* e);
	//handle mouse as child
	virtual void childHandleMouse(int x, int y, SDL_Event* e);
	virtual void onMouseDown();
	virtual void onMouseUp();
	virtual void onHover();
	virtual void offHover();
	void play(int timepassed);
	void pollBuffer();
	gObj* animate(int variable, int destination, int timepassed);
	virtual void gatherRenderData();
	virtual void render(std::list<renderData>* data);
	renderData backgroundTexture;
	SDL_Color backgroundColor;
	renderData ctextTexture;
	int fontHeight;
	int textHeight;
	int textWidth;
	int textOrientation;
	//Stores the position type of the object
	int position;
	//Stores the events to be polled by play
	int eventBuffer;
	int events;
	//opacity, currently not needed
	Cint opacity;
	//current text value of the object.
	std::string text;
	//id of the object, should be distinct
	std::string id;
	//class of the object, for objects that would have multiple version with similiar functions.
	std::string Class;
	//parameters
	Cint top, left, width, height;
	bool mousedown;
	bool hover;
	bool show;
};


#endif