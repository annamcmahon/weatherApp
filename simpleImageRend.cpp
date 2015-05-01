#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

SDL_Window* window;
        SDL_Renderer* renderer;
        bool InitEverything();
        bool InitSDL();
        bool CreateWindow();
        bool CreateRenderer();
        void SetupRenderer();
	bool setUpImage();
//SDL_Texture *myarray[15];
SDL_Texture *my;
SDL_Surface *surface;
SDL_Rect rect;

int main(){

InitEverything();
for(int i=0;i<3;i++)
{
	SDL_RenderClear(renderer);
   char filename[] = "imgs/rain.bmp";
	rect = {0,0,1776,952};
   filename[0] = i + '0';
   surface = IMG_Load("imgs/rain.bmp");
//   myarray[i] = SDL_CreateTextureFromSurface(renderer,surface);
my = SDL_CreateTextureFromSurface(renderer,surface);
  if ( surface  == nullptr){
                cout<< "surface is null";
        }
        if(renderer == nullptr){
                cout << "texture is null";
        }
    SDL_RenderCopy(renderer, my, nullptr, &rect );
    SDL_RenderPresent(renderer);

	SDL_Delay(500);
//	SDL_FreeSurface(surface);
}

}
bool InitEverything() {
        if ( !InitSDL() )
                return false;
        if ( !CreateWindow() )
                return false;
        if ( !CreateRenderer() )
                return false;
        SetupRenderer();
    //    if ( !SetupTTF( "ttf/ClearSans-Regular.ttf" ) )
      //          return false;
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

bool setUpImage() {
        int flags=IMG_INIT_JPG | IMG_INIT_PNG;
        int initted = IMG_Init(flags);
        if ((initted & flags) != flags) {
                return false;
                cout<<"Failed to initialize image lib "<<IMG_GetError()<<endl;
        }
        return true;
}
