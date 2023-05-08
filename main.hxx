#pragma once
#include <iostream>
#include <boost/filesystem.hpp>

#include "init.hxx"
#include "dir_struct.hxx"

class version {

    public:
    version ();

};

class backup_util {

    vector      <version>  version_list;
    dir_struct              *curr_status;
    
    public:
    backup_util ();
};