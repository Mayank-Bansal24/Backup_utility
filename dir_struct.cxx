#include "dir_struct.hxx"

void dir_struct :: add_file (path p)
{
  file_data*      new_file;

  new_file = new file_data (p);
  this->files.push_back (*new_file);

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

void dir_struct :: get_files_from_dir (path p)
{
   vec v;                                             
   
   copy (directory_iterator(p), directory_iterator(), back_inserter(v));
   for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
   {
     if (is_regular_file (*it))
       add_file (*it);
     else 
       get_files_from_dir(*it);
   }

  return;
}

dir_struct :: dir_struct (path p)
{
  try
  {
    if (exists(p))    
    {
      if (is_regular_file(p))        
        {
          cout << "Path must be a directory not a file.\n" << '\n';   
        }  
      else if (is_directory (p))      
        {
          get_files_from_dir (p);
          this->dir_size  = get_dir_size();
        }
      else
        cout << p << " exists, but is not a directory\n";
    }
    else
      cout << p << "Path does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return;
}



