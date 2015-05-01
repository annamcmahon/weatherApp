//
//  callSynthesisHelper.cpp
//
// displays visually some of the methods used to obtain the full paths used in getWeatherData.cpp

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
	
	printTree(pt, 0); // prints the whole pt tree nicely depends on indent function, to get and use this function see parseTree.cpp
	parseForFullPath(pt, ""); // this gives you all the full paths,  which can be used in the below function
	
	
	
	// another way to loop through tree
	BOOST_FOREACH(const ptree::value_type &v, pt.get_child("query.results.channel.item.condition")) {
		cout<< v.first; // v.first is the name of the child
		cout<< ": " << v.second.data()<< endl; // v.second is the child tree
	}
	

	 woeid = "91418771"; // this is the location
         system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select * from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > weatherDataFileExtended").c_str());
        weatherDataFileExtended = "weatherDataFileExtended";


	   string woeid = "91418771"; // this is the location

        system(("curl https://query.yahooapis.com/v1/public/yql \
                        -d q=\"select item.condition from weather.forecast where woeid="+woeid+"\" \
                        -d format=json > file2").c_str());

        
        string tagfile = "file2";
        ptree pt;   // "ptree" or property tree is a struct defined by boosts' lib
        read_json(tagfile, pt);

        printTree(pt, 0); // prints the whole pt tree nicely depends on indent function, to get and use this function see parseTree.cpp
        parseForFullPath(pt, ""); // this gives you all the full paths,  which can be used in the below function



        // another way to loop through tree
        BOOST_FOREACH(const ptree::value_type &v, pt.get_child("query.results.channel.item.condition")) {
                cout<< v.first; // v.first is the name of the child
                cout<< ": " << v.second.data()<< endl; // v.second is the child tree
        }




	
}
