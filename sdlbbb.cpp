#include <iostream>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include <vector>
//#include "sdlClass.h"
#include "renderableImage.h"
//#include <boost/tuple/tuple.hpp>
//#include <boost/iterator/zip_iterator.hpp>
#include <tuple>
using namespace std;
//using boost::tuple;

sdlClass::sdlClass(string filenameStr_, vector<string> temp_, int delay_) {

        temp = temp_;
	SCREENWIDTH = 1776;
	SCREENHEIGHT = 952;	
	InitEverything();		
	SDL_Color textColor = { 255, 255, 255, 255 }; // white
	SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black
	delay = delay_;
		
	filenameStr = filenameStr_;
	SDL_RenderClear(renderer);
	
	CreateImageTextures(filenameStr );
 	 
	CreateTextTextures();
	for(int i = 0; i< textures.size(); i++){
                SDL_RenderCopy( renderer, textures.at(i), nullptr, &(Rects.at(i)));
	}
	SDL_RenderPresent(renderer);

	SDL_Delay(10000);

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
        return true;
}
bool sdlClass::CreateWindow() {
	window = SDL_CreateWindow("Test SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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
        SDL_RenderSetLogicalSize( renderer, SCREENWIDTH, SCREENHEIGHT );
        // Set color of renderer to red
//        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
}
void sdlClass::CreateTextTextures() {
	// temp += "\xB0";
	string varStr = "7";
	
	//string varStr = "\xB0"; // degree symbol
	varStr =temp.at(0) + "\xB0";
	  SDL_Surface* surf = TTF_RenderText_Solid( font, varStr.c_str(), textColor );
        textTexture = SurfaceToTexture( surf );
        SDL_QueryTexture( textTexture, NULL, NULL, &textRect.w, &textRect.h );
         textRect.w  =textRect.w*3;
        textRect.h =textRect.h*3;
	textRect.x = SCREENWIDTH/2 - (textRect.w)/2;
        textRect.y = SCREENHEIGHT/2 - (textRect.h)/2;
	 textures.push_back(textTexture);
        Rects.push_back(textRect);
	
	int w, h;
        varStr = "high:" + temp.at(1);
	 surf = TTF_RenderText_Solid( font, varStr.c_str(), textColor );
        textTexture = SurfaceToTexture( surf );
        SDL_QueryTexture( textTexture, NULL, NULL, &w, &h );
	 textRect.w = textRect.w/5;
        textRect.h = textRect.w/5;
        textRect.y = SCREENHEIGHT/2 + 100;
	   textRect.x = SCREENWIDTH/2;
	

	 textures.push_back(textTexture);
        Rects.push_back(textRect);

	 varStr = "low:" + temp.at(2);
         surf = TTF_RenderText_Solid( font, varStr.c_str(), textColor );
        textTexture = SurfaceToTexture( surf );
        SDL_QueryTexture( textTexture, NULL, NULL, &textRect.w, &textRect.h );
        textRect.x = SCREENWIDTH/2;
        textRect.y = SCREENHEIGHT/2 + 200;
	
	textures.push_back(textTexture);
        Rects.push_back(textRect);

}
SDL_Texture* sdlClass::SurfaceToTexture( SDL_Surface* surf ) {
        SDL_Texture* text;
        text = SDL_CreateTextureFromSurface( renderer, surf );
        SDL_FreeSurface( surf );
        return text;
}                  
void sdlClass::CreateImageTextures(string filenameStr) {
     imageTexture = load_texture(("imgs/weatherBackground/"+filenameStr).c_str(), renderer); //CHANGED "img_test.png"
     // We need to create a destination rectangle for the image (where we want this to be show) on the renderer area
     imageDestRect.x = 0; imageDestRect.y = 0;
     imageDestRect.w = SCREENWIDTH; imageDestRect.h = SCREENHEIGHT;

	 imageTexture2 = load_texture(("imgs/weatherDog/"+filenameStr).c_str(), renderer); //CHANGED "img_test.png"
	imageDestRect2.x =SCREENWIDTH -SCREENWIDTH/7 ; imageDestRect2.y = 0;
    imageDestRect2.w = SCREENWIDTH/6; imageDestRect2.h = SCREENWIDTH/6;

	 imageTexture3 = load_texture(("imgs/weatherIcon/"+filenameStr).c_str(), renderer); //CHANGED "img_test.png"
     imageDestRect3.x = 20; imageDestRect3.y = 20;
     imageDestRect3.w = SCREENWIDTH/15; imageDestRect3.h = SCREENWIDTH/15;
	textures.push_back(imageTexture);  textures.push_back(imageTexture2);  textures.push_back(imageTexture3);
	Rects.push_back(imageDestRect);Rects.push_back(imageDestRect2);Rects.push_back(imageDestRect3);

}
