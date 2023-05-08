#pragma once
#include <iostream>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <cstring>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <filesystem>

#include "init.hxx"
#include "dir_struct.hxx"
#include "log.hxx"

namespace fs = boost::filesystem;
using namespace std;

class backup_util {

    vector <dir_struct>     version_list;
    dir_struct              curr_status;
    string                  author;
    fs::path                loc;              
        
    public:
    backup_util             (fs::path loc, logger* log);
    vector<dir_struct>      get_prev_version (string version_no);
};
