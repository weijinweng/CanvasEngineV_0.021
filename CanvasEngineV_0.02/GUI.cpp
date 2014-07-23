#include "GUI.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

extern SDL_Renderer* mainRenderer;
extern TTF_Font* CanvasFont;
extern SDL_Window* mainWindow;

CanvasTexture::CanvasTexture()
{
	texture = NULL;
	pixel = NULL;
	textColor;
}

CanvasTexture::CanvasTexture(std::string img)
{
	texture = NULL;
	pixel = NULL;
	loadImage(img);
}

bool CanvasTexture::lockTexture()
{
	if( pixel!=NULL)
	{
		return false;
	}else {
		if(SDL_LockTexture(texture, NULL, &pixel, &pitch)!=0)
		{
			printf("error locking %s\n", SDL_GetError());
			return false;
		}
		return true;
	}
}

bool CanvasTexture::unlockTexture()
{
	if( pixel = NULL)
	{
		return true;
	} else {
		SDL_UnlockTexture(texture);
		pixel = NULL;
		pitch = 0;
		return true;
	}
}

bool CanvasTexture::loadImage(std::string img)
{
	SDL_DestroyTexture(texture);

	texture = NULL;

	SDL_Surface* imgSurface = IMG_Load(img.c_str());
	if(imgSurface == NULL)
	{
		printf("IMG_load failed %s\n", IMG_GetError());
	}else {
		SDL_Surface* optSurface = SDL_ConvertSurfaceFormat(imgSurface, SDL_PIXELFORMAT_RGBA8888, NULL);
		if(optSurface == NULL)
		{
			printf("Unable to convert surface! %s\n", SDL_GetError());
		} else {
			texture = SDL_CreateTexture(mainRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, optSurface->w, optSurface->h);
			if(texture == NULL)
			{
				printf("Unable to create texture %s\n", SDL_GetError());
			} else {
                SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
				SDL_LockTexture(texture, &optSurface->clip_rect, &pixel, &pitch);
				memcpy(pixel, optSurface->pixels, optSurface->pitch*optSurface->h);
				SDL_UnlockTexture(texture);
				pixel = NULL;
				
				width = optSurface->w;
				height = optSurface->h;
			}
		}
		SDL_FreeSurface(optSurface);
	}
	SDL_FreeSurface(imgSurface);
	return texture != NULL;
}

