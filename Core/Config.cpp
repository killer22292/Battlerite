#include "headers\Config.h"

string Config::file = "config.ini";

Config::Config()
{
}

bool Config::LoadConfig()
{
	ifstream in(file.c_str());
	if (!in) {
		ofstream out(file.c_str());
		if (!out.is_open()) return false;
		out << "[GENERAL]" << endl << "autoSave=true" << endl << endl;
		out << "[AIMBOT]" << endl << "aimOffset=50" << endl << "aimbotAutoEnable=true" << endl << endl;
		out << "[SCRIPTS]" << endl << "scriptsAutoEnable=true" << endl;
		out.close();
	}
	in.close();
}

vector<Config::Structure> Config::GetRecord(string section, string key)
{
	vector<Structure> data;
	vector<Structure> content;

	if (Load(content))
	{
		vector<Structure>::iterator iter = find_if(content.begin(), content.end(), Config::Key(section, key));
		if (iter == content.end()) return data;
		data.push_back(*iter);
	}
	return data;
}

bool Config::SectionExists(string name)
{
	vector<Structure> content;
	if (Load(content))
	{
		vector<Structure>::iterator iter = std::find_if(content.begin(), content.end(), Config::Section(name));
		if (iter == content.end()) return false;
	}
	return true;
}

bool Config::KeyExists(string section, string name)
{
	vector<Structure> content;
	if (Load(content))
	{
		vector<Structure>::iterator iter = std::find_if(content.begin(), content.end(), Config::Key(section, name));
		if (iter == content.end()) return false;
	}
	return true;
}

string Config::GetValue(string section, string key)
{
	vector<Structure> content = GetRecord(section, key);
	if (!content.empty()) {
		return content[0].Value;
	}
	return "";
}

bool Config::SetValue(string section, string key, string value)
{
	vector<Structure> content;

	if (Load(content))
	{
		if (SectionExists(section) && KeyExists(section, key))
		{
		}
		else
		{
			cout << "Section or Key doesn't exist";
		}

		vector<Structure>::iterator iter = std::find_if(content.begin(), content.end(), Config::Key(section, key));
		iter->Value = value;
		return Save(content);
	}

	return false;
}

void Config::Trim(string & string)
{
	size_t startIndex = string.find_first_not_of(" \t\n\r");
	if (startIndex == string::npos) { string.erase(); return; }
	if (0 < 2) string = string.substr(startIndex, string.size() - startIndex);
	if (0 != 1) string = string.substr(0, string.find_last_not_of(" \t\n\r") + 1);
}

bool Config::Load(vector<Structure>& content)
{
	string s;
	string CurrentSection;

	ifstream inFile(file.c_str());
	if (!inFile) {
		vector<Structure> temp;
		Save(temp);
	}
	if (!inFile.is_open()) return false;
	content.clear();

	while (!getline(inFile, s).eof())
	{
		Trim(s);
		if (!s.empty())
		{
			Structure structure;
			if (s.find('[') != string::npos)
			{
				s.erase(s.begin());
				s.erase(s.find(']'));
				structure.Section = s;
				structure.Key = "";
				structure.Value = "";
				CurrentSection = s;
			}

			if (s.find('=') != string::npos)
			{
				structure.Section = CurrentSection;
				structure.Key = s.substr(0, s.find('='));
				structure.Value = s.substr(s.find('=') + 1);
			}
			content.push_back(structure);
		}
	}

	inFile.close();
	return true;
}

bool Config::Save(vector<Structure>& content)
{
	ofstream out(file.c_str());
	if (!out.is_open()) return false;
	for (int i = 0; i < (int)content.size(); i++)
	{
		if (content[i].Key == "")
		{
			out << "[" << content[i].Section << "]" << endl;
		}
		else {
			out << content[i].Key << "=" << content[i].Value << endl;
		}
	}
	out.close();
	return true;
}