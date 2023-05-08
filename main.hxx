#pragma once
#include <iostream>
#include <boost/filesystem.hpp>

#include "init.hxx"
#include "dir_struct.hxx"

namespace fs = boost::filesystem;
using namespace std;

class version {

    dir_struct*         curr_version;
    string              version_no;
    time_t              commit_time;

    public:
    version             (dir_struct* curr_version, string version_no);
    dir_struct*         get_dir_struct ();
    string              get_version ();

};

class backup_util {

    vector      <version>   version_list;
    dir_struct              *curr_status;
    string                  author;
        
    public:
    backup_util             ();
    version*                get_prev_version (string version_no);
};
