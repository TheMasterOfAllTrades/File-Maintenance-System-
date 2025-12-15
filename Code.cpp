#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

class Node{
public:
    int version_id;
    std::string content;
    std::string message; // Empty if not a snapshot
    time_t created_timestamp;
    time_t snapshot_timestamp; // Null if not a snapshot
    Node* parent;
    std::vector<Node*> children;
    bool is_snapshot;

    Node(int v=0, std::string c="", std::string m="", Node* prnt=nullptr, bool snap=false){
        version_id=v;
        content=c;
        message=m;
        created_timestamp=time(0);
        snapshot_timestamp=0;
        parent=prnt;
        is_snapshot=snap;
        std::vector <Node*> children;
    }
};

int hashing(std::string s, int capacity){
    int A=29;
    long long N=100000007;//large prime number
    long long hash=0;
    for(char ch: s){
        hash=(A*hash+(int)ch)%N;
    }
    return hash%capacity;
}

int hashing_ID(int n, int capacity){
    return n%capacity;
}

class HashNode_ID{
public:
    int key;
    Node* value;
    HashNode_ID* next;

    HashNode_ID(int a,Node* b,HashNode_ID* n=nullptr){
    key=a;
    value=b;
    next=n;
    }
};

class HashMap_ID{
public:
    std::vector<HashNode_ID*> arr;
    int capacity;
    int size;

    HashMap_ID(int c=1000){
        capacity=c;
        arr.resize(capacity,nullptr);
        size=0;
    }

    void rehash_ID(){
        int old_length=capacity;
        capacity=capacity*2;
        std::vector<HashNode_ID*> n_arr;
        n_arr.resize(capacity, nullptr);
        for (int i=0;i<old_length;i++){
            HashNode_ID* head=arr[i];
            while (head!=nullptr){
                HashNode_ID* n_node=head->next;
                int n_index=hashing_ID(head->key,capacity);
                head->next=n_arr[n_index];
                n_arr[n_index]=head;
                head=n_node;
            }
        }
        arr=std::move(n_arr);
    }
};

void insertPair_ID(HashMap_ID& Map,int key, Node* value){
    int index=hashing_ID(key,Map.capacity);
    HashNode_ID* head=Map.arr[index];
    if (Map.arr[index]==nullptr){
        Map.arr[index]=new HashNode_ID(key,value);
        Map.size++;
    }
    else{
        while (head->next!=nullptr){
            if (head->key==key){
                head->value=value;
                return;
            }
            head=head->next;
        }
        if (head->key==key){
            head->value=value;
            return;
        }
        head->next=new HashNode_ID(key,value);
        Map.size++;
    }
    if ((double)Map.size/Map.capacity>0.8){
        Map.rehash_ID();
    }
}

Node* get_ID(HashMap_ID& Map,int key){
    int index=hashing_ID(key, Map.capacity);
    HashNode_ID* head=Map.arr[index];
    while (head!=nullptr){
        if (head->key==key){
            return head->value;
        }
        head=head->next;
    }
    return nullptr;
}

bool find_ID(HashMap_ID& Map,int key){
    int index=hashing_ID(key, Map.capacity);
    HashNode_ID* head=Map.arr[index];
    while (head!=nullptr){
        if (head->key==key){
            return true;
        }
        head=head->next;
    }
    return false;
}

class File{
public:
    Node* root; // Your implementation of the tree
    Node* active_version;
    HashMap_ID version_map; // Your implementation of the HashMap
    int total_versions;
    time_t last_modified;
    std::string fname;

    File(std::string name, Node* r=nullptr,Node* a=nullptr, int t=0){
        root=r;
        active_version=a;
        total_versions=t;
        fname=name;
    }
};

class HashNode{
public:
    std::string key;
    File* value;
    HashNode* next;

    HashNode(std::string a,File* b,HashNode* n=nullptr){
    key=a;
    value=b;
    next=n;
    }
};

class HashMap{
public:
    std::vector<HashNode*> arr;
    int capacity;
    int size;

    HashMap(int c=1000){
        capacity=c;
        arr.resize(capacity,nullptr);
        size=0;
    }

    void rehash(){
        int old_length=capacity;
        capacity=capacity*2;
        std::vector<HashNode*> n_arr;
        n_arr.resize(capacity, nullptr);
        for (int i=0;i<old_length;i++){
            HashNode* head=arr[i];
            while (head!=nullptr){
                HashNode* n_node=head->next;
                int n_index=hashing(head->key,capacity);
                head->next=n_arr[n_index];
                n_arr[n_index]=head;
                head=n_node;
            }
        }
        arr=std::move(n_arr);
    }
};

