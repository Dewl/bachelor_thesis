#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>
using namespace std;

unordered_map<string, string> config_Read(const char *fname);
int config_GetInt(unordered_map<string, string>& config, const string& key);

#endif /* !CONFIG_H */
