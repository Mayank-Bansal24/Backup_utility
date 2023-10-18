#include "main.hxx"
#include "log.hxx"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>

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
    fs::create_directories("./.backup_util/jsons/");
    // cout<<argc;
    pid_t pid = fork();


    if (pid == -1) 
    {
        // failed to fork
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    }
    else if (pid == 0) 
    {
        // child case
        std::vector<char*> c_args;
         c_args.push_back(const_cast<char*>("python3"));
        c_args.push_back(const_cast<char*>("/usr/local/bin/Python-Firebase/Firebase.py"));
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
        // parent 
        int status;
        wait(&status);
        if (WIFEXITED(status)) 
        {
            fs::path path("./.backup_util/jsons/");
    int count = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".json") {
            count++;
        }
    }
    if(argc==1 && count<2) {
        pid_t pid = fork();
        if (pid == -1) {
            // fork() falied
            std::cerr << "Failed to fork process" << std::endl;
            return 1;
        } 
        else if (pid == 0) {
            // child process
        char* command = "cp";
        string src;
        cout<<"Enter the source of service Account key"<<endl;
        cin>>src;
        char* srcf=&src[0];
        char* proc_args[] = {"cp",srcf,"./.backup_util/jsons/"};
        
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
        cout<<"These are newly added files.\n\n";
        for (auto ele:add_file){
            cout<<"\033[0;32m"<<ele.get_path().string()<<"\033[0m"<<"\n";
        }
        cout<<"\n";
    }
    else{
        cout<<"There are no newly added files.\n\n";
    }
    if(mod_file.size()){
        cout<<"These files are modified and are ready to be commited.\n\n";
        for (auto ele:mod_file){
            cout<<"\033[0;35m"<<ele.get_path().string()<<"\033[0m"<<"\n";
        }
        cout<<"\n";
    }
    else{
        cout<<"There are no modified files.\n\n";
    }
    if(del_file.size()){
        cout<<"These files are deleted.\n\n";
        for (auto ele:del_file){
            cout<<"\033[0;31m"<<ele.get_path().string()<<"\033[0m"<<"\n";
        }
        cout<<"\n";
    }
    else{
        cout<<"There are no deleted files.\n\n";
    }
    
    /* Return if true if everything happen happily*/
    return true;
}

void backup_util :: add_dir_version (dir_struct *curr_version)
{
    /* Create a version dir and save curr_obj and files*/
    curr_version->save_files(this->version_list.size());
    this->version_list.push_back (*curr_version);

    // Conert to json
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

    // -m is for messgage  
    // command should be like git commit -m "Files to be comitted" .
    if (args.size() < 4 || args[2] != "-m")
    {
        cout << "Invalid arguments\n";
        cout<< "Enter the commit message\n";
        return false;
    }

    dir_struct* curr_status = new dir_struct (loc, log);
    curr_status->set_commit_message(args[3]);
    curr_status->set_commit_time(time(NULL));

    vector <file_data> mod_files =  curr_status->get_mod_files(last_ver.get_files());

    curr_status->set_mod_files (mod_files);
    add_dir_version (curr_status);
    cout<<"Version"<<this->version_list.size()-1<<" created"<<"\n";
    
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
        cout << "\033[0;33m" << "Version no: " << version_no << "\033[0m" << "\n";
        cout << "\033[0;33m" << "Commit Message: " << it.get_commit_message() << "\033[0m" << "\n";

        time_t commit_time = it.get_commit_time();
        struct tm t = *localtime(&commit_time);
        
        cout << "\033[0;34m" << "Time: " <<t.tm_mday<<"/"<<t.tm_mon+1<<"/"<<t.tm_year+1900<<"::"<<t.tm_hour<<":"<<t.tm_min<<":"<<t.tm_sec<<"\033[0m"<<"\n\n";
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
    vector<string>      args;           // To store argv's in form of vector<string>
    logger              *log;           // To track previous logs and add new ones 
    bool                status;         // Check current status of the process
    log = new logger();                 // Initialize the status

    args =  process_args(argc, argv);   //  function to store argv's in form of vector<string>
    // log->set_flags(args[2]);   /// TO be fixed
    
    /* Checking if the path is provided or not*/
    if (argc < 2 )
    {
        // cout << "Please provide the path for the directory to be in backup!!" << "\n";
        log->print ("Please provide the path for the directory to be in backup!!", ERROR);
        /* Show help page along with steps to run it */
        cout<<"1. Use \"init\" command to initialize the directory.\n";
        cout<<"2. Use \"commit\" command to commit all the file of the directory.\n";
        cout<<"3. Use \"restore\" command to restore the previous version.\n";
        cout<<"4. Use \"status\" command to view the status of all the files of the directory.\n";
        cout<<"5. Use \"log\" command to view the commit history.\n";
        cout<<"6. Use \"push\" command to push the folder to the firebase.\n";
        /* Show Help*/
        return 0;
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
        // instance = new backup_util();
        // instance->load_backup_util ();
        // status = status & instance->add (instance->get_last_dir_struct(), args[2]);
        // cout << "Done" << "\n";
    }
    else if (args[1] == "commit")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        log->print ("Running commit call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->commit (instance->get_last_dir_struct(), args);
        if (status) cout<<"All Files Commited Successfully\n";
        log->print ("commit exited with status " + to_string((int)status) + ".", INFORMATION); 
    }
    else if (args[1] == "restore")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        int vn;
        cout<<"Enter the version no. to be restored"<<endl;
        cin>>vn;
        log->print("Running restore call on dir",INFORMATION);
        instance->restore(vn);
        log->print("restore exited",INFORMATION);


    }
    else if (args[1] == "push")
    {
        instance = new backup_util ();
        instance->load_backup_util ();
        log->print("Running push call on dir",INFORMATION);
        instance->push();
        log->print("push exited",INFORMATION);
    }
    else if (args[1] == "status")
    {
        /* Check for existing version*/
        instance = new backup_util();
        instance->load_backup_util();
        log->print ("Running status call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->status (instance->get_last_dir_struct());
        log->print ("status exited with status " + to_string((int)status) + ".", INFORMATION); 

    }
    else if (args[1] == "log")
    {
        instance = new backup_util();
        instance->load_backup_util();
        log->print ("Running log call on dir" + instance->get_path().string() + ".", INFORMATION);
        status = status & instance->git_log ();
        log->print ("log exited with status " + to_string((int)status) + ".", INFORMATION); 
    }
    else
    {
        cout<<"1. Use \"init\" command to initialize the directory.\n";
        cout<<"2. Use \"commit\" command to commit all the file of the directory.\n";
        cout<<"3. Use \"restore\" command to restore the previous version.\n";
        cout<<"4. Use \"status\" command to view the status of all the files of the directory.\n";
        cout<<"5. Use \"log\" command to view the commit history.\n";
        cout<<"6. Use \"push\" command to push the folder to the firebase.\n";
        /* Show Help*/
    }

    return 0;
}