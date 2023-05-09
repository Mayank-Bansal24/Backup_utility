#include "main.hxx"

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
void backup_util::set_author_name(string s){
    this->author=s;
    return;
}


void backup_util::set_project_name(string s){
    project=s;
    return;
}


bool backup_util :: add (void)
{
    return true;
}

bool backup_util :: status (void)
{
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

bool backup_util :: init (void)
{
    return true;

}


int main(int argc, char* argv[]) {
    /* Create a Directory to store required files*/
    vector<string> args; 
    logger         *log;

    log = new logger();

    args =  process_args(argc, argv);
    /* Initializing logger */
    // log->set_flags(args[2]);   /// TO be fixed
    
    /* Checking if the path is provided or not*/
    if (argc < 2 )
    {
        cout << "Please provide the path for the directory to be in backup!!" << "\n";
        log->print ("Please provide the path for the directory to be in backup!!", ERROR);
        /* Show help page along with steps to run it */
    }
    // backup_util* instance = new backup_util();


    if (args[1] == "init")
    {
        /* Script to config project
            Author Name
            Proj_Name
            Remote repo  
            */
            vector<string> ag;
            ag.push_back("fill_details");
            // remoteutil(1,ag);
            fs::create_directories(".backup_util/firebase"); 

            bool status = init_dir_i(args[2], log);

            /*
            Empty prev_version file
            Cur_version file
        */
    } 

   
    else{
        std::string author_name, project_name;
        std::cout << "Enter author name: ";
        std::getline(std::cin, author_name);
        std::cout << "Enter project name: ";
        std::getline(std::cin, project_name);
    }
    bool status = init_dir_i(args[1], log);
    
    return 0;
}