#include "init.hxx"
#include "dir_struct.hxx"

namespace fs = boost::filesystem;

bool 
init_dir (char* dir2)
{
    bool            success;

    success = true;

    // fs::path parent_loc = dir1;
    // success = success & fs::create_directories (dir1);
    
    dir_struct dir = dir_struct(dir2);

    for (auto it: dir.get_files())
    {
        cout << it.get_path().string() << "\n";
    }

    return success;
}
