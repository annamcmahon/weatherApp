#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
using boost::property_tree::ptree;
using namespace std;

string weatherDataFile;//stores the name of the file the weahter data is queried into
string weatherDataFileExtended;
string woeid; // this is the location
string code; // this is the weather code gotten from the API, used to determine image display
ptree pt;   // "ptree" or property tree is a struct defined by boosts' lib

void queryWeatherApiForSimpleStats();
void queryWeatherApiForExtendedStats();
int getWeatherConditionCode();
string findDataMemberWithFullPathString(const ptree&, string);
void fillMaps(map<int,string>& , map<int,string>&);
bool load_files();
void commandLinePrint();
void apiStuff(int&);
void sdlStuff(string, int);

//set up/ initialize SDL things 
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();
bool SetupTTF( const std::string &fontName);
bool setUpImage();

SDL_Rect windowRect = { 900, 300, 400, 400 };// not used, possibly change/ set this to be adaptive
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color textColor = { 255, 255, 255, 255 }; // white
SDL_Color backgroundColor = { 0, 0, 0, 255 }; // black
SDL_Texture* solidTexture;
SDL_Rect solidRect;
SDL_Rect imageDestRect;
TTF_Font* font;
SDL_Texture *imageTexture;

//other SDL funcitons
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);// Load an image from "fname" and return an SDL_Texture with the content of the image
void CreateTextTextures();
void CreateImageTextures(string);
SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
void check_error_sdl_img(bool check, const char* message);

string textDescription, date, high, low; // all the variables to parse for
string temp;

