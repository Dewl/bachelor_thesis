#include "config.h"
#include <fstream>

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

int config_GetInt(unordered_map<string, string>& config, const string& key)
{
	return stoi(config[key]);
}
