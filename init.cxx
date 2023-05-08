#include "init.hxx"
#include "dir_struct.hxx"


bool 
init_dir_i (string dir2, logger *new_instance)
{
    bool            success;
    success = true;

    // fs::path parent_loc = dir1;
    success = success & fs::create_directories (".backup_util");
    
    path p(dir2);
    dir_struct dir = dir_struct(p, new_instance);

    for (auto it: dir.get_files())
    {
        cout << it.get_path().string() << "\n";
    }

    return success;
}
