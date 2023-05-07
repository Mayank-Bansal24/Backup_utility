#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <filesystem>

int main(int argc,char* argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork process" << std::endl;
        return 1;
    } else if (pid == 0) {
    char* command = "python3";
    char* argument_list[10] = {"python3", "Firebase.py"};
    for(int i=1;i<argc;i++){
        argument_list[i+1]=argv[i];
    }

    int status_code = execvp(command, argument_list);

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