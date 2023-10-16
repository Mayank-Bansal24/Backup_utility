#include "dir_struct.hxx"
#include "json.hxx"
#include <fstream>

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

   copy (fs::directory_iterator(p), fs::directory_iterator(), back_inserter(v));

   for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
   {
     if (p.string().find(".backup_util") != std::string::npos) continue;
     if (is_regular_file (*it))
        add_file (*it, files);
     else 
        get_files_from_dir_h (*it, files);
   }

  return;
}

void dir_struct :: set_mod_files (vector<file_data> &mod_files)
{
  this->mod_files = mod_files;
  return;
} 

void dir_struct :: set_commit_message (string message)
{
  this->commit_message = message;
}

string dir_struct :: get_commit_message ()
{
  return this->commit_message;
}

void dir_struct :: set_commit_time (time_t t)
{
  this->commit_time = t;
}

time_t dir_struct :: get_commit_time ()
{
  return this->commit_time;
}

void dir_struct :: save_files (int version_no)
{

  string output_file = "./.backup_util/versions/files_backup";
  output_file += to_string(version_no);
  output_file += ".tar.gz";
  
  struct archive *a = archive_write_new();
  struct archive_entry *entry = nullptr;

  archive_write_add_filter_gzip(a);
  archive_write_set_format_pax_restricted(a);
  archive_write_open_filename(a, output_file.c_str());

  std::vector<char> buf(16384);
    for(auto file: this->mod_files) {

      if (file.get_path().string().find(".backup_util") != std::string::npos) continue;
      if (file.get_status() == DELETED) continue;

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

void dir_struct:: load_files (int Version_no)
{

  // Delete previous files
  for (auto it: this->mod_files)
  {
    if (it.get_status() == DELETED)
    {
      // cout << it.get_path() << " deleted\n";
      fs::remove(it.get_path().string());
    }
  }

  // Load added and modified files
  string input_file = "./.backup_util/versions/files_backup"+to_string(Version_no)+".tar.gz";
  // cout<<input_file<<endl;

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
        // txt.erase(0, 1);
        txt = "" + txt;
        // cout <<  << '\n';
        // if (txt.rfind("/") > 1) {
          string dir_path = txt.substr(0, txt.rfind("/"));
          // cout << dir_path << "\n";
          if (dir_path != "."){
          fs::create_directories(dir_path); // TO be fixed
          }
        // }
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

dir_struct :: dir_struct ()
{
  this->loc = "";
  this->log = nullptr;
  this->dir_size = 0;
}

dir_struct :: dir_struct (fs::path p, logger* log)
{
  this->loc = p;
  this->log = log;
  try
  {
    
    if (exists(p))    
    {
      if (is_regular_file(p))        
        {
          log->print("Path must be a directory not a file.\n",'e');
          cout << "Path must be a directory not a file.\n" << '\n';   
        }  
      else if (is_directory (p)) {
          this->files = get_files_from_dir (p);
          this->dir_size  = get_dir_size();
        }
      else {
        log->print(p.string()+" exists, but is not a directory\n",'e');
        cout << p << " exists, but is not a directory\n";
      }
        
    }
    else{
      log->print(p.string()+"Path does not exist\n",'e');
      cout << p << "Path does not exist\n";
    }
      
  }

  catch (const fs::filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return;
}

vector <file_data> dir_struct :: get_new_files (vector<file_data> &previous_files)
{
    map <fs::path, file_data*>              old_struct;
    vector<file_data>                       new_files, new_struct;

    new_struct = get_files_from_dir (this->loc);

    for (auto &it:previous_files)
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
  file_data                          curr_file ,
                                     old_file;
  vector <file_data>                  mod_files, new_files;
  map <string, file_data>           mp;
  
  for(auto it: this->files)
      mp[it.get_path().string()] = it;      // we map the file's path to the file

  for(auto it: prev_version)
  {
      if (it.get_path().string().find(".backup_util") != std::string::npos) continue;

      curr_file = mp[it.get_path().string()];
      old_file = it;
     
    if (curr_file.get_file_size() < 0)
      {
        // in case file was present in previous version, we set its status to be deleted and push in the modified list
          old_file.set_status (DELETED);
          mod_files.push_back (old_file);
      }
    else if (curr_file.get_last_mod_time () > old_file.get_last_mod_time ())
      {
          curr_file.set_status (MODIFIED);
          mod_files.push_back (curr_file);
      }
      
  }

  new_files = get_new_files(prev_version);

  for (auto it:new_files)
    {
      if (it.get_path().string().find(".backup_util") != std::string::npos) continue;

      it.set_status(ADDED);
      mod_files.push_back (it);
    }
    
  return mod_files;
}


json dir_struct::dump_dir_struct ()
{
  json obj;

  obj["dir_size"] = this->dir_size;
  obj["loc"] = this->loc.string();
  obj["files"] = json::array();
  obj["mod_files"] = json::array();
  obj["commit_message"] = this->commit_message;
  obj["commit_time"] = this->commit_time;

  for(auto it: this->files)
    obj["files"].push_back(it.dump_file_data());
  for(auto it: this->mod_files)
    obj["mod_files"].push_back(it.dump_file_data());

  return obj;
}

dir_struct :: dir_struct (json obj)
{
  this->dir_size = obj["dir_size"];
  this->loc = (string) obj["loc"];
  this->commit_message = (string) obj["commit_message"];  
  this->commit_time = (time_t) obj["commit_time"];
  this->files = vector <file_data> ();
  this->mod_files = vector <file_data> ();

  for(auto it: obj["files"])
    {
      this->files.push_back(file_data(it, 0));
    }
  for(auto it: obj["mod_files"])
    {
      this->mod_files.push_back(file_data(it, 0));
    }
  return;
}
