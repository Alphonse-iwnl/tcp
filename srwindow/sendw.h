#include<deque>
#include<vector>
#include<map>
#include<ctime>
#include<pthread.h>
#define MAX 100

class sendwindow{
    int maxseq;
    ~sendwindow();
    public:
    sendwindow(int ack,int windowsize);
    void setrto(int rto);
    void sendrequest(int seq,int timeout);
    static void pop(int seq);
    void push(int seq);
    bool exists(int seq);
    bool dequewait(int timeout);
    static void readack(int ack,int newwindowsize);
    void writeseq();

    static std::deque<int> seqdeque;
    static std::deque<int>::iterator rptr;
    static int popfinish;
    static int windowsize;
    static void *timeout(void *);
    static pthread_t timeoutid;
    static std::map<int,time_t> timeoutrecord;
    static int rto;
    static std::vector<int> timeoutseq;
};
