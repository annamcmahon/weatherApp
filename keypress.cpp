#include <iostream>
#include <SDL2/SDL.h>
//#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
using namespace std;
SDL_Event input;


int main(int argc, const char * argv[])
{
SDL_Init(SDL_INIT_EVERYTHING);


int y = 0;
int z = 0;
int w = 0;
bool key = false;

const Uint8 *state = SDL_GetKeyboardState(NULL);


while (1){
   w = 0;
   SDL_PollEvent(&input);

  //check for events generated

   switch (input.type) {
       case SDL_KEYDOWN:  
           key = true;
           break;
           case SDL_KEYUP:
           key = false;
           break;

       default:
           break;
   }



  // Check for no. of keys pressed using ASCII code

    for (y =0 ;y<=127;y++)
        if(state[y] == 1)
            w++;


   // Display the messages

   if (key && z==0 )
   {
       cout << "Key is Pressed" << endl;
       z = 1;
   }
   else if (!key && w < 1 && z==1)
   {
       cout << "Key is released" << endl;
       z = 0;

   }



}

return 0;
 }