void insertPair(HashMap& Map,std::string key, File* value){
    int index=hashing(key,Map.capacity);
    HashNode* head=Map.arr[index];
    if (Map.arr[index]==nullptr){
        Map.arr[index]=new HashNode(key,value);
        Map.size++;
    }
    else{
        while (head->next!=nullptr){
            if (head->key==key){
                head->value=value;
                return;
            }
            head=head->next;
        }
        if (head->key==key){
            head->value=value;
            return;
        }
        head->next=new HashNode(key,value);
        Map.size++;
    }
    if ((double)Map.size/Map.capacity>0.8){
        Map.rehash();
    }
}

File* get(HashMap& Map,std::string name){
    int index=hashing(name, Map.capacity);
    HashNode* head=Map.arr[index];
    while (head!=nullptr){
        if (head->key==name){
            return head->value;
        }
        head=head->next;
    }
    return nullptr;
}

bool find(HashMap& Map,std::string name){
    int index=hashing(name, Map.capacity);
    HashNode* head=Map.arr[index];
    while (head!=nullptr){
        if (head->key==name){
            return true;
        }
        head=head->next;
    }
    return false;
}

HashMap fileobjects;
std::vector<std::string> folder;
std::vector<File*> fileref;

void heapify(std::vector <File*>& arr, int n, int i){
    // building a max heap by exchanging the elements with the left/right node
    int m=i;
    int l=2*i+1;
    int r=2*i+2;
    if (l<n && (arr[m]->last_modified)<(arr[l]->last_modified) ){
        m=l;
    }
    if (r<n && (arr[m]->last_modified)<(arr[r]->last_modified) ){
        m=r;
    }
    if (m!=i){
        std::swap(arr[i],arr[m]);
        heapify(arr,n,m);
    }
}

void heapify2(std::vector <File*>& arr, int n, int i){
    // building a max heap by exchanging the elements with the left/right node
    int m=i;
    int l=2*i+1;
    int r=2*i+2;
    if (l<n && (arr[m]->total_versions)<(arr[l]->total_versions) ){
        m=l;
    }
    if (r<n && (arr[m]->total_versions)<(arr[r]->total_versions) ){
        m=r;
    }
    if (m!=i){
        std::swap(arr[i],arr[m]);
        heapify2(arr,n,m);
    }
}

void create(std::string name){
    if (name.empty()){
        std::cout<<"File Name cannot be empty"<<std::endl;
        return ;
    }
    File* myfile= new File(name); //creates the file object
    insertPair(fileobjects,name,myfile); //inserts the name,file object pointer in the map
    fileref.push_back(myfile);
    //creates the root node and initialises all the member varaibles
    Node* n= new Node();
    n->version_id=0;
    n->content="";
    n->message="Initially Snapshotted";
    n->created_timestamp=time(0);
    n->snapshot_timestamp=time(NULL);
    n->parent=nullptr;
    n->is_snapshot=true;
    //links the root to the tree(into the file)
    myfile->root=n;
    myfile->active_version=n;
    myfile->fname=name;
    myfile->total_versions=1;
    insertPair_ID(myfile->version_map,0,n);
    myfile->last_modified=time(0);
}

void read(std::string name){
    File* myfile=get(fileobjects,name);
    if ((myfile->active_version->content).empty()){
        std::cout<<"File has no contents to read"<<std::endl;
    }
    else{
        std::cout<<myfile->active_version->content<<std::endl;
    }
}

void insert(std::string name, std::string cont){
    File* myfile=get(fileobjects,name);
    if (!myfile->active_version->is_snapshot){
        myfile->active_version->content.append(cont);
    }
    else{
        Node* n=new Node();
        n->content.append(cont);
        n->created_timestamp=time(0);
        n->is_snapshot=false;
        n->version_id=myfile->total_versions;
        n->parent=myfile->active_version;
        myfile->active_version->children.push_back(n);
        myfile->active_version=n;
        insertPair_ID(myfile->version_map,myfile->total_versions,n);
        myfile->total_versions=myfile->total_versions+1;
    }
    myfile->last_modified=time(0);
}

void update(std::string name, std::string cont){
    File* myfile=get(fileobjects,name);
    if (!myfile->active_version->is_snapshot){
        myfile->active_version->content=cont;
    }
    else{
        Node* n=new Node();
        n->content=cont;
        n->created_timestamp=time(0);
        n->is_snapshot=false;
        n->version_id=myfile->total_versions;
        n->parent=myfile->active_version;
        myfile->active_version->children.push_back(n);
        myfile->active_version=n;
        insertPair_ID(myfile->version_map,myfile->total_versions,n);
        myfile->total_versions=myfile->total_versions+1;
    }
    myfile->last_modified=time(0);
}

void snapshot(std::string name, std::string mssg){
    File* myfile=get(fileobjects,name);
    if (myfile->active_version->is_snapshot){
        std::cout<<"The Version is already a Snapshot."<<std::endl;
        return ;
    }
    myfile->active_version->is_snapshot=true;
    myfile->active_version->message=mssg;
    myfile->active_version->snapshot_timestamp=time(0);
}

