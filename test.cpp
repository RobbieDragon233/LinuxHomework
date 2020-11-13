#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
using namespace std;

int main(){
    
    int fd_of_index = open("index", O_RDONLY);
    int fd_of_list = open("a.txt", O_RDONLY);

    int64_t a;
    cout<<"List nums"<<endl;
    for(int i=0;i<15;i++){
        lseek(fd_of_list,i * sizeof(a), SEEK_SET);
        read(fd_of_list, &a, sizeof(a));
        cout<<a<<endl;
    }
    cout<<"Index nums"<<endl;
    for(int i=0;i<10;i++){
        lseek(fd_of_index,i * sizeof(a), SEEK_SET);  
        read(fd_of_index, &a, sizeof(a));
        cout<<a<<endl;
    }
    
    
}