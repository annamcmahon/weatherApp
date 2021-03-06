#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <map>
#include <vector>
using boost::property_tree::ptree;
using namespace std;
class WeatherApiClass {
public:
	WeatherApiClass(string);
	void commandLinePrint();
	int getCode();
	vector<string> getTemp();
	string getPicFilename();
	string getDate();
	string getCondition();
private:
	void queryWeatherApiForSimpleStats();
	void fillMaps();
	void queryWeatherApiForExtendedStats();
	string findDataMemberWithFullPathString(const ptree&, string);
	int getWeatherConditionCode();	
	int weatherConditionCode;
	vector<string> allTemps;
	string weatherDataFile;//stores the name of the file the weahter data is queried into
	string weatherDataFileExtended;
	string woeid; // this is the location
	string code; // this is the weather code gotten from the API, used to determine image display
	ptree pt;   // "ptree" or property tree is a struct defined by boosts' lib
	string temp, textDescription, date, high, low, city, state; // all the variables to parse for
	map<int,string> weatherCodeMap;
	map<int,string> weatherCodeMapReduced;
};
