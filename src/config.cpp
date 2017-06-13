#include "config.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

unordered_map<string, string> config_Read(const char *fname)
{
	unordered_map<string, string>  ret;
	ifstream in(fname);

	string key, val;
	while (in >> key >> val) {
		ret.insert(make_pair<string, string>
				((string) key, (string) val));
	}

	in.close();
	return ret;
}

void config_Write(unordered_map<string, string>& config,
		const char *fname)
{
	ofstream out(fname);

	for (auto it = config.begin(); it != config.end(); ++it) {
		out << it->first << " " << it->second << endl;
	}
	out.close();
}

int config_GetInt(unordered_map<string, string>& config,
		const string& key, int def)
{
	if (config[key] == "") {
		config[key] = to_string(def);
	}
	return stoi(config[key]);
}


double config_GetDouble(unordered_map<string, string>& config,
		const string& key, double def)
{
	if (config[key] == "") {
		config[key] = to_string(def);
	}

	stringstream convert;
	convert << config[key] << endl;

	double ret;
	convert >> ret;

	return ret;
}

void config_SetInt(unordered_map<string, string>& config,
		const string& key, int val)
{
	config[key] = to_string(val);
}

void config_SetDouble(unordered_map<string, string>& config,
		const string& key, double val)
{
	config[key] = to_string(val);
}
