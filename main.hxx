#pragma once
#include <iostream>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <filesystem>

#include "init.hxx"
#include "dir_struct.hxx"
#include "log.hxx"
#include "json.hxx"

namespace fs = boost::filesystem;
using namespace std;
using json = nlohmann::json;

class backup_util {
public:
    vector <dir_struct>     version_list;
    dir_struct              curr_status;
    string                  author_name;
    string                  project_name;
    fs::path                loc;
    logger*                 log;              
        
    public:
    backup_util             (fs::path loc, logger* log);
    bool                    status (dir_struct last_ver);
    bool                    init   (vector<string> &args);
    dir_struct              get_last_dir_struct ();
    void                    set_author_name(string s);
    void                    set_project_name(string s);
    backup_util             (fs::path p);
    bool                    add    ();
    bool                    commit ();
    bool                    restore();
    int                     remoteutil (int argc, vector<string> argv);
    vector<dir_struct>      get_prev_version (string version_no);
    void                    dump_backup_util (fs::path p);
        
};
