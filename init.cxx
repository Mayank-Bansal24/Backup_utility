#include "init.hxx"
#include "dir_struct.hxx"


bool 
init_dir_i (string path, logger *new_instance)
{
    bool            success;
    success = true;

    fs::path dir(path);
    // fs::path parent_loc = dir1;
    success = success & fs::create_directories (dir.string() + ".backup_util");
    fs::create_directories (dir.string() + ".backup_util/firebase"); 
    fs::create_directories (dir.string() + ".backup_util/versions");
    dir_struct dir_struct_obj = dir_struct(dir, new_instance);

    for (auto it: dir_struct_obj.get_files())
    {
        cout << it.get_path().string() << "\n";
    }

    return success;
}