bool CanvasTexture::loadText(std::string text)
{

	SDL_Surface* textSurface = TTF_RenderText_Solid(CanvasFont, text.c_str(), textColor);
	if(textSurface == NULL)
	{
		printf("TT_RenderText_Blended error %s\n", TTF_GetError());
		return false;
	} else {
		if(texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
		texture = SDL_CreateTextureFromSurface(mainRenderer, textSurface);
		if(texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface error %s\n", SDL_GetError());
		}
		else {
			height = textSurface->h;
			width = textSurface->w;
		}
	}
	SDL_FreeSurface(textSurface);
	return texture != NULL;
}


bool CanvasTexture::colorKey(SDL_Color key, SDL_Color to)
{
	if(texture == NULL)
	{
		printf("happening\n");
		return false;
	} else {
        SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
		SDL_Surface* optSurface = SDL_ConvertSurfaceFormat(SDL_GetWindowSurface(mainWindow),SDL_PIXELFORMAT_RGBA8888,NULL);
		if(lockTexture())
		{
			printf("happening2\n");
			Uint32* cPixels = (Uint32*)pixel;
			int pixelCount = pitch/4*height;

			Uint32 colorK = SDL_MapRGB(optSurface->format, key.r, key.g, key.b);
			Uint32 colorT = SDL_MapRGB(optSurface->format, to.r, to.g, to.b);
			for(int i = 0; i < pixelCount; ++i)
			{

				if(cPixels[i] == colorK)
				{
					cPixels[i] = colorT;
				}
			}
		}

		unlockTexture();
	}
	return true;
}

void renderData::render()
{
	SDL_RenderCopy(mainRenderer, texture.texture, NULL, &obj);
}

intAnimation::intAnimation(int* value, int destination, int time)
{
	v = value;
	this->coordinate = destination;
	this->time = time;
	tpassed = 0;
	origin = *value;
}

bool intAnimation::play(int timepassed)
{
	if(!time)
		return false;
	if(tpassed == 0)
	{
		origin = *v;
	}
	tpassed += timepassed;
	double timeFraction =((double)tpassed/(double)time);
	*v = origin +(timeFraction*(coordinate - origin));
	if(tpassed >= time)
		time = 0;
	return true;
}

Cint::Cint()
{

}

Cint::Cint(int value)
{
	v = value;
}

void Cint::animate(int value, int time)
{
	intAnimation newAnimation(&this->v, value, time);
	animations.push_back(newAnimation);
}

void Cint::delay(int time)
{
	intAnimation newAnimation(&this->v, 0, time);
	animations.push_back(newAnimation);
}

void Cint::play(int timepassed)
{
	if(animations.size()!= 0)
	{
		if(!animations.front().play(timepassed))
			animations.pop_front();
	}
}

bool Cint::operator=(int other)
{
	this->v = other;
	return true;
}

gObj::gObj(int top, int left, int width, int height)
{
	this->top = top;
	this->left = left;
	this->width = width;
	this->height = height;
	this->setColor(255,255,255,0);
	text = "";
	id = "";
	Class = "joe";
	opacity.v = 0;
	fontHeight = 40;
	textOrientation = CENTERED_TEXT;
	show = true;
}

void gObj::pollBuffer()
{
	events = eventBuffer;
	eventBuffer = 0;
}

void gObj::handleMouse(SDL_Event* e)
{
	if(show)
	{
		if((*e).type == SDL_MOUSEBUTTONDOWN||(*e).type == SDL_MOUSEBUTTONUP || (*e).type == SDL_MOUSEMOTION)
		{

			int x, y;
			SDL_GetMouseState(&x, &y);
			if((*e).type == SDL_MOUSEBUTTONDOWN)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{

					if(x > (left.v+width.v)
						|| x < left.v
						|| y > (top.v + height.v)
						|| y < top.v)
					{

						offHover();
						hover = false;
					} else
					{
						onMouseDown();
						mousedown = true;
					}
				}

			}
			if((*e).type == SDL_MOUSEBUTTONUP)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v)
						|| x < left.v
						|| y > (top.v + height.v)
						|| y < top.v)
					{
						offHover();
						hover = false;
					} else 
					{
						onMouseUp();
						mousedown = false;
					}
				}
			}
			if((*e).type == SDL_MOUSEMOTION)
			{
				if(x > (left.v+width.v)
					|| x < left.v
					|| y > (top.v + height.v)
					|| y < top.v)
				{
					offHover();
					hover = true;
				}else 
				{
					onHover();
					hover = false;
				}
			}
		}
	}

}

void gObj::childHandleMouse(int ix, int iy, SDL_Event* e)
{
	if(show)
	{
		if((*e).type == SDL_MOUSEBUTTONDOWN||(*e).type == SDL_MOUSEBUTTONUP || (*e).type == SDL_MOUSEMOTION)
		{

			int x, y;
			SDL_GetMouseState(&x, &y);
			if((*e).type == SDL_MOUSEBUTTONDOWN)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v + ix)
						|| x < left.v + ix
						|| y > (top.v + height.v + iy)
						|| y < top.v + iy)
					{

						offHover();
						hover = false;
					} else
					{
						onMouseDown();
						mousedown = true;
					}
				}

			}
			if((*e).type == SDL_MOUSEBUTTONUP)
			{
				if((*e).button.button == SDL_BUTTON_LEFT)
				{
					if(x > (left.v+width.v + ix)
						|| x < left.v + ix
						|| y > (top.v + height.v + iy)
						|| y < top.v + iy)
					{
						offHover();
						hover = false;
					} else 
					{
						onMouseUp();
						mousedown = false;
					}
				}
			}
			if((*e).type == SDL_MOUSEMOTION)
			{
				if(x > (left.v+width.v + ix)
					|| x < left.v + ix
					|| y > (top.v + height.v + iy)
					|| y < top.v + iy)
				{
					offHover();
					hover = false;
				}else 
				{
					onHover();
					hover = true;
				}
			}
		}

	}
}

