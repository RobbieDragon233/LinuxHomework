#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

#define list_file "a.txt"
#define index_file "index"
// i didn't close the file, so next step is close files and add judge true or false 

// every page is 4kb and one data is 8byte
// so i choose the length of array equals to 512 
class Index{
public:
    //if no para, the first col will be key
    Index();
    Index(int row, int col, int attr);
    ~Index();
    // Index(int key_attr, int row, int col);
    void Creat(int key_attr);
    void Flush(int start, int end);
    void ReadFromDisk();
    bool hasIndex();
    int* Search(int attr, int64_t low, int64_t high);
    void add(int64_t* data);

    void debug(int i);

private:
    int attr = -1;
    int64_t* index=NULL;
    int num_of_index = 10; //need change
    int col=10;
    int row=10;
};

void Index::add(int64_t* data){
    this->num_of_index++;
    this->index = (int64_t*)realloc(this->index, this->num_of_index*sizeof(int64_t));
    this->index[this->num_of_index-1] = data[this->attr];
}

int* Index::Search(int attr, int64_t low, int64_t high){
    int *res = (int*)malloc(sizeof(int));
    if(attr != this->attr){
        cout<<"ask attr is "<<attr<<" and the index attr is "<<this->attr<<endl;
        res[0] = -1;
        return res;
    }
    int len = 1;
    for(int i = 0;i < this->num_of_index; i++){
        if(index[i]>=low && index[i]<high){
            len++;
            res = (int*)realloc(res, (len)*sizeof(int));
            res[len-1] = i;
        }
    }
    res[0] = len;
    return res;
}

bool Index::hasIndex(){
    int fd_of_index = open(index_file, O_RDONLY);
    if(fd_of_index != -1){
        return true;
    }else{
        return false;
    }
}

Index::~Index(){
    // //flush when append, so no need flush now
    // this->Flush()
}

Index::Index(){

}

Index::Index(int row, int col, int attr){
    cout<<"In Index.cpp"<<endl;

    this->row = row;
    this->col = col;
    if(hasIndex()){
        cout<<"there exist index file"<<endl;
        if(attr != this->attr){
            cout<<"The index key has already exist and the attribute is "<<this->attr<<endl;
            cout<<"But we change it for you~"<<endl;
            this->attr = attr;
        }
        ReadFromDisk();
    }else{
        Creat(attr);
    }
    
}

void Index::Creat(int key_attr){
    cout<<"Create the index~"<<endl;

    int fd_of_List = open(list_file, O_RDONLY);
    
    this->attr = key_attr;
    cout<<"I have change the attr to "<<key_attr<<endl;
    // this->index = (int64_t*)malloc(sizeof(int64_t));
    // this->index[0]=1;
    // cout<<index[0]<<endl;

    int64_t a[this->col];
    this->num_of_index = 0;
    lseek(fd_of_List, (this->num_of_index)*(this->col*sizeof(int64_t)), SEEK_SET);
    while(read(fd_of_List, a, this->col*sizeof(int64_t))){
        this->num_of_index++;
        this->index = (int64_t*)realloc(this->index, this->num_of_index*sizeof(int64_t));
        this->index[this->num_of_index-1] = a[key_attr-1];
        lseek(fd_of_List, (this->num_of_index)*(this->col*sizeof(int64_t)), SEEK_SET);
        cout<<(this->num_of_index)*(this->col*sizeof(int64_t))<<endl;
    }
    Flush(0, this->num_of_index);
    // close(fd_of_List);
}

void Index::Flush(int start, int end){
    cout<<"Flush on the disk~"<<endl;

    int fd_of_Index = open(index_file, O_RDWR | O_CREAT | O_APPEND);
    

    write(fd_of_Index, this->index, sizeof(int64_t)*this->num_of_index);
    close(fd_of_Index);

    // fd_of_Index = open(this->index_file, O_RDWR | O_CREAT | O_APPEND);
    
    // // test whether the index write successful
    // int64_t tmp[this->num_of_index];
    // lseek(fd_of_Index, 0, SEEK_SET);
    // read(fd_of_Index, tmp, sizeof(int64_t)*this->num_of_index);
    // cout<<tmp[0]<<endl;
    // for(int i=0;i<this->num_of_index;i++){
    //     cout<<"!"<<tmp[i]<<endl;
    // }

    close(fd_of_Index);
}

void Index::ReadFromDisk(){
    cout<<"read from disk~"<<endl;
    
    int fd_of_Index = open(index_file, O_RDWR | O_CREAT);

    index = (int64_t*)malloc(this->num_of_index*(sizeof(int64_t)));
    read(fd_of_Index, index, this->num_of_index*sizeof(int64_t));
}

void Index::debug(int i){
    cout<<i<<endl;
}

// int main(){

//     Index a = Index(10,5);

//     return 0;
// }
