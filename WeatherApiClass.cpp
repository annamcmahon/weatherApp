#include "WeatherApiClass.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <map>
#include <vector>
using namespace std;
using boost::property_tree::ptree;
WeatherApiClass::WeatherApiClass(string woeidLookUp) {
	woeid= woeidLookUp;
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
	city = findDataMemberWithFullPathString(pt, "query.results.channel.location.city");
	state = findDataMemberWithFullPathString(pt, "query.results.channel.location.region");
	allTemps.push_back(date);
	allTemps.push_back(temp); 
        allTemps.push_back(textDescription);
	allTemps.push_back(high);
        allTemps.push_back(low);
	allTemps.push_back(city);
	allTemps.push_back(state);
	
	fillMaps();
}
void WeatherApiClass::commandLinePrint() {
	cout<< "the current condition code is: "<< code<< endl;
	cout<<"the current condition text is: "<< textDescription<< endl;
	cout<<"the current temperature is by return is: "<< temp<< endl;
	cout<<"the date : "<< date<< endl;
	cout<<"the high today: "<< high<< endl;
	cout<<"the low today: "<< low<< endl;
	cerr << endl; // cerr is like cout but error checking(?), outputs immediately
}
int WeatherApiClass::getWeatherConditionCode() {
	string stringCode = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.code");
	int code = atoi(stringCode.c_str());//convert string to interger
	return code;
}
void WeatherApiClass::queryWeatherApiForSimpleStats() {
        system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select item.condition from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > weatherDataFile").c_str());
        weatherDataFile = "weatherDataFile";
}
void WeatherApiClass::queryWeatherApiForExtendedStats() {
         system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select * from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > weatherDataFileExtended").c_str());
	weatherDataFileExtended = "weatherDataFileExtended";
}
string WeatherApiClass::findDataMemberWithFullPathString(const ptree& pt, string key) {
        return pt.get<string>(key);
}  
int WeatherApiClass::getCode() {
	return weatherConditionCode;
}
vector<string> WeatherApiClass::getTemp() {
	return allTemps;
}
string WeatherApiClass::getDate() {
        return date;
}
string WeatherApiClass::getCondition() {
        return textDescription;
}



string WeatherApiClass::getPicFilename() {
	return weatherCodeMapReduced[weatherConditionCode];
}
void WeatherApiClass::fillMaps() {
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
	weatherCodeMapReduced.insert( pair<int,string>(27, "cloudynight") );
	weatherCodeMapReduced.insert( pair<int,string>(28, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(29, "cloudy") );
	weatherCodeMapReduced.insert( pair<int,string>(30, "cloudynight") );
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
