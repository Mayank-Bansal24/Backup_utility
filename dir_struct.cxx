#include <boost/filesystem/operations.hpp>
#include "dir_struct.hxx"

void dir_struct :: add_file (fs::path p,
                             vector<file_data> &files)
{
  file_data*      new_file;

  new_file = new file_data (p);
  files.push_back (*new_file);

  return;
}

intmax_t dir_struct :: get_dir_size()
{
  intmax_t size = 0;

  for (auto it:this->files)
    size += it.get_file_size();
  
  return size;
}

vector<file_data> dir_struct:: get_files()
{
  return this->files;
}

void dir_struct :: get_files_from_dir_h (fs::path p, 
                                         vector<file_data> &files)
{
   vec          v;

   copy (directory_iterator(p), directory_iterator(), back_inserter(v));
   
   for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
   {
     if (is_regular_file (*it))
        add_file (*it, files);
     else 
        get_files_from_dir_h (*it, files);
   }

  return;
}

void dir_struct:: save_files ()
{
  
  string output_file = "./files_backup.tar.gz";
  struct archive *a = archive_write_new();
  struct archive_entry *entry = nullptr;

  archive_write_add_filter_gzip(a);
  archive_write_set_format_pax_restricted(a);
  archive_write_open_filename(a, output_file.c_str());

  std::vector<char> buf(16384);
    for(auto file: this->files) {

      if (file.get_path().string() == "") continue;
      std::string file_path = file.get_path().string();
      std::string backup_file_path = file_path;
      std::replace(backup_file_path.begin()+2, backup_file_path.end(), '/', '\\');

      entry = archive_entry_new();
      archive_entry_set_pathname(entry, backup_file_path.c_str());
      archive_entry_set_size(entry, file.get_file_size());
      archive_entry_set_filetype(entry, AE_IFREG);
      archive_entry_set_perm(entry, 0644);
      archive_write_header(a, entry);

      std::ifstream in_file(file_path, std::ios::binary);
      while(!in_file.eof()) {
          in_file.read(buf.data(), buf.size());
          archive_write_data(a, buf.data(), in_file.gcount());
      }

      archive_entry_free(entry);
  }

  archive_write_close(a);
  archive_write_free(a);

}

void dir_struct:: load_files ()
{

  string input_file = "./files_backup.tar.gz";

    struct archive *a = archive_read_new();
    struct archive_entry *entry = nullptr;
    int r;

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, input_file.c_str(), 10240);
    if (r != ARCHIVE_OK) {
        printf("archive not ok\n");
        return;
    }

    std::vector<char> buf(16384);
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    
        string txt = archive_entry_pathname(entry);
        std::replace(txt.begin(), txt.end(), '\\', '/');
        txt.erase(0, 1);
        txt = "./diri" + txt;
        string dir_path = txt.substr(0, txt.rfind("/"));
        cout << dir_path << '\n';
        filesystem::create_directories(dir_path);
        std::ofstream x_file(txt, std::ios::binary);

        while(1) {
            ssize_t size = archive_read_data(a, buf.data(), buf.size());
            if (size < 0) {
                archive_read_free(a);
                return;
            }
            if(size == 0) {
                break;
            }

            x_file.write(buf.data(), size);   
        }
    }

    archive_read_free(a);
}

vector <file_data> dir_struct::get_files_from_dir (fs::path p)
{
  vector <file_data>               vec_files;

  get_files_from_dir_h (p, vec_files);

  return vec_files;  
}

dir_struct :: dir_struct (fs::path p)
{
  this->loc = p;

  try
  {
    if (exists(p))    
    {
      if (is_regular_file(p))        
        {
          cout << "fs::path must be a directory not a file.\n" << '\n';   
        }  
      else if (is_directory (p))      
        {
          this->files = get_files_from_dir (p);
          this->dir_size  = get_dir_size();
        }
      else
        cout << p << " exists, but is not a directory\n";
    }
    else
      cout << p << "fs::path does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return;
}

vector <file_data> dir_struct :: get_new_files ()
{
    map <fs::path, file_data*>              old_struct;
    vector<file_data>                       new_files, new_struct;
    
    new_struct = get_files_from_dir (this->loc);
    
    for (auto &it:this->files)
      old_struct [it.get_path()] = &it;

    for(auto it:new_struct)
    {
      if (old_struct[it.get_path()] == NULL)
        new_files.push_back(it);
    }

    return new_files;  
}

vector <file_data> dir_struct :: get_mod_files (vector<file_data> prev_version)
{
  file_data*                          old_file = NULL;
  vector <file_data>                  mod_files, new_files;
  map <fs::path, file_data>           mp;

  for(auto it: this->files)
    mp[it.get_path()] = it;
  
  for(auto it: prev_version)
  {
    *old_file = mp[it.get_path()];

    if (old_file == NULL)
      {
          old_file->set_status (DELETED);
          mod_files.push_back (*old_file);
      }
    else if (mp[it.get_path ()].get_last_mod_time () > it.get_last_mod_time ())
      {
          old_file->set_status (MODIFIED);
          mod_files.push_back (*old_file);
      }
  }

  new_files = get_new_files();

  for (auto it:new_files)
    mod_files.push_back (it);

  return mod_files;
}

vector <file_data> dir_struct :: get_status ()
{
  vector <file_data> prev = this->prev_version->get_files();

  return prev;  
}

