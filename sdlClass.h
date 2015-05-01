#ifndef ____sdlClass__
#define ____sdlClass__
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include <vector>
#include "renderableImage.h"
#include <tuple>
#include <boost/tuple/tuple.hpp>
using namespace std;
class sdlClass {
public:
	sdlClass(string,vector<string> ,int);
private:
        vector<string> moreWeatherDetails;

	int delay;
	string filenameStr;

	string filenameStr2;
	vector<string> temp;
	int* screenheight;
	int* screenwidth;	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Color textColor; 
	SDL_Color backgroundColor; 

	
	SDL_Texture* textTexture;
	SDL_Rect textRect;
	vector<SDL_Texture*> textures;
	vector<SDL_Rect> Rects;
//        textTextures.push_back(solidTexture);
  //      textTextures.push_back(textTexture);

	SDL_Rect imageDestRect;
	TTF_Font* font;
	
	SDL_Texture *imageTexture;
 	SDL_Rect imageDestRect2;
        SDL_Texture *imageTexture2;
	 SDL_Rect imageDestRect3;
        SDL_Texture *imageTexture3;
	void sdlStuff(string, int);
	//set up/ initialize SDL things 
	int SCREENWIDTH;
	int SCREENHEIGHT;
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
	void CreateImageTextures(string);
	SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
	void check_error_sdl_img(bool check, const char* message);
};
#endif
