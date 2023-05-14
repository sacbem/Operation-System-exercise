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

vector<int> refList;
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
        refList.push_back(rand() % PAGE_RANGE);
    }
}
void PrintPageList(){
    cout<<"Reference list: ";
    for (auto i : refList){
        cout<< i << " ";
    }
    cout<< endl;
}
int Queue::OPT() {
    int pgFault = 0; 

    for (int n = 0; n < refList.size(); n++) {
        if (data.size() < STAGING_QUEUE_SIZE) { 
            if (find(data.begin(), data.end(), refList[n]) == data.end()) { 
                data.push_back(refList[n]); 
                pgFault++; 
            }
        }
        else { 
            if (find(data.begin(), data.end(), refList[n]) == data.end()) { 
                for (int i = n + 1; i < refList.size(); i++) {
                    auto it = find(data.begin(), data.end(), refList[i]);
                    if (it != data.end()) {
                        hisroty_patten.push_back(distance(refList.begin() + n, it));
                    }
                    else {
                        hisroty_patten.push_back(ACCESS_DINED);
                    }
                }
                auto maxIndex = max_element(hisroty_patten.begin(), hisroty_patten.end()) - hisroty_patten.begin();
                data[maxIndex] = refList[n]; 
                pgFault++; 
            }
        }
    }

    return pgFault;
}

int main (){

    Queue q;
    int res;
    CreatePageList();
    PrintPageList();

    res = q.OPT();
    q.Print();
    cout<<"pgFault :"<<res<<endl;
    
    return 0 ;
}