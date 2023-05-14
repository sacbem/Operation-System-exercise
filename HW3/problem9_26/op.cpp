#include <iostream>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <vector>
using namespace std;

#define PAGE_COUNT 10
#define PAGE_RANGE 10
#define STAGING_QUEUE_SIZE 5
#define DEFAULT -1
#define SUCCESS 0
#define ERROR -1
#define ACCESS_DINED 100000

vector<int> ref_list;
void CreatePageList();
void PrintPageList();
typedef class Queue Queue;

class Queue {
    private:
        vector<int> data;
        vector<int> hisroty_patten;
    public:
        Queue();
        void push_back(int item);
        void Print();
        int OPT();
        ~Queue();
};

Queue::Queue(){
    data.reserve(STAGING_QUEUE_SIZE);
    hisroty_patten.reserve(STAGING_QUEUE_SIZE);
}
Queue::~Queue(){
}
void Queue::push_back(int item){
    data.push_back(item);
}
void Queue::Print(){
    cout << "Staging Queue : ";
    for (auto i : data){
        cout<< i << " ";
    }
    cout<< endl;
}
void CreatePageList() {
    srand(time(NULL));
    for (int i = 0; i < PAGE_COUNT; i++) {
        ref_list.push_back(rand() % PAGE_RANGE);
    }
}
void PrintPageList(){
    cout<<"Reference list: ";
    for (auto i : ref_list){
        cout<< i << " ";
    }
    cout<< endl;
}
int Queue::OPT() {
    int pg_fault = 0; 

    for (int n = 0; n < ref_list.size(); n++) {
        if (data.size() < STAGING_QUEUE_SIZE) { 
            if (find(data.begin(), data.end(), ref_list[n]) == data.end()) { 
                data.push_back(ref_list[n]); 
                pg_fault++; 
            }
        }
        else { 
            if (find(data.begin(), data.end(), ref_list[n]) == data.end()) { 
                for (int i = n + 1; i < ref_list.size(); i++) {
                    auto it = find(data.begin(), data.end(), ref_list[i]);
                    if (it != data.end()) {
                        hisroty_patten.push_back(distance(ref_list.begin() + n, it));
                    }
                    else {
                        hisroty_patten.push_back(ACCESS_DINED);
                    }
                }
                auto maxIndex = max_element(hisroty_patten.begin(), hisroty_patten.end()) - hisroty_patten.begin();
                data[maxIndex] = ref_list[n]; 
                pg_fault++; 
            }
        }
    }

    return pg_fault;
}

int main (){

    Queue q;
    int res;
    CreatePageList();
    PrintPageList();

    res = q.OPT();
    q.Print();
    cout<<"pg_fault :"<<res<<endl;
    
    return 0 ;
}