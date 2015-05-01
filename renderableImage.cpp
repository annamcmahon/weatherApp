
//  renderableImage.cpp
//  
//
//  Created by Anna Jo McMahon on 4/30/15.
//
//

#include "renderableImage.h"
using namespace std;
#include <iostream>
#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sdlClass.h"
//renderableImage::renderableImage()
//{
//}

renderableImage::renderableImage(string imageName,SDL_Renderer* ren ,int width, int height,int xpos, int ypos )
{
	renderer = ren;
	textureImage = imageName;
	rect.w = width;
	rect.h = height;
	rect.x = xpos;
	rect.y = ypos;
	CreaterenderableImage();
	if(renderer == nullptr){
		cout<<"shoot";
	}
        SDL_RenderSetLogicalSize( renderer, 1776,952 );
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        SDL_RenderPresent(renderer);

}

int renderableImage::getWidth()
{
	return width;
}

int renderableImage::getHeight()
{
	return height;
}
int renderableImage::getX()
{
	return xpos;
}
int renderableImage::getY()
{
	return ypos;
}
void renderableImage::CreaterenderableImage() {
	cout<< "imgs/"+textureImage+".bmp"<<endl;
	texture = load_texture(("imgs/"+textureImage+".bmp").c_str());
}

SDL_Texture* renderableImage::getTexture() {
	return texture;
}
SDL_Rect renderableImage::getRect() {
	return rect;
}

SDL_Texture* renderableImage::load_texture(const char* fname) {
     SDL_Surface *image = IMG_Load(fname);
     check_error_sdl_img(image == nullptr, "Unable to load image");
     SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
     check_error_sdl_img(img_texture == nullptr, "Unable to create a texture from the image");
     SDL_FreeSurface(image);
     return img_texture;
}
 // In case of error, print the error code and close the application
void renderableImage::check_error_sdl_img(bool check, const char* message) {
     if (check) {
         std::cout << message << "broken"; //<< IMG_GetError() << std::endl;
//        IMG_Quit();
//      SDL_Quit();
       std::exit(-1);
     }
}

void renderableImage::renderImage(){
//  SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
cout<<"render";
        SDL_Delay(5000);
}
