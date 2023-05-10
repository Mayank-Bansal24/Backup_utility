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
void logger::print(string info,int type){
    time_t tmNow;
    tmNow = time(NULL);
    struct tm t = *localtime(&tmNow);
    bool flag=false;
    // log_file("temp.txt", std::ofstream::app,std::ios_base::app);
    std::ofstream log_file;
    log_file.open("temp.txt", std::ios_base::app); // append instead of overwritex
    for (int i=0;i<(int)flag_req.size();i++){
        if((flag_req[i]=='E' && type==0) || (flag_req[i]=='W' && type==1) || (flag_req[i]=='D' && type==2) || (flag_req[i]=='I' && type==3)){
            flag=true;
        }
    }
        map<int,char> mp;
        mp[0]='E';
        mp[1]='W';
        mp[2]='D';
        mp[3]='I';
    if(type==0 || type==1 || flag){
       log_file<<mp[type]<<" : "<<t.tm_mday<<"/"<<t.tm_mon+1<<"/"<<t.tm_year+1900<<"::"<<t.tm_hour<<":"<<t.tm_min<<":"<<t.tm_sec<<" : "<<info<<"\n";
    }
} 
