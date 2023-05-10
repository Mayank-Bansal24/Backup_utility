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
    backup_util             ();
    bool                    status (dir_struct last_ver);
    bool                    add    (dir_struct last_ver);
    bool                    commit (dir_struct last_ver);
    bool                    restore ();
    bool                    init  (vector<string> &args);
    int                     remoteutil (int argc, vector<string> argv);
    vector<dir_struct>      get_prev_version ();
    dir_struct              get_last_dir_struct ();
    void                    set_author_name (string s);
    void                    set_project_name (string s);
    void                    add_dir_version (dir_struct *curr_status);
    void                    dump_backup_util ();
    void                    load_backup_util ();
    fs::path                get_path();
        
};
