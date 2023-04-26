#include "main.hxx"

int main (int argc, char* argv[])
{
    
    bool status = init_dir (argv[1]);    
    
    return !status;
}