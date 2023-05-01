#include <bits/stdc++.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

class file_data {

    bool                empty;                  /* TRUE if the file is empty */
    intmax_t            file_size;              /* for storing file size */
    path                loc;                    /* Returns the path of the file */
    int                 status;                 /* status of the file*/
    time_t              last_mod_time;          /* for storing last modified time of the files */

    public:

    file_data           (path loc);             /* Constructor */
    
    intmax_t            get_file_size();        /* Returns the file size */
    path                get_path ();            /* Returns the path of the file*/
    time_t              get_last_mod_time ();   /* Returns the last modified time */
};