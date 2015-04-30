#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include "WeatherApiClass.h"
#include "sdlClass.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

using namespace std;
   int delay;
        string filenameStr;
        string temp;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Color textColor;
        SDL_Color backgroundColor;
        SDL_Texture* solidTexture;
        SDL_Rect solidRect;
        SDL_Rect imageDestRect;
        TTF_Font* font;
        SDL_Texture *imageTexture;
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
        void CreateImageTextures(string);
        SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
        void check_error_sdl_img(bool check, const char* message);




int main(int argc, char** argv) {
	string temp; // temperature
	int weatherConditionCode; // code used by yahoo weather api
	WeatherApiClass weatherApi; // declaring object
	weatherConditionCode = weatherApi.getCode(); // returns int code
	temp = weatherApi.getTemp(); // returns string with temperature
	weatherApi.commandLinePrint(); // prints to cout
	string pictureFilenameString =  weatherApi.getPicFilename(); // gets filename excluding extension (relative path as well)
	int delay = 5000; // 5 seconds
	InitEverything();
	SDL_Color textColor = { 255, 255, 255, 255 }; // white
	SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black
//	delay = delay_;
//	temp = temp_;
//	filenameStr = filenameStr_;
	SDL_StartTextInput();
	bool quit = 0;
	while (!quit){
		SDL_Event e;
		while (SDL_PollEvent( &e ) != 0){
			if (e.type == SDL_QUIT){
			                      quit = 1;
			              }
			              if (e.type == SDL_KEYDOWN){
			                      quit = 1;
			              }
			              if (e.type == SDL_MOUSEBUTTONDOWN){
			                      quit = 1;
			              }
			      }
			      cout<< pictureFilenameString<< "|";
			
//			Render the scene
			      SDL_RenderClear(renderer);
			      CreateImageTextures(pictureFilenameString );
			      SDL_RenderCopy(renderer, imageTexture, NULL, &imageDestRect);
			
			      CreateTextTextures();
			      SDL_RenderCopy( renderer, solidTexture, nullptr, &solidRect );
//			      SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
			      SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(imageTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

	
	// initialize SDL2_TTF
	bool SetupTTF( const std::string &fontName) {
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
	bool setUpImage() {
		int flags=IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init(flags);
		if ((initted & flags) != flags) {
			return false;
			cout<<"Failed to initialize image lib "<<IMG_GetError()<<endl;
		}
		return true;
	}
 // In case of error, print the error code and close the application
	void check_error_sdl_img(bool check, const char* message) {
		if (check) {
			std::cout << message << " " << IMG_GetError() << std::endl;
			IMG_Quit();
			SDL_Quit();
			std::exit(-1);
		}
	}
 // Load an image from "fname" and return an SDL_Texture with the content of the image
	SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer) {
		SDL_Surface *image = IMG_Load(fname);
		check_error_sdl_img(image == nullptr, "Unable to load image");
		
		SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
		check_error_sdl_img(img_texture == nullptr, "Unable to create a texture from the image");
		SDL_FreeSurface(image);
		return img_texture;
	}
	
	
	bool InitEverything() {
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
	bool InitSDL() {
		if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
		{
			std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
	}
	bool CreateWindow() {
		window = SDL_CreateWindow("Test SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1776,952, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		if ( window == nullptr )
		{
			std::cout << "Failed to create window : " << SDL_GetError();
			return false;
		}
		return true;
	}
	bool CreateRenderer() {
		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		if ( renderer == nullptr ) {
			std::cout << "Failed to create renderer : " << SDL_GetError();
			return false;
		}
		return true;
	}
	void SetupRenderer() {
		// Set size of renderer to the same as window
		SDL_RenderSetLogicalSize( renderer, 1776,952 );
		// Set color of renderer to red
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
	}
	void CreateTextTextures() {
		// temp += "\xB0";
		string varStr = "7";
		//string varStr = "\xB0"; // degree symbol
		varStr = temp + "\xB0";
		SDL_Surface* solid = TTF_RenderText_Solid( font, varStr.c_str(), textColor );
		solidTexture = SurfaceToTexture( solid );
		
		SDL_QueryTexture( solidTexture, NULL, NULL, &solidRect.w, &solidRect.h );
		solidRect.x = 400;
		solidRect.y = 0;
	}
	SDL_Texture* SurfaceToTexture( SDL_Surface* surf ) {
		SDL_Texture* text;
		text = SDL_CreateTextureFromSurface( renderer, surf );
		SDL_FreeSurface( surf );
		return text;
	}
	void CreateImageTextures(string filenameStr) {
		imageTexture = load_texture(("imgs/"+filenameStr+".bmp").c_str(), renderer); //CHANGED "img_test.png"
		// We need to create a destination rectangle for the image (where we want this to be show) on the renderer area
		imageDestRect.x = 0; imageDestRect.y = 0;
		imageDestRect.w = 1776; imageDestRect.h = 952;
	}

