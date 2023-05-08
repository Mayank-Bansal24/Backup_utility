#include "remote_util.hxx"

int remoteutil(int argc, vector<string> argv) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    } else if (pid == 0) {
    char* command = "python3";
    char* proc_args[10];
    vector<string> argument_list;
    proc_args[0] = "python3";
    proc_args[1] = "Firebase.py";
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