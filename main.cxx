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
    cout << loc.string().size() << "\n";
    new_dir = new dir_struct (loc, log);
    this->curr_status = *new_dir;
    this->version_list.push_back (*new_dir);

    std::ifstream ifs(loc.string());
    json obj = json::parse(ifs); 
    this->author_name = obj["author_name"];
    this->loc = (string)obj["loc"];
    this->project_name = obj["project_name"];
    this->version_list = vector<dir_struct> ();

    for (auto it : obj["version_list"])
    {
        dir_struct dir(it);
        this->version_list.push_back(dir);
    }
    return;
}

int backup_util :: remoteutil(int argc, vector<string> argv) 
{
    
    // cout<<argc;
    pid_t pid = fork();


    if (pid == -1) 
    {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    }
    else if (pid == 0) 
    {
        std::vector<char*> c_args;
         c_args.push_back(const_cast<char*>("python3"));
        c_args.push_back(const_cast<char*>("../Python-Firebase/Firebase.py"));
        for (const auto& arg : argv) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
    c_args.push_back(nullptr); 
        int status_code = execvp(c_args[0], c_args.data());
        if (status_code == -1) 
        {
            printf("Process did not terminate correctly\n");
        }
    } 
    else 
    {
        int status;
        wait(&status);
        if (WIFEXITED(status)) 
        {
            fs::path path("../Python-Firebase/");
    int count = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".json") {
            count++;
        }
    }
            if(argc==1 && count<2){
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

bool backup_util :: add (dir_struct  last_ver)
{
    dir_struct* curr_status = new dir_struct (loc, log);

    vector <file_data> mod_files =  curr_status->get_mod_files(last_ver.get_files());
    
    
    return true;
}

bool backup_util :: status (dir_struct  last_ver)
{
    dir_struct* curr_status = new dir_struct (loc, log);

    vector<file_data> mod_files = curr_status->get_mod_files(last_ver.get_files());
    vector<file_data> add_file;
    vector<file_data> mod_file;
    vector<file_data> del_file;

    for (file_data ele:mod_files){
        int k=ele.get_status();
        if(k == ADDED){
            add_file.push_back(ele);
        }
        else if (k == MODIFIED){
            mod_file.push_back(ele);
        }
        else{
            del_file.push_back(ele);
        }
    }
        if(add_file.size()){
            cout<<"These are newly added files\n";
            for (auto ele:add_file){
                cout<<ele.get_path()<<"\n";
            }
        }
        if(mod_file.size()){
            cout<<"These files are modified use add command to commit\n";
            for (auto ele:mod_file){
                cout<<ele.get_path()<<"\n";
            }
        }
        if(del_file.size()){
            cout<<"These files are deleted\n";
            for (auto ele:del_file){
                cout<<ele.get_path()<<"\n";
            }
        }
    
    /* Return if true if everything happen happily*/
    return true;
}

void backup_util :: add_dir_version (dir_struct *curr_version)
{
    /* Create a version dir and save curr_obj and files*/
    curr_version->save_files(this->version_list.size());
    this->version_list.push_back (*curr_version);

    this->dump_backup_util();
    /* Upload data */
    /* Other things to do*/
}

bool backup_util :: push(){
    
    int total=this->version_list.size()-1;
    int cpv=this->pushed_version;
    for (int i=cpv+1;i<=total;i++){
        vector<string> arguments;
    arguments.push_back("upload_file");
    arguments.push_back(this->project_name);
    arguments.push_back(to_string(i)+".tar.gz");
    arguments.push_back("./.backup_util/versions/");
    arguments.push_back("files_backup"+to_string(i)+".tar.gz");
    remoteutil(5,arguments);
    }
    this->pushed_version=total;
    this->dump_backup_util();
}

bool backup_util :: commit (dir_struct last_ver, vector<string> &args)
{
    string commit_message = "";

    if (args.size() < 4 || args[2] != "-m")
    {
        cout << "Invalid arguments\n";
        return false;
    }

    dir_struct* curr_status = new dir_struct (loc, log);
    curr_status->set_commit_message(args[3]);
    curr_status->set_commit_time(time(NULL));

    vector <file_data> mod_files =  curr_status->get_mod_files(last_ver.get_files());

    curr_status->set_mod_files (mod_files);
    add_dir_version (curr_status);

    
    return true;
}

bool backup_util :: restore (int vn)
{
    if(vn>this->version_list.size()-1){
        cout<<"Version doesn't exists"<<endl;
    }
    else if(vn>this->pushed_version){
        cout<<"First push the Versions"<<endl;
    }
    else{
        cout<<this->pushed_version<<endl;
        for (int i=1;i<=vn;i++)
        {
                vector<string> arguments;
                arguments.push_back("download_file");
                arguments.push_back(this->project_name);
                arguments.push_back(to_string(i)+".tar.gz");
                arguments.push_back("./.backup_util/versions/");
                arguments.push_back("files_backup"+to_string(i)+".tar.gz");
                remoteutil(5,arguments);
                this->version_list[i].load_files(i);

        }
        // this->version_list[1].load_files(1);
    }
    return true;
    
}

bool backup_util :: git_log (void)
{
    int version_no = -1;
    for (auto it: this->version_list)
    {
        version_no++;
        if (version_no == 0) continue;
        cout << "Version no: " << version_no << "\n";
        cout << "Commit Message: " << it.get_commit_message() << "\n";

        time_t commit_time = it.get_commit_time();
        struct tm t = *localtime(&commit_time);
        
        cout << "Time: " <<t.tm_mday<<"/"<<t.tm_mon+1<<"/"<<t.tm_year+1900<<"::"<<t.tm_hour<<":"<<t.tm_min<<":"<<t.tm_sec<<"\n\n";
    }

    return true;
}


bool backup_util :: init (vector<string> &args)
{
        bool status = init_dir_i(args[2], log);
        string author_name, project_name;
        cout << "Enter author name: ";
        getline(std::cin, author_name);
        this->set_author_name(author_name);
        this->pushed_version=0;
        cout << "Enter project name: ";
        getline(std::cin, project_name);
        this->set_project_name(project_name);

        dir_struct *empty_dir = new dir_struct ();
        this->version_list = { *empty_dir };
        this->loc = fs::path(args[2]);
        this->dump_backup_util();

        vector<string> ag;
        ag.push_back("fill_details");
        remoteutil(1,ag);
        ag.clear();
        ag.push_back("initialize");
        ag.push_back(project_name);
        remoteutil(2,ag);
        fs::create_directories(".backup_util/firebase"); 


    return true;
}

dir_struct backup_util :: get_last_dir_struct()
{
    return this->version_list[version_list.size()-1];
}



void backup_util :: dump_backup_util ()
{
    json obj;
    obj["author_name"] = this->author_name;
    obj["loc"] = this->loc.string();
    obj["project_name"] = this->project_name;
    obj["version_list"] = json::array();
    obj["pushed_version"]=this->pushed_version;

    for (auto it : this->version_list)
        obj["version_list"].push_back(it.dump_dir_struct());

    std::ofstream ofs("./.backup_util/versions/state.json");
    ofs << std::setw(4) << obj.dump() << std::endl;
    return;
}

void backup_util :: load_backup_util ()
{
    // read json from given path
    std::ifstream ifs("./.backup_util/versions/state.json");
    json obj = json::parse(ifs); 
    this->author_name = obj["author_name"];
    this->loc = (string)obj["loc"];
    this->project_name = obj["project_name"];
    this->pushed_version=obj["pushed_version"];
    this->version_list = vector<dir_struct> ();

    for (auto it : obj["version_list"])
    {
        dir_struct dir(it);
        this->version_list.push_back(dir);
    }

    return;
}

backup_util :: backup_util ()
{

}

fs::path backup_util:: get_path ()
{
    return this->loc;
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
    
    backup_util* instance;
    /* Load File here*/
    backup_util* last_instance;
    for (auto it:args){
        string a=it;
        if((int)a.size()>5 && a.substr(0,5)=="-log="){
            log->set_flags(a.substr(5, (int)a.size()-5));
        }
    }
    if (args[1] == "init")
    {
        instance = new backup_util();
        log->print ("Running init call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->init (args);
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 
    } 
    else if (args[1] == "add")
    {

    }
    else if (args[1] == "commit")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        log->print ("Running init call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->commit (instance->get_last_dir_struct(), args);
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 
    }
    else if (args[1] == "restore")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        int vn;
        cout<<"Enter the version no to be restored"<<endl;
        cin>>vn;
        instance->restore(vn);
        


    }
    else if (args[1] == "push")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        instance->push();
    }
    else if (args[1] == "status")
    {
        /* Check for existing version*/
        instance = new backup_util();
        instance->load_backup_util();
        log->print ("Running init call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->status (instance->get_last_dir_struct());
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 

    }
    else if (args[1] == "log")
    {
        instance = new backup_util();
        instance->load_backup_util();
        log->print ("Running init call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->git_log ();
        log->print ("init exited with status " + to_string((int)status) + ".", INFORMATION); 
    }
    else
    {
        cout<<"ERROR! Missing Arguments\n";
        cout<<"1. Use Add command to add file to the directory.\n";
        cout<<"2. Use Commit command to commit all the file of the directory.\n";
        cout<<"3. Use Restore command to restore the previous version.\n";
        cout<<"4. Use Status command to view ths status of all the files of the directory.\n";
        /* Show Help*/
    }

    return 0;
}