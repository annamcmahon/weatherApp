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

string indent(int level) {
        string s;
        for (int i=0; i<level; i++) s += "  ";
        return s;
}

void printTree (ptree &pt, int level) {
        if (pt.empty()) {
                cerr << "\""<< pt.data()<< "\"";
        } else {
                if (level) cerr << endl;
                cerr << indent(level) << "{" << endl;
                for (ptree::iterator pos = pt.begin(); pos != pt.end();) {
                        cerr << indent(level+1) << "\"" << pos->first << "\": ";
                        printTree(pos->second, level + 1);// recursively called
                        ++pos;
                        if (pos != pt.end()) {
                                cerr << ",";
                        }
                        cerr << endl;
                }
                cerr << indent(level) << " }";
        }
        return;
}


void parseForFullPath(const ptree& pt, string key)
{
        string nkey;
        if (!key.empty()){
                nkey = key + ".";
                cout<<nkey<<endl;
        }
        ptree::const_iterator end = pt.end();
        for (ptree::const_iterator it = pt.begin(); it != end; ++it){
                parseForFullPath(it->second, nkey + it->first);
        }
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

	boost::optional<string> temp, textDescription, code, date, high, low ,l1, l2, l3; // all the variables to parse for
	
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
	        date = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..date");
        high = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast..high");
        low = findDataMemberWithFullPathString(pt, "query.results.channel.item.forecast.3.day");

        cout<<"the date : "<< date<< endl;
        cout<<"the high today: "<< high<< endl;
        cout<<"the low today: "<< low<< endl;
		
	 l1 = findDataMemberWithFullPathString(pt, "query.results.channel.location.city");
        l2 = findDataMemberWithFullPathString(pt, "query.results.channel.location.country");
        l3 = findDataMemberWithFullPathString(pt, "query.results.channel.location.region");
	cout<<"the location "<< l1<< endl;
        cout<<"the  "<< l2<< endl;
        cout<<"the "<< l3<< endl;
	parseForFullPath( pt, "");
	   // query for general day information
        system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select * from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > file2").c_str());

        tagfile = "file2";
        read_json(tagfile, pt);
	parseForFullPath( pt, "query.results.channel.item.forecast.");
	parseForFullPath( pt, "Mon");
	cerr << endl; // cerr is like cout but error checking(?), outputs immediately
}






