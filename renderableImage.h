//
//  renderableImage.h
//  
//
//  Created by Anna Jo McMahon on 4/30/15.
//
//

#ifndef ____renderableImage__
#define ____renderableImage__
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "sdlClass.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>
using namespace std;
class renderableImage
{
public:
//	renderableImage();
	renderableImage(string,SDL_Renderer* , int, int, int, int);
	int getWidth();
	int getHeight();
	int getX();
	int getY();
	SDL_Texture* getTexture();
	SDL_Rect getRect();
	void CreaterenderableImage();
	void check_error_sdl_img(bool, const char*);
	SDL_Texture* load_texture(const char* );
	void renderImage();
private:
	SDL_Texture* texture;
	SDL_Rect rect;
	string textureImage;
	int width, height,xpos, ypos;
	SDL_Renderer* renderer;
};
#endif /* defined(____renderableImage__) */


