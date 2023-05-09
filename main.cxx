#include "main.hxx"
#include "log.hxx"

vector<string> process_args (int argc, char* argv[])
{   
    vector <string> args;

    for(int i = 0; i < argc; i++)
        args.push_back (argv[i]);
    
    return args;
}

backup_util :: backup_util (fs::path loc, logger* log)
{
    dir_struct          *new_dir;

    this->loc = loc;

    new_dir = new dir_struct (loc, log);
    this->curr_status = *new_dir;
    return;
}

int backup_util :: remoteutil(int argc, vector<string> argv) 
{
    
    cout<<argc;
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    } else if (pid == 0) {
    char* command = "python3";
    char* proc_args[] = {"python3","../Python-Firebase/Firebase.py"};
    for(int i=0;i<argc;i++){
        string k=argv[i];
        proc_args[i+2]=&k[0];

    }
    proc_args[argc+2]=NULL;
    
    
    int status_code = execvp(command,proc_args);

    if (status_code == -1) {
        printf("Process did not terminate correctly\n");
    }
        
        return 0;
    } else {
        // Parent process
       
        int status;
        wait(&status);
        cout << "Waited for child" << '\n';
        if (WIFEXITED(status)) {
            if(argc==1){
            pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    } 
    else if (pid == 0) {
    char* command = "cp";
    string src;
    cout<<"Enter the source of service Account key"<<endl;
    cin>>src;
    char* srcf=&src[0];
    char* proc_args[] = {"cp",srcf,"../Python-Firebase/"};
    
    proc_args[3]=NULL;
    
    
    int status_code = execvp(command,proc_args);

    if (status_code == -1) {
        printf("Process did not terminate correctly\n");
    }
        
        return 0;
    }
            



            else {
        // Parent process
       
        int status;
        wait(&status);
        cout << "Waited for child" << '\n';
        if (WIFEXITED(status)) {
            
            std::cout << "Child process exited with status code " << WEXITSTATUS(status) << std::endl;

        } else {
            std::cerr << "Child process terminated abnormally" << std::endl;
            return 1;
        }
    }
            }

        } else {
            std::cerr << "Child process terminated abnormally" << std::endl;
            return 1;
        }
    }
    

    return 0;
}
void backup_util::set_author_name (string s){
    this->author_name = s;
    return;
}


void backup_util::set_project_name (string s){
    project_name = s;
    return;
}

bool backup_util :: add (void)
{
    return true;
}

bool backup_util :: status (dir_struct  last_ver)
{
    dir_struct* curr_status = new dir_struct (loc, log);

    
    vector<file_data> mod_files = curr_status->get_mod_files(last_ver.get_files());
    
    // Display files changes

    /* Return if true if everything happen happily*/
    return true;
}

bool backup_util :: commit (void)
{
    return true;
    
}

bool backup_util :: restore (void)
{
    return true;
    
}

bool backup_util :: init (vector<string> &args)
{
        string author_name, project_name;
        cout << "Enter author name: ";
        getline(std::cin, author_name);
        cout << "Enter project name: ";
        getline(std::cin, project_name);

        vector<string> ag;
        ag.push_back("fill_details");
        // remoteutil(1,ag);
        fs::create_directories(".backup_util/firebase"); 

        bool status = init_dir_i(args[2], log);

        
    return true;
}

dir_struct backup_util :: get_last_dir_struct()
{
    return this->version_list.back();
}

int main(int argc, char* argv[]) {
    /* Create a Directory to store required files*/
    vector<string>      args; 
    logger              *log;
    bool                status;
    log = new logger();

    args =  process_args(argc, argv);
    // log->set_flags(args[2]);   /// TO be fixed
    
    /* Checking if the path is provided or not*/
    if (argc < 2 )
    {
        cout << "Please provide the path for the directory to be in backup!!" << "\n";
        log->print ("Please provide the path for the directory to be in backup!!", ERROR);
        /* Show help page along with steps to run it */
    }
    
    backup_util* instance = new backup_util (args[2], log);
    /* Load File here*/
    backup_util* last_instance;

    if (args[1] == "init")
    {
        log->print ("Running init call on dir" + args[2] + ".", INFORMATION);
        status = status & instance->init (args);
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 
    } 
    else if (args[1] == "add")
    {

    }
    else if (args[1] == "commit")
    {

    }
    else if (args[1] == "restore")
    {

    }
    else if (args[1] == "status")
    {
        log->print ("Running init call on dir" + args[2] + ".", INFORMATION);
        status = status & instance->status (last_instance->get_last_dir_struct());
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 

    }
    else
    {
        /* Show Help*/
    }

    return 0;
}