#include <SDL2/SDL.h>
//#include <SDL_ttf.h>
#include <SDL2/SDL_ttf.h>
//g++ -std=c++0x textRenderTest.cpp -o textRenderTest `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf
#include <iostream>
using namespace std;
int main(int argc, char ** argv)
{
 bool quit = false;
 SDL_Event event;

 SDL_Init(SDL_INIT_VIDEO);
 TTF_Init();

cout<<" here"<<endl;
 SDL_Window *window = SDL_CreateWindow("SDL_ttf in SDL2",
  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
  480, 0);
 SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
cout<<" here23"<<endl;

 TTF_Font *font = TTF_OpenFont("arial.ttf", 25);
cout<<" here23"<<endl;
 
const char * error = TTF_GetError();
cout<<" here23"<<endl;

 SDL_Color color = { 255, 255, 255 };
cout<<" here23"<<endl;
SDL_Surface *surface;
if(!(surface=TTF_RenderText_Solid(font,"Hello World!",color))) {
cout<< error;
} 

// SDL_Surface *surface = TTF_RenderText_Solid(font,
  //text , color );
cout<<" here23"<<endl;
 
SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
  surface);
cout<<" here 2"<<endl;
 int texW = 0;
 int texH = 0;
 SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
 SDL_Rect dstrect = { 0, 0, texW, texH };

 while (!quit)
 {
  SDL_WaitEvent(&event);

  switch (event.type)
  {
  case SDL_QUIT:
   quit = true;
   break;
  }
cout<<" here3"<<endl;
  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  SDL_RenderPresent(renderer);
 }

 SDL_DestroyTexture(texture);
 SDL_FreeSurface(surface);
 TTF_CloseFont(font);

 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
 TTF_Quit();
 SDL_Quit();

 return 0;
}
