#pragma once

#include <bits/stdc++.h>
#include <boost/filesystem.hpp>
#include "file.hxx"
#include <archive.h>
#include <archive_entry.h>
#include "log.hxx"

namespace fs = boost::filesystem;
using namespace std;

typedef vector <fs::path> vec;


class dir_struct
{   
    dir_struct*                 prev_version;
    intmax_t                    dir_size;

    vector <file_data>          files;
    path                        loc;


    void                        get_files_from_dir_h (fs::path p,
                                                      vector<file_data> &files);
    void                        add_file (fs::path p,
                                           vector<file_data> &files);     
    vector <file_data>          get_new_files ();  

    public:

    dir_struct (fs::path dir);
    
    intmax_t                    get_dir_size ();
    void                        save_files ();
    void                        load_files ();
    vector <file_data>          get_files ();
    vector <file_data>          get_mod_files (vector <file_data> prev_version);
    vector <file_data>          get_status ();
    vector <file_data>          get_files_from_dir (fs::path p);

};