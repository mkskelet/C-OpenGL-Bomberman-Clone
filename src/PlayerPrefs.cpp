///
/// Class providing interface for saving / loading data.
///

#include "PlayerPrefs.h"
#include <sstream>
#include <iostream>	// temp
using namespace std;

std::string PlayerPrefs::path = "./examples/player.prefs";

void PlayerPrefs::SetInt(std::string key, int value) {
	ostringstream ss;
	ss << value;
	SetString(key, ss.str());
}

void PlayerPrefs::SetFloat(std::string key, float value) {
	ostringstream ss;
	ss << value;
	SetString(key, ss.str());
}

void PlayerPrefs::SetString(std::string key, std::string value) {
	fstream file;
	
	//cout << "TRYING TO SAVE KEY (" << key << ") with value " << "(" << value << ")\n";	// TEMP
	
	if(GetString(key, "NULL").compare("NULL") == 0) {
		file.open(path.c_str(), ios::out | ios::app);
		
		file << key << "=" << value << "\n";
		
		file.close();
	}
	else {
		string playerPrefs = "";
		file.open(path.c_str(), ios::in);
		
		string line;
		while(getline(file, line, '=')) {
			//cout << line << "\n";
			playerPrefs += line;
			playerPrefs += "=";
			string l;
			getline(file, l);
			if(line.compare(key) == 0) {
				playerPrefs += value;
				playerPrefs += "\n";
			}
			else {
				playerPrefs += l;
				playerPrefs += "\n";
			}
		}
		
		file.close();
		
		file.open(path.c_str(), ios::out | ios::trunc);
		
		file << playerPrefs;
		
		file.close();
	}	
}

int PlayerPrefs::GetInt(std::string key, int defaultValue) {
	string s = GetString(key, "NULL");
	//cout << "GOT INT: " << s << "\n";		// TEMP
	if(s.compare("NULL") == 0) return defaultValue;
	else return atoi(s.c_str());
}

float PlayerPrefs::GetFloat(std::string key, float defaultValue) {
	string s = GetString(key, "NULL");
	
	if(s.compare("NULL") == 0) return defaultValue;
	else return atof(s.c_str());
}

std::string PlayerPrefs::GetString(std::string key, std::string defaultValue) {
	ifstream file;
	
	file.open(path.c_str(), ios::in);
	if(file.is_open()) {
		string line;
		//cout << "COMPARING KEY(" << key << ") to lines:\n";	// TEMP
		while(getline(file, line, '=')) {
			//cout << "-\t" << line << "\t- ";				// TEMP
			string l;			// value
			getline(file, l);
			if(line.compare(key) == 0) {
				//cout << "comparison successfull\n";		//TEMP
				
				//cout << "EXTRACTED LINE: " << l << "\n";	// TEMP
				file.close();
				return l;
			}
			//cout << "comparison failed\n";				// TEMP	
		}
		file.close();
	}
	
	return defaultValue;
}