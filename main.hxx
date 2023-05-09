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
    string                  author_name;
    string                  project_name;
    fs::path                loc;
    logger*                 log;              
        
    public:
    backup_util             (fs::path loc, logger* log);
    bool                    status (dir_struct last_ver);
    bool                    add    ();
    bool                    commit ();
    bool                    restore();
    bool                    init   (vector<string> &args);
    int                     remoteutil (int argc, vector<string> argv);
    vector<dir_struct>      get_prev_version (string version_no);
    dir_struct              get_last_dir_struct ();
    void                    set_author_name(string s);
    void                    set_project_name(string s);
        
};
