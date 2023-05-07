#include <stdio.h>
#include <iostream>
#include <Python.h>

using namespace std;

int main(int argc,char* argv[])
{
	Py_Initialize();
    PyObject *name , *load_module ,*func ,*call_func ,*args;
    name = PyUnicode_FromString((char*)"Firebase");
    load_module=PyImport_Import(name);
    
    string cmd=argv[1];
    if (cmd=="fill_details"){
        func=  PyObject_GetAttrString(load_module,(char*)"fill_details");
    }
    else if (argv[1]=="initialize"){
        func=  PyObject_GetAttrString(load_module,(char*)"initialize");

    }
    else if (argv[1]=="delete_file"){
        func=  PyObject_GetAttrString(load_module,(char*)"delete_file");

    }
    else if (argv[1]=="download_file"){
        func=  PyObject_GetAttrString(load_module,(char*)"download_file");

    }
    else if (argv[1]=="upload_file"){
        func=  PyObject_GetAttrString(load_module,(char*)"upload_file");

    }
    else{
        func=  PyObject_GetAttrString(load_module,(char*)"geturl");
        // cout<<1<<endl;
    }
    cout<<"func done"<<endl;
    // call_func=PyObject_CallObject(func,NULL);
    // cout<<call_func<<endl;


	return 0;
}