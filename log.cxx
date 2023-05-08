#include"log.hxx"
#include <time.h>

void logger::set_flags(string s)
{
    this->flag_req = s;
    return;
}

void logger::close_p (){
    log_file.close();
}

void logger::print(string info,char type){
    time_t tmNow;
    tmNow = time(NULL);
    struct tm t = *localtime(&tmNow);
    bool flag=false;
    cout<<flag_req<<endl;
    for (int i=0;i<(int)flag_req.size();i++){
        if(flag_req[i]==type){
            flag=true;
        }
    }
    if(type=='e' || type=='w' || flag){
       log_file<<type<<" : "<<t.tm_mday<<"/"<<t.tm_mon+1<<"/"<<t.tm_year+1900<<"::"<<t.tm_hour<<":"<<t.tm_min<<":"<<t.tm_sec<<" : "<<info<<"\n";
    }
} 
