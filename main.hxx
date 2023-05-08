#pragma once
#include <iostream>
#include <boost/filesystem.hpp>

#include "init.hxx"
#include "dir_struct.hxx"

namespace fs = boost::filesystem;
using namespace std;

class backup_util {

    vector <dir_struct>     version_list;
    dir_struct              curr_status;
    string                  author;
        
    public:
    backup_util             ();
    vector<dir_struct>      get_prev_version (string version_no);
};
