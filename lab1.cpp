#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <chrono>


using namespace std;

typedef struct {
    int flag;
    char symbol;
}FlagSym;

void* proc1(void* arg) {
    cout << "Start first thread" << endl;
    FlagSym* args = (FlagSym*)arg;
    while (args->flag == 0) {
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
    }
    cout << "End first thread" << endl;
    pthread_exit((void*)1);
}

void* proc2(void* arg) {
    cout << "Start second thread" << endl;
    FlagSym* args = (FlagSym*)arg;
    while (args->flag == 0) {
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
    }
    cout << "End second thread" << endl;
    pthread_exit((void*)2);
}

int main() {
    FlagSym arg1;
    FlagSym arg2;
    arg1.flag = 0;
    arg1.symbol = '1';
    arg2.flag = 0;
    arg2.symbol = '2';
    int error = 0;
    cout << "Program is started..." << endl;
    pthread_t id1, id2;
    error = pthread_create(&id1, NULL, proc1, &arg1);
    if (error != 0) {
        cout << "in First thread create error:" << error << endl;
        return 1;
    }
    error = pthread_create(&id2, NULL, proc2, &arg2);
    if (error != 0) {
        cout << "in Second thread create error:" << error << endl;
        return 2;
    }
    cout << "Please, enter any key" << endl;
    getchar();
    cout << "Key is pressed" << endl;
    int* exitcode;

    arg1.flag = 1;
    arg2.flag = 1;
    error = pthread_join(id1, (void**)&exitcode);
    if (error != 0) cout << "IN  first thread pthread_join return error:" << strerror(error) << endl; else
        printf("exitcode = %p\n", exitcode);
    error = pthread_join(id2, (void**)&exitcode);
    if (error != 0) cout << "IN  second thread pthread_join return error:" << strerror(error) << endl;
    else
        printf("exitcode = %p\n", exitcode);
    printf("End program\n");
    return 0;
}