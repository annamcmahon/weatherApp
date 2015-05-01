#ifndef ____sdlClass__
#define ____sdlClass__
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include "renderableImage.h"
#include <vector>
using namespace std;
class sdlClass {
public:
	sdlClass(string,vector<string> ,int);
private:
	int delay;
	string filenameStr;
	string filenameStr2;
	vector<string> temp;
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Color textColor; 
	SDL_Color backgroundColor; 
	SDL_Texture* solidTexture;
	SDL_Rect solidRect;
	SDL_Rect imageDestRect;
	TTF_Font* font;
//	renderableImage backgroundImage("rain",0,0,1776,925);
	
	SDL_Texture *imageTexture;
 	SDL_Rect imageDestRect2;
        SDL_Texture *imageTexture2;
	void sdlStuff(string, int);
	//set up/ initialize SDL things 

	bool InitEverything();
	bool InitSDL();
	bool CreateWindow();
	bool CreateRenderer();
	void SetupRenderer();
	bool SetupTTF( const std::string &fontName);
	bool setUpImage();
	//other SDL funcitons
	SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);// Load an image from "fname" and return an SDL_Texture with the content of the image
	void CreateTextTextures();
	void CreateImageTextures(string, string);
	SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
	void check_error_sdl_img(bool check, const char* message);
};
#endif
