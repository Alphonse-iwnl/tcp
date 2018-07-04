#include<vector>
#include<algorithm>
#include<string>
#include<ctime>
#include<pthread.h>

#define MAX 4096

class sendwindow{
    int buf[MAX];    
    int *optr,*iptr,*nptr;
    int maxseq;
    ~sendwindow();
    public:
    sendwindow();
    void recvRTO(int RTO);
    void writemsg();
    void readack(int ack,int windowsize);
    void pop(int seq);
    void push(int seq);
    bool exists(int seq); 
    int *find(int seq);
    static void *timeout(void *);
    static pthread_t timeoutid;
    static std::vector<std::time_t> overtime;
    static int rto;
};


