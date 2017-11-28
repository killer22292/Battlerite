#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class Config
{
	static string file;
	Config();
public:
	struct Structure
	{
		string Section;
		string Key;
		string Value;
	};

	static vector<Structure> GetRecord(string section, string key);

	static bool SectionExists(string name);
	static bool KeyExists(string section, string name);

	static string GetValue(string section, string key);
	static bool SetValue(string section, string key, string value);

	static bool Load(vector<Structure>& content);
	static bool Save(vector<Structure>& content);
	static bool LoadConfig();
private:
	static void Trim(string& string);

	struct Section : std::unary_function<Structure, bool>
	{
		string section_;

		Section(const string& section) : section_(section) {}

		bool operator()(const Structure& rec) const
		{
			return rec.Section == section_;
		}
	};

	struct Key : unary_function<Structure, bool>
	{
		string section_;
		string key_;

		Key(const string& section, const string& key) : section_(section), key_(key) {}

		bool operator()(const Structure& rec) const
		{
			return ((rec.Section == section_) && (rec.Key == key_));
		}
	};
};