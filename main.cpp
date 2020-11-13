/*
1.write not one by one
*/
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include "index.h"

using namespace std;

#define list_file "a.txt"
#define index_file "index"

class File{
public:
    File();
    File(int row, int col, int attr);
    bool hasFile();
    bool hasIndex();
    void CreatListFile();
    void CreatIndexFile(int attr);
    void addData(int64_t *data);
    // File(int row, int col);
    // bool IsSuccess();
    // int GetErrorCode();
private:
    int row = 10;
    int col = 5;
    int block = 8;
    int search_num;
    long m_lReturnCode;
    long m_lErrorCode;
    Index index;

public:
    // bool Append(int64_t* data);
    int* Search(int attr, int64_t low, int64_t high);
    int* Search(int attr, int64_t low, int64_t high, int return_num);
    int Search(int row, int col);
};

void File::addData(int64_t *data){
    int fd_of_list = open(list_file, O_APPEND);
    write(fd_of_list, data, sizeof(data);
    index.add(data);
}

File::File(int row, int col, int attr){
    if(this->hasFile()){
        this->row = row;
        this->col = col;
        cout<<"There exist a List File"<<endl;
        //pass
    }else{
        this->row = row;
        this->col = col;
        this->CreatListFile();
    }
    
    if(this->hasIndex()){
        //pass
    }else{
        this->CreatIndexFile(attr);
    }
}

void File::CreatListFile(){
    cout<<"create the List File"<<endl;

    int fd = open(list_file, O_RDWR | O_CREAT);
    int64_t tmp;
    srand(time(0));
    if(fd == -1){
        m_lReturnCode = -1;
        m_lErrorCode = -1;
    }
    for(int i=0;i<this->row;i++){
        for(int j=0;j<this->col;j++){
            tmp = rand();
            write(fd, &tmp,sizeof(tmp));
        }
    }
    close(fd);
    // test can binary file read successful
    // answer is yes
    // so i don't need add 1 byte of blank space 
    // int64_t b;
    // fd = open("a.txt", O_RDONLY);
    // for(int i=0;i<20;i++){
    //     read(fd, &b, sizeof(b));
    //     cout<<b<<endl;
    // }
    // close(fd);
}

bool File::hasIndex(){
    cout<<"judge whether has index file~"<<endl;

    int fd_of_index = open(index_file, O_RDONLY);
    if(fd_of_index != -1){
        return true;
    }else{
        return false;
    }
}

void File::CreatIndexFile(int attr){
    this->index = Index(this->row, this->col, attr);
}

bool File::hasFile(){
    cout<<"judge exist list file~"<<endl;

    int fd_of_list = open(list_file, O_RDONLY);
    if(fd_of_list != -1){
        return true;
    }else{
        return false;
    }
}

int File::Search(int row, int col){
    cout<<"search a value by File"<<endl;

    int fd;
    int64_t tmp, res;
    
    fd = open(list_file, O_RDONLY);
    // //read one by one(tmp));
    // }
    // for(int i=0;i<row-1;i++){
    //     cout<<"row"<<endl;
    //     read(fd, &tmp, sizeof
    // for(int i=0;i<col-1;i++){
    //     cout<<"col"<<endl;
    //     read(fd,&tmp,sizeof(tmp));
    // }

    //use lseek to change
    lseek(fd, (row-1)*(this->col*8)+(col-1)*8, SEEK_SET);
    read(fd,&res,sizeof(res));
    close(fd);
    return res;

}

int* File::Search(int attr, int64_t low, int64_t high){
    // now the policy of search is that the first number is the num of the arry
    // If there exist time, I want to change the return num is the number and add a *res parameter

    int* index_res = this->index.Search(attr, low, high);
    if(index_res[0] == -1){
        cout<<"this attr does't build index"<<endl;
    }else{
        cout<<"solved by index"<<endl;
        return index_res;
    }

    cout<<"search a range by File"<<endl;

    int64_t readNum;
    int point = this->block * (attr - 1) - this->block * this-> col;
    int* res = NULL;
    this->search_num = 1;

    int fd = open(list_file, O_RDONLY);
    res = (int*)malloc(sizeof(int));
    res[0]=0;
    for(int i=0;i<this->row;i++){
        point = point + this->col * this->block;
        // cout<<"point:"<<point<<endl;
        lseek(fd, point, SEEK_SET);
        read(fd, &readNum, this->block);
        if(readNum < high && readNum >=low){
            this->search_num++;
            res = (int*)realloc(res, (this->search_num)*sizeof(int));
            res[this->search_num-1] = i;
        }
    }
    if(this->search_num == 1){
        res[0] = -1;
    }else{
        res[0] = this->search_num;
    }
    close(fd);
    return res;
}

int* File::Search(int attr, int64_t low, int64_t high, int return_num){
    cout<<"search a range by File and ask the return num"<<endl;

    int64_t readNum;
    int point = 0;
    int* res = NULL;
    this->search_num = 1;

    int fd = open(list_file, O_RDONLY);
    res = (int*)malloc(sizeof(int));
    res[0]=0;
    for(int i=0;i<this->row;i++){
        point = point + (i * this->col + attr - 1) * this->block;
        lseek(fd, point, SEEK_SET);
        read(fd, &readNum, this->block);
        if(readNum < high && readNum >=low){
            this->search_num++;
            res = (int*)realloc(res, (this->search_num)*sizeof(int));
            res[this->search_num-1] = i+1;
        }
        if(this->search_num>return_num){
            break;
        }
    }
    res[0] = this->search_num;
    close(fd);
    return res;
}

int main(){
    File a = File(10,1,1);

    int fd = open(list_file, O_RDONLY);
    // int64_t b[10];
    // read(fd, b, sizeof(b));
    // for(int i=0;i<10;i++){
    //     cout<<b[i]<<endl;
    // }

    // int64_t b;
    // cout<<sizeof(b)<<endl;
    // cout<<a.Search(2,7)<<endl;

    cout<<"Begin Search"<<endl;
    int* c = a.Search(1,0,1182245360);
    if(c[0] == -1){
        cout<<"There is no num"<<endl;
    }else{
        cout<<"The number:"<<c[0]<<endl;
        for(int i=1;i<c[0];i++){
            cout<<"!!!"<<endl;
            cout<<c[i]<<endl;
        }
    }
    
    cout<<a.Search(1,1)<<endl;
    // cout<<a.Search(1,5)<<endl;

}