#include "file.hxx"
#include <system_error>
namespace fs = boost::filesystem;

/* Constructor */
file_data   ::  file_data (path loc)
{
    std::error_code             ec;
    
    if((this->empty = fs::is_empty (loc)))
        return;
        
    if (is_regular_file(loc))        // is p a regular file?
        {
            this->file_size = fs::file_size (loc);
            this->status = 1;
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

/* Returns the file_size */
intmax_t file_data :: get_file_size ()
{
    return this->file_size;
}

/* Returns the path of the object */
path file_data :: get_path ()
{
    return this->loc;
}

/* Return the last modified time*/
time_t file_data :: get_last_mod_time()
{
    return this->last_mod_time;
}