void gObj::play(int timepassed)
{
	if(events & MOUSEDOWN)
	{
		onMouseDown();
	}
	if(events & MOUSEUP)
	{
		onMouseUp();
	}
	if(events & OFFHOVER)
	{
		offHover();
	}
	if(events & ONHOVER)
	{
		onHover();
	}
}

bool gObj::setText(std::string text)
{
	ctextTexture.texture.loadText(text);
	this->text = text;
	return true;
}

bool gObj::setImage(std::string img)
{
	backgroundTexture.texture.loadImage(img);
	return true;
}

void gObj::setColor(int r, int g, int b, int opacity)
{
	backgroundColor.r = r;
	backgroundColor.g = g;
	backgroundColor.b = b;
	this->opacity.v = opacity;
	backgroundColor.a = opacity;
}

void gObj::onMouseDown()
{
}

void gObj::onMouseUp()
{
}

void gObj::onHover()
{
}

void gObj::offHover()
{
}



gObj* gObj::animate(int variable, int destination, int time)
{
	switch(variable)
	{
	case TOP:
		{
			top.animate(destination, time);
		}
		break;
	case LEFT:
		{
			left.animate(destination, time);
		}
		break;
	case WIDTH:
		{
			width.animate(destination, time);
		}
		break;
	case HEIGHT:
		{
			height.animate(destination, time);
		}
		break;
	case OPACITY:
		{
			opacity.animate(destination, time);
		}
		break;
	}
	return this;
}

void gObj::gatherRenderData()
{
	backgroundTexture.obj.x = left.v;
	backgroundTexture.obj.y = top.v;
	backgroundTexture.obj.w = width.v;
	backgroundTexture.obj.h = height.v;

	if(text != "")
	{
		ctextTexture.obj.h = fontHeight;
		ctextTexture.obj.w = ((double)ctextTexture.texture.width)*((double)fontHeight/(double)ctextTexture.texture.height);
		ctextTexture.obj.y = top.v;
		if(textOrientation == RIGHT_TEXT)
		{
			ctextTexture.obj.x = height.v - ctextTexture.obj.w + left.v;
		} else if (textOrientation == CENTERED_TEXT)
		{
			ctextTexture.obj.x = left.v + ((double)(width.v - ctextTexture.obj.w))/2;
		} else {
			ctextTexture.obj.x = left.v;
		}
	}

}

void gObj::render(std::list<renderData>* data)
{
	data->push_back(backgroundTexture);
	data->push_back(ctextTexture);
	/*
	if(show)
	{
		renderRect.x = left.v + x;
		renderRect.y = top.v + y;
		renderRect.w = width.v;
		renderRect.h = height.v;
		if(backgroundTexture.texture == NULL)
		{
			if(backgroundColor.a != 0)
			{
				SDL_SetRenderDrawColor(mainRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
				SDL_RenderFillRect(mainRenderer, &renderRect);
			}
		} else {
			SDL_RenderCopy(mainRenderer, backgroundTexture.texture, NULL, &renderRect);
		}
		if(ctextTexture.texture != NULL)
		{
			renderRect.h = fontHeight;
			renderRect.w = ((double)ctextTexture.width)*((double)fontHeight/(double)ctextTexture.height);
			if(textOrientation == RIGHT_TEXT)
			{
				renderRect.x = height.v - renderRect.w + left.v;
			} else if (textOrientation == CENTERED_TEXT)
			{
				renderRect.x = left.v + ((double)(width.v - renderRect.w))/2;
			} else {
				renderRect.x = left.v;
			}
			renderRect.x += x;
			SDL_RenderCopy(mainRenderer, ctextTexture.texture, NULL, &renderRect);
		}
	}
	*/
}

gObj::~gObj()
{
}