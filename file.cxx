#include "file.hxx"
#include <boost/filesystem/operations.hpp>
#include <system_error>

/* Default constructor*/
file_data :: file_data()
{
    this->empty = true;
    this->file_size = -1;
    this->status = -1;
    this->last_mod_time = time(0);
    this->loc = fs::path ("./");
    return;
}

/* Parameterized constructor */
file_data   ::  file_data (fs::path loc)
{
    std::error_code             ec;
    
    this->empty = fs::is_empty (loc);
        
        
    if (is_regular_file(loc))        // is p a regular file?
        {
            this->file_size = fs::file_size (loc);
            this->status = -1;
        }
    else
        {
            this->file_size = 0;
            this->status = 0;
        }
    
    this->last_mod_time = fs::last_write_time (loc);
    this->loc = loc;
    
    return;
}

/* Constructor that loads data from json */
file_data :: file_data (json obj, int x)
{
    this->empty = obj["empty"];
    this->file_size = obj["file_size"];
    this->status = obj["status"];
    this->last_mod_time = obj["last_mod_time"];
    this->loc = fs::path ((string)obj["path"]);
    return;
}

/* Returns the object as json */
json file_data::dump_file_data ()
{
    json obj;
    obj["empty"] = this->empty;
    obj["path"] = this->loc.string();
    obj["file_size"] = this->file_size;
    obj["status"] = this->status;
    obj["last_mod_time"] = this->last_mod_time;
    
    return obj;
}

/* Returns the file_size */
intmax_t file_data :: get_file_size ()
{
    return this->file_size;
}

/* Returns the path of the object */
fs::path file_data :: get_path ()
{
    return this->loc;
}

/* Return the last modified time*/
time_t file_data :: get_last_mod_time()
{
    return this->last_mod_time;
}


void file_data :: set_status (int status)
{
    this->status = status;
    return;
}

int file_data :: get_status ()
{
    return status;
}