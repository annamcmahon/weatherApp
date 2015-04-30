#include <iostream>
#include <string>
#include <stdio.h>
#include <iostream>
#include "WeatherApiClass.h"
#include "sdlClass.h"
#include "renderableImage.h"
using namespace std;
int main(int argc, char** argv) {	
	string temp; // temperature
	int weatherConditionCode; // code used by yahoo weather api
	WeatherApiClass weatherApi; // declaring object
	weatherConditionCode = weatherApi.getCode(); // returns int code	
	temp = weatherApi.getTemp(); // returns string with temperature
	weatherApi.commandLinePrint(); // prints to cout
	string pictureFilenameString = weatherApi.getPicFilename(); // gets filename excluding extension (relative path as well)
	int delay = 5000; // 5 seconds
	cout<< "here";
	sdlClass(pictureFilenameString,temp,delay); // only needs to be declared
	return 0;
} 
