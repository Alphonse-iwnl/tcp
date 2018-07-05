#include<deque>
#include<vector>
#include<map>
#include<ctime>
#include<pthread.h>
#define MAX 100

class sendwindow{
    std::deque<int> seqdeque;
    std::deque<int>::iterator rptr;
    int maxseq;
    int windowsize;
    int popfinish;
    ~sendwindow();
    public:
    sendwindow(int ack,int windowsize);
    void setrto(int rto);
    void sendrequest(int seq,int timeout);
    void pop(int seq);
    void push(int seq);
    bool exists(int seq);
    bool dequewait(int timeout);
    void readack(int ack,int newwindowsize);
    void writeseq();

    static void *timeout(void *);
    static pthread_t timeoutid;
    static std::map<int,time_t> timeoutrecord;
    static int rto;
    static std::vector<int> timeoutseq;
};