void rollBack(std::string name, int versionID=-1){
    File* myfile=get(fileobjects,name);
    if (versionID > (int)myfile->total_versions){
        std::cout<<"Error: INVALID VERSION ID passed through"<<std::endl;
        return ;
    }
    if (versionID==-1){
        if (myfile->active_version->parent!=nullptr){
            myfile->active_version=myfile->active_version->parent;
        }
        else{
            std::cout<<"Already at the root of the tree, no Parent to ROLLBACK"<<std::endl;
        }
    }
    else{
       if (find_ID(myfile->version_map,versionID)){
            myfile->active_version=get_ID(myfile->version_map,versionID);
       }
       else{
            std::cout<<"INVALID VERSION_ID passed"<<std::endl;
       }
    }
}

void history(std::string name){
    File* myfile=get(fileobjects,name);
    Node* present= myfile->active_version;
    std::cout<<"Version ID | Message | TimeStamp"<<std::endl;
    while (present){ //chronologically prints all the information in the order from the active node to the root of the tree
        if (present->is_snapshot){
            std::cout<<present->version_id<<" | "<<present->message<<" | "<<ctime(&present->snapshot_timestamp);
        }
        present=present->parent;
    }
}

void recentFiles(int num){
    if (num<1) {std::cout<<"Error: Number passed should be positive."<<std::endl;return ;}
    int n=fileref.size();
    if (num>n) num=n;
    std::vector<File*> heap=fileref;
    std::cout<<"File Names in Descending Order of their LAST MODIFICATION TIMES:"<<std::endl;
    for (int i=n/2-1;i>=0;i--){
            heapify(heap,n,i);
    }
    while (num){
        n=heap.size();
        std::cout<<heap[0]->fname<<std::endl;
        std::swap(heap[0],heap[n-1]);
        heap.pop_back();
        heapify(heap,n-1,0);
        num--;
    }
}

void biggestTrees(int num){
    if (num<1) {std::cout<<"Error: Number passed should be positive."<<std::endl;return ;}
    int n=fileref.size();
    if (num>n) num=n;
    std::vector<File*> heap=fileref;
    std::cout<<"File Names in Descending Order of number of VERSION NODES:"<<std::endl;
    for (int i=n/2-1;i>=0;i--){
        heapify2(heap,n,i);
    }
    while (num){
        n=heap.size();
        std::cout<<heap[0]->fname<<std::endl;
        std::swap(heap[0],heap[n-1]);
        heap.pop_back();
        heapify2(heap,n-1,0);
        num--;
    }
}

int main(){
    std::cout<<"Enter the Commands, once all commands have been entered, Enter EXIT to terminate"<<std::endl;
    std::string command;
    while (true){
        std::getline(std::cin,command);
        std::istringstream ite(command);
        std::string action, name;
        ite>>action;
        if (action=="EXIT"){
            break;
        }
        if (command==""){
            std::cout<<"INVALID COMMAND"<<std::endl;
            continue;
        }
        if (action=="RECENT_FILES"){
            int number;
            ite>>number;
            recentFiles(number);
            continue;
        }
        if (action=="BIGGEST_TREES"){
            int number;
            ite>>number;
            biggestTrees(number);
            continue;
        }
        ite>>name;
        std::string content;
        std::getline(ite,content);
        if (!content.empty()){
            content.erase(content.begin());
        }
        if (action=="CREATE"){
            if (!find(fileobjects,name)){
                folder.push_back(name);
                create(name);
            }
            else{
                std::cout<<"File Name already exists"<<std::endl;
            }
        }
        if (action=="READ"){
            if (find(fileobjects,name)){
                read(name);
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
        if (action=="INSERT"){
            if (content.empty()){
                std::cout<<"Warning: INSERT Function called without any content"<<std::endl;
            }
            if (find(fileobjects,name)){
                insert(name,content);
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
        if (action=="UPDATE"){
            if (content.empty()){
                std::cout<<"Warning: UPDATE Function called without any content"<<std::endl;
            }
            if (find(fileobjects,name)){
                update(name,content);
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
        if (action=="SNAPSHOT"){
            if (content.empty()){
                std::cout<<"Warning: Snapshot message empty, Snapshot still saved"<<std::endl;
            }
            if (find(fileobjects,name)){
                snapshot(name,content);
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
        if (action=="ROLLBACK"){
            if (find(fileobjects,name)){
                if (content!=""){
                    try{
                        int ID=stoi(content);
                        rollBack(name,ID);
                    }catch(std::invalid_argument&){
                        std::cout<<"INVALID INPUT FORMAT FOR VERSION ID, PLS TRY AGAIN"<<std::endl;
                    }
                }
                else{
                    rollBack(name);
                }
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
        if (action=="HISTORY"){
            if (find(fileobjects,name)){
                history(name);
            }
            else{
                std::cout<<"File doesn't exist"<<std::endl;
            }
        }
    }
}
