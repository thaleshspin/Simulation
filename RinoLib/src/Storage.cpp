#include "NaoLib/Utils/Storage.h"
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace Rinobot::Nao::Utils;
using namespace std;

Storage::Storage()
{
    
}

Storage::Storage(const string fileName)
{
    Open(fileName);
}

Storage::Storage(const string path, const string fileName)
{
    Open(path, fileName);
}

bool Storage::IsOpened()
{
    return isOpened;
}

bool Storage::Open(const string fileName)
{
    if(IsOpened())
        Close();
    
    string path = fileName;

    char *homeEnv = getenv("HOME");
    if(homeEnv != NULL)
    {
        string home = homeEnv;
        size_t found = path.find("~");
        if (found != string::npos)
        {
            path.replace(found, 1, home);
        }
    }

    string folder = path.substr(0, path.find_last_of("\\/"));
    ss.str("");
    ss << "mkdir -p " << folder;
    system(ss.str().c_str());

    this->fileName = path;
    isOpened = true;
    
    Read();
    return isOpened;
}

bool Storage::Open(const string path, const string fileName)
{
    return Open(FILE(path, fileName));
}

void Storage::Close()
{
    if(IsOpened())  
        Write();   
    data.clear();
    isOpened = false;
}

bool Storage::Get(const string key, float *value)
{
    if ( data.find(key) == data.end() )
        return false;
    *value = atof(data[key].c_str());
    return true;
}

bool Storage::Get(const string key, int *value)
{
    if ( data.find(key) == data.end() )
        return false;
    *value = atoi(data[key].c_str());
    return true;
}

bool Storage::Get(const string key, string *value)
{
    if ( data.find(key) == data.end() )
        return false;
    *value = data[key];
    return true;
}

void Storage::Set(const string key, float value)
{
    ss.str("");
    ss << value;
    data[key] = ss.str();
}

void Storage::Set(const string key, int value)
{
    ss.str("");
    ss << value;
    data[key] = ss.str();
}

void Storage::Set(const string key, string value)
{
    data[key] = value;
}

void Storage::Remove(const std::string key)
{
    map<string, string>::iterator it = data.find(key);
    if ( it != data.end() )
        data.erase(it);
}

void Storage::Read()
{
    if(!IsOpened())
        return;

    std::ifstream file(fileName.c_str());
    if(!file.is_open())
        return;

    string key, value;
    while(!file.eof())
    {
        file >> key;
        if(file.eof()) break;
        file >> value;
        if(file.eof()) break;
        data[key] = value;
    }
    file.close();
}

void Storage::Write()
{
    if(!IsOpened())
        return;

    ss.str("");
    for (map<string, string>::iterator it = data.begin(); it != data.end(); it++ )
    {
        ss << it->first << "\t\t" << it->second << endl;
    }
    std::ofstream file(fileName.c_str());
    file << ss.str();
    file.close();
}