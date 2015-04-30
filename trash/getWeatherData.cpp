//getWeatherData.cpp

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH
#include <stdio.h>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <iostream>
#include <boost/optional/optional.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::property_tree::ptree;


boost::optional<string> findDataMemberWithFullPathString(const ptree& pt, string key){
	boost::optional<string> v = pt.get_optional<string>(key);
	return pt.get_optional<string>(key);
}

int main(int, char*[]) {

	string woeid = "91418771"; // this is the location
	
	system(("curl https://query.yahooapis.com/v1/public/yql \
		   -d q=\"select item.condition from weather.forecast where woeid="+woeid+"\" \
		   -d format=json > file").c_str());

	
	string tagfile = "file";
	ptree pt;   // "ptree" or property tree is a struct defined by boosts' lib
	read_json(tagfile, pt);
	
	//printTree(pt, 0); // prints the whole pt tree nicely depends on indent function, to get and use this function see parseTree.cpp
	//parseForFullPath(pt, ""); // this gives you all the full paths,  which can be used in the below function

	boost::optional<string> temp, textDescription, code, date, high, low; // all the variables to parse for
	
	// query for the current conditions
	temp = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.temp");
	textDescription = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.text");
	code = findDataMemberWithFullPathString(pt, "query.results.channel.item.condition.code");

	cout<< "the current condition code is: "<< code<< endl;
	cout<<"the current condition text is: "<< textDescription<< endl;
	cout<<"the current temperature is by return is: "<< temp<< endl;
	
	
	// query for general day information
	system(("curl https://query.yahooapis.com/v1/public/yql \
			-d q=\"select * from weather.forecast where woeid="+woeid+"\" \
			-d format=json > file2").c_str());
	
	tagfile = "file2";
	read_json(tagfile, pt);
	
	date = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..date");
	high = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..high");
	low = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..low");
	
	cout<<"the date : "<< date<< endl;
	cout<<"the high today: "<< high<< endl;
	cout<<"the low today: "<< low<< endl;
	
	cerr << endl; // cerr is like cout but error checking(?), outputs immediately
}

