#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
//#include "sdlClass.h"
#include "renderableImage.h"
using namespace std;

sdlClass::sdlClass(string filenameStr_, vector<string> temp_, int delay_) {
	temp = temp_;
	InitEverything();
	SDL_Color textColor = { 255, 255, 255, 255 }; // white
	SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black
	delay = delay_;
	
	filenameStr2 = "cloudy";
	filenameStr = filenameStr_;
	SDL_RenderClear(renderer);

	CreateImageTextures(filenameStr);
//	SDL_RenderCopy(renderer, imageTexture, NULL, &imageDestRect);
	cout<< "img 1"<< endl;
	SDL_RenderCopy(renderer, imageTexture2, NULL, &imageDestRect2);
	cout<< "img 2"<< endl;
	SDL_RenderCopy(renderer, imageTexture3, NULL, &imageDestRect3);
	cout<< "img 3"<< endl;
	CreateTextTextures();
	SDL_RenderCopy( renderer, solidTexture, nullptr, &solidRect );

	SDL_RenderPresent(renderer);

	SDL_Delay(delay);

	// Clear the allocated resources
	SDL_DestroyTexture(imageTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// initialize SDL2_TTF
bool sdlClass::SetupTTF( const std::string &fontName) {
	if ( TTF_Init() == -1 ) {
		std::cout << " Failed to initialize TTF : " << TTF_GetError() << std::endl;
		return false;
	}
	// Load fonts at a large size
	font = TTF_OpenFont( fontName.c_str(), 90 );
	// Error check
	if ( font == nullptr ) {
		std::cout << " Failed to load font : " << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}



bool sdlClass::setUpImage() {
	int flags=IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		return false;
		cout<<"Failed to initialize image lib "<<IMG_GetError()<<endl;
	}
	return true;
}
// In case of error, print the error code and close the application
void sdlClass::check_error_sdl_img(bool check, const char* message) {
	if (check) {
		std::cout << message << " " << IMG_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		std::exit(-1);
	}
}
// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* sdlClass::load_texture(const char* fname, SDL_Renderer *renderer) {
	SDL_Surface *image = IMG_Load(fname);
	check_error_sdl_img(image == nullptr, "Unable to load image");
	
	SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
	check_error_sdl_img(img_texture == nullptr, "Unable to create a texture from the image");
	SDL_FreeSurface(image);
	return img_texture;
}
bool sdlClass::InitEverything() {
	if ( !InitSDL() )
		return false;
	if ( !CreateWindow() )
		return false;
	if ( !CreateRenderer() )
		return false;
	SetupRenderer();
	if ( !SetupTTF( "ttf/ClearSans-Regular.ttf" ) )
		return false;
	if (!setUpImage())
		return false;
	//       CreateTextTextures();
	
	return true;
}
bool sdlClass::InitSDL() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}
}

bool sdlClass::CreateWindow() {
	window = SDL_CreateWindow("Test SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1776,952, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if ( window == nullptr )
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}
	return true;
}
bool sdlClass::CreateRenderer() {
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if ( renderer == nullptr ) {
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}
	return true;
}
void sdlClass::SetupRenderer() {
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, 1776,952 );
	// Set color of renderer to red
	//        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
}
void sdlClass::CreateTextTextures() {
	// temp += "\xB0";
	string varStr = "7";
	//string varStr = "\xB0"; // degree symbol
	cout << "temp sdlclass"<<endl;
	varStr = temp.at(0) + "\xB0";
	SDL_Surface* solid = TTF_RenderText_Solid( font, varStr.c_str(), textColor );
	solidTexture = SurfaceToTexture( solid );
	
	SDL_QueryTexture( solidTexture, NULL, NULL, &solidRect.w, &solidRect.h );
	solidRect.x = 400;
	solidRect.y = 0;
}
SDL_Texture* sdlClass::SurfaceToTexture( SDL_Surface* surf ) {
	SDL_Texture* text;
	text = SDL_CreateTextureFromSurface( renderer, surf );
	SDL_FreeSurface( surf );
	return text;
}
void sdlClass::CreateImageTextures(string filenameStr) {
	imageTexture = load_texture(("imgs/weatherBackground/"+filenameStr).c_str(), renderer);
	imageDestRect.x = 0; imageDestRect.y = 0;
	imageDestRect.w = 1776; imageDestRect.h = 952;
 	imageTexture2 = load_texture(("imgs/weatherDog/"+filenameStr).c_str(), renderer);
	imageDestRect2.x = 0; imageDestRect2.y = 0;
	imageDestRect2.w = 200; imageDestRect2.h = 200;
	imageTexture3 = load_texture(("imgs/weatherIcon/"+filenameStr).c_str(), renderer);
	imageDestRect3.x = 400; imageDestRect3.y = 0;
	imageDestRect3.w = 200; imageDestRect3.h = 200;
}
