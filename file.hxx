#include <bits/stdc++.h>
#include <boost/filesystem.hpp>
#include "json.hxx"

using namespace std;
using namespace boost::filesystem;
using json = nlohmann::json;


enum file_status {
    ADDED,
    MODIFIED,
    DELETED
};

class file_data {

    bool                empty;                  /* TRUE if the file is empty */
    intmax_t            file_size;              /* for storing file size */
    path                loc;                    /* Returns the path of the file */
    int                 status;                 /* status of the file*/
    time_t              last_mod_time;          /* for storing last modified time of the files */
    public:
    
    file_data           ();                     /* Default constructor*/
    file_data           (path loc);             /* Parameterized constructor */
    file_data           (json obj);             /* Constructor that loads data from json */
    
    json                dump_file_data ();      /* Returns the object as json */
    intmax_t            get_file_size ();       /* Returns the file size */
    path                get_path ();            /* Returns the path of the file */
    time_t              get_last_mod_time ();   /* Returns the last modified time */
    void                get_updated ();         /* Updates the data of the file */
    void                set_status (int status);           /* Sets status of the file */
    int                 get_status();
};