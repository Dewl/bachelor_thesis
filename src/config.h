#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
using namespace std;

unordered_map<string, string> config_Read(const char *fname);
void config_Write(unordered_map<string, string>& config,
		const char *fname);
int config_GetInt(unordered_map<string, string>& config,
		const string& key, int def);

double config_GetDouble(unordered_map<string, string>& config,
		const string& key, double def);

void config_SetInt(unordered_map<string, string>& config,
		const string& key, int val);

void config_SetDouble(unordered_map<string, string>& config,
		const string& key, double val);

#endif /* !CONFIG_H */
