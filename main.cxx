#include "main.hxx"

void find_path(const std::string& file, std::vector<std::string>& path) {
    // implementation of git add function
    std::cout << "Finding path of file: " << file << std::endl;
    fs::path current_path = fs::current_path();
    fs::path file_path = current_path / file;
    if (fs::exists(file_path)) {
        path.push_back(file_path.string());
    } else {
        std::cout << "File not found: " << file << std::endl;
    }
}

vector<string> process_args (int argc, char* argv[])
{   
    vector <string> args;

    for(int i = 0; i < argc; i++)
        args.push_back (argv[i]);
    
    return args;
}

void execute_command(int argc, char* argv[]) {
    std::map<std::string, std::string> command_args;
    int num_args_to_parse = std::min(argc - 1, 2); // only parse first two arguments
    for (int i = 1; i <= num_args_to_parse; ++i) {
        if (i < argc - 1) {
            command_args[argv[i]] = argv[i + 1];
            ++i;
        } else {
            command_args[argv[i]] = "";
        }
    }
    std::vector<std::string> path;
    for (const auto& arg : command_args) {
        std::cout << "Map formed: "<<arg.first << " -> " << arg.second << std::endl;
        if (arg.first == "path") {
            find_path(arg.second, path);
        }
        else {
            std::cout << "Unidentified function" << std::endl;
        }
    }
    for (const auto& p : path) {
        std::cout << "Path: " << p << std::endl;
    }
}

backup_util :: backup_util (fs::path loc, logger* log)
{
    dir_struct          *new_dir;

    this->loc = loc;

    new_dir = new dir_struct (loc, log);
    this->curr_status = *new_dir;
    return;
}

int remoteutil(int argc, vector<string> argv) {
    cout<<argc;
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    } else if (pid == 0) {
    char* command = "python3";
    char* proc_args[10];
    vector<string> argument_list;
    proc_args[0] = "python3";
    proc_args[1] = "./Python-Firebase/Firebase.py";
    for(int i=1;i<argc;i++){
        string n=argv[i];
        proc_args[i+1] = &n[0];
    }

    int status_code = execvp(command,proc_args);

    if (status_code == -1) {
        printf("Process did not terminate correctly\n");
    }

    printf("This line will not be printed if execvp() runs correctly\n");
        
        return 0;
    } else {
        // Parent process
       
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status code " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Child process terminated abnormally" << std::endl;
            return 1;
        }
    }
    

    return 0;
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
            remoteutil(1,ag);
            fs::create_directories(".backup_util/firebase"); 
            bool status = init_dir_i(args[2], log);

            /*
            Empty prev_version file
            Cur_version file
        */
    } 

   
    
    return 0;
}