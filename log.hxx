#pragma once

#include <bits/stdc++.h>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

enum status_flags
{
    ERROR,
    WARNING,
    DEBUG,
    INFORMATION,
    
};

class logger{
    string flag_req;
    std::ofstream log_file;
    logger(const logger&);
    logger& operator=(const logger&);
    
    public:
    logger() : log_file("temp.txt", std::ofstream::app) {}
    
    // logger(string filename,string flag_req);
    void set_flags(string z);
    void close_p ();
    void print(string s,int type);
};

