//
// Created by plihuo on 2022/10/26.
//

#ifndef OPENGLSTUDYDAY3_UTILS_H
#define OPENGLSTUDYDAY3_UTILS_H
#define BACKTRACE_SIZE 16
#include <csignal>
#include <execinfo.h>
#include "iostream"

using namespace std;
class Utils{
public:
    static void registerSignal(){
        signal(SIGSEGV, segv_signal_handler);
        signal(SIGSEGV, segv_signal_handler);
        signal(SIGSEGV, segv_signal_handler);
    }
private:
    static void segv_signal_handler(int signum){

        int nptrs, i;
        void *buffer[BACKTRACE_SIZE] = {0};
        char **strings;

        cout<<"\n>>>>>>>>> Catch Signal " <<signum<<"<<<<<<<<<\n";
        nptrs = backtrace(buffer, BACKTRACE_SIZE);
        strings = backtrace_symbols(buffer, nptrs);
        if (!strings) {
            cout<<"backtrace_symbols() error"<<endl;
            exit(-1);
        }
        cout<<"===== print backtrace begin =====\n"<<endl;
        for (i = 0; i < nptrs; i++) {
            cout<< i << strings[i]<<endl;
        }
        cout<<"===== print backtrace end =====\n\n";
        free(strings);
        exit(-1);
    }
};
#endif //OPENGLSTUDYDAY3_UTILS_H
