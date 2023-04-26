#include "init.hxx"

namespace fs = boost::filesystem;

bool 
init_dir (char* dir)
{
    bool            success;

    success = true;

    fs::path parent_loc = dir;
    success = success & fs::create_directories (dir);


    return success;
}
