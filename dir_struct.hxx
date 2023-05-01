#pragma once

#include <bits/stdc++.h>
#include <boost/filesystem.hpp>
#include "file.hxx"

using namespace boost::filesystem;
using namespace std;

typedef vector<path> vec;


class dir_struct
{   
    intmax_t                    dir_size;
    vector <file_data>          files;

    void                        get_files_from_dir (path p);
    void                        add_file (path p);       

    public:

    dir_struct (path dir);
    
    vector <file_data>          get_files ();
    intmax_t                    get_dir_size ();

};