int main(int argc, char** argv) {
	if ( !InitEverything() ) {
		return -1;
	}
	int weatherConditionCode;

	apiStuff(weatherConditionCode);	
	commandLinePrint();

	map<int,string> weatherCodeMap;
	map<int,string> weatherCodeMapReduced;
	fillMaps(weatherCodeMap, weatherCodeMapReduced);
	string filenameStr = weatherCodeMapReduced[weatherConditionCode];
	int delay = 5000;
	sdlStuff(filenameStr, delay);
	return 0;
}
void sdlStuff(string filenameStr, int delay) {

	SDL_RenderClear(renderer);
	CreateImageTextures(filenameStr );
	SDL_RenderCopy(renderer, imageTexture, NULL, &imageDestRect);

	CreateTextTextures();
	SDL_RenderCopy( renderer, solidTexture, nullptr, &solidRect );
//	SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
	SDL_RenderPresent(renderer);

	SDL_Delay(delay);

     // Clear the allocated resources
	SDL_DestroyTexture(imageTexture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void apiStuff(int& weatherConditionCode) {
	queryWeatherApiForSimpleStats();
	read_json(weatherDataFile, pt);// make the property tree from the weatherData file
	weatherConditionCode = getWeatherConditionCode();
	// query for the current conditions
	temp = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.temp");
	textDescription = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.text");
	code = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.code");
	queryWeatherApiForExtendedStats();
	read_json(weatherDataFileExtended, pt);
	date = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..date");
	high = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..high");
	low = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..low");
}
void commandLinePrint() {
	cout<< "the current condition code is: "<< code<< endl;
	cout<<"the current condition text is: "<< textDescription<< endl;
	cout<<"the current temperature is by return is: "<< temp<< endl;
	cout<<"the date : "<< date<< endl;
	cout<<"the high today: "<< high<< endl;
	cout<<"the low today: "<< low<< endl;
	cerr << endl; // cerr is like cout but error checking(?), outputs immediately
}
int getWeatherConditionCode() {
	string stringCode = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.code");
	int code = atoi(stringCode.c_str());//convert string to interger
	return code;
}
void queryWeatherApiForSimpleStats() {
        woeid = "91418771"; // this is the location
        system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select item.condition from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > weatherDataFile").c_str());
        weatherDataFile = "weatherDataFile";
}
void queryWeatherApiForExtendedStats() {
	 woeid = "91418771"; // this is the location
         system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select * from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > weatherDataFileExtended").c_str());
	weatherDataFileExtended = "weatherDataFileExtended";
}
string findDataMemberWithFullPathString(const ptree& pt, string key) {
        return pt.get<string>(key);
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
void fillMaps(map<int,string>& weatherCodeMap, map<int,string>& weatherCodeMapReduced) {
	weatherCodeMap.insert( pair<int,string>(0, "tornado") );
	weatherCodeMap.insert( pair<int,string>(1, "tropical storm") );
	weatherCodeMap.insert( pair<int,string>(2, "hurricane") );
	weatherCodeMap.insert( pair<int,string>(3, "severe thunderstorms") );
	weatherCodeMap.insert( pair<int,string>(4, "thunderstorms") );
	weatherCodeMap.insert( pair<int,string>(5, "mixed rain and snow") );
	weatherCodeMap.insert( pair<int,string>(6, "mixed rain and sleet") );
	weatherCodeMap.insert( pair<int,string>(7, "mixed snow and sleet") );
	weatherCodeMap.insert( pair<int,string>(8, "freezing drizzle") );
	weatherCodeMap.insert( pair<int,string>(9, "drizzle") );
	weatherCodeMap.insert( pair<int,string>(10, "freezing rain") );
	weatherCodeMap.insert( pair<int,string>(11, "showers") );
	weatherCodeMap.insert( pair<int,string>(12, "showers") );
	weatherCodeMap.insert( pair<int,string>(13, "snow flurries") );
	weatherCodeMap.insert( pair<int,string>(14, "light snow showers") );
	weatherCodeMap.insert( pair<int,string>(15, "blowing snow") );
	weatherCodeMap.insert( pair<int,string>(16, "snow") );
	weatherCodeMap.insert( pair<int,string>(17, "hail") );
	weatherCodeMap.insert( pair<int,string>(18, "sleet") );
	weatherCodeMap.insert( pair<int,string>(19, "dust") );
	weatherCodeMap.insert( pair<int,string>(20, "foggy") );
	weatherCodeMap.insert( pair<int,string>(21, "haze") );
	weatherCodeMap.insert( pair<int,string>(22, "smoky") );
	weatherCodeMap.insert( pair<int,string>(23, "blustery") );
	weatherCodeMap.insert( pair<int,string>(24, "windy") );
	weatherCodeMap.insert( pair<int,string>(25, "cold") );
	weatherCodeMap.insert( pair<int,string>(26, "cloudy") );
	weatherCodeMap.insert( pair<int,string>(27, "mostly cloudy (night)") );
	weatherCodeMap.insert( pair<int,string>(28, "mostly cloudy (day)") );
	weatherCodeMap.insert( pair<int,string>(29, "partly cloudy (night)") );
	weatherCodeMap.insert( pair<int,string>(30, "partly cloudy (day)") );
	weatherCodeMap.insert( pair<int,string>(31, "clear (night)") );
	weatherCodeMap.insert( pair<int,string>(32, "sunny") );
	weatherCodeMap.insert( pair<int,string>(33, "fair (night)") );
	weatherCodeMap.insert( pair<int,string>(34, "fair (day)") );
	weatherCodeMap.insert( pair<int,string>(35, "mixed rain and hail") );
	weatherCodeMap.insert( pair<int,string>(36, "hot") );
	weatherCodeMap.insert( pair<int,string>(37, "isolated thunderstorms") );
	weatherCodeMap.insert( pair<int,string>(38, "scattered thunderstorms") );
	weatherCodeMap.insert( pair<int,string>(39, "scattered thunderstorms") );
	weatherCodeMap.insert( pair<int,string>(40, "scattered showers") );
	weatherCodeMap.insert( pair<int,string>(41, "heavy snow") );
	weatherCodeMap.insert( pair<int,string>(42, "scattered snow showers") );
	weatherCodeMap.insert( pair<int,string>(43, "heavy snow") );
	weatherCodeMap.insert( pair<int,string>(44, "partly cloudy") );
	weatherCodeMap.insert( pair<int,string>(45, "thundershowers") );
	weatherCodeMap.insert( pair<int,string>(46, "snow showers") );
	weatherCodeMap.insert( pair<int,string>(47, "isolated thundershowers") );
	weatherCodeMap.insert( pair<int,string>(3200, "not available") );
	
	
	
	weatherCodeMapReduced.insert( pair<int,string>(0, "severe") );
	weatherCodeMapReduced.insert( pair<int,string>(1, "severe") );
	weatherCodeMapReduced.insert( pair<int,string>(2, "severe") );
	weatherCodeMapReduced.insert( pair<int,string>(3, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(4, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(37, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(38, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(39, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(45, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(47, "storm") );
	weatherCodeMapReduced.insert( pair<int,string>(5, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(6, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(8, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(9, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(10, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(11, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(12, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(35, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(40, "rain") );
	weatherCodeMapReduced.insert( pair<int,string>(19, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(20, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(21, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(22, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(23, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(24, "reducedVis") );
	weatherCodeMapReduced.insert( pair<int,string>(26, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(27, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(28, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(29, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(30, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(44, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(31, "sunny") );
	weatherCodeMapReduced.insert( pair<int,string>(32, "sunny") );
	weatherCodeMapReduced.insert( pair<int,string>(33, "sunny") );
	weatherCodeMapReduced.insert( pair<int,string>(34, "sunny") );
	weatherCodeMapReduced.insert( pair<int,string>(36, "sunny") );
	weatherCodeMapReduced.insert( pair<int,string>(7, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(13, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(14, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(15, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(16, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(17, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(18, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(25, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(41, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(42, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(43, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(46, "snow") );
	weatherCodeMapReduced.insert( pair<int,string>(3200, "na") );
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
//        window = SDL_CreateWindow( "Server", windowRect.x, windowRect.y, windowRect.w, windowRect.h, 0 );
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
