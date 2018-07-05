#include"sendw.h"

sendwindow::sendwindow(int ack,int windowsize){
    this->maxseq=ack;
    this->windowsize=windowsize;
    rptr=seqdeque.begin()+windowsize;
    this->popfinish=0;

    pthread_create(&sendwindow::timeoutid,NULL,sendwindow::timeout,NULL);
}

void sendwindow::setrto(int rto){
    this->rto=rto;
}

void sendwindow::pop(int seq){
    if(seqdeque.front()&&seqdeque.front()<=seq)
        seqdeque.pop_front();
    else this->popfinish=1;
}

void sendwindow::readack(int ack,int newwindowsize){
    this->windowsize=newwindowsize;
    ack--;
    while(popfinish!=1)
        sendwindow::pop(ack);
    popfinish=0;
    auto it=timeoutrecord.find(ack);
    timeoutrecord.erase(timeourecord.begin(),++it);

    rptr=seqdeque.begin()+windowsize;
}

void sendwindow::push(int seq){
    seqdeque.push_back(seq);
}

void sendwindow::sendrequest(int seq,int timeout){
    int n;
    if((n=MAX-(int)seqdeque.size())>0){
        sendwindow::push(seq); 
    }else{
        if(sendwindow::dequewait(timeout))
            sendwindow::push(seq);
        else{ //add to resendlist
            timeoutseq.push_back(seq); 
        }  
    }
}

bool sendwindow::dequewait(int timeout){
    int newsizes;
    time_t starttime=time(0);
    while(starttime+timeout<=time(0)&&seqdeque.size()==MAX);
    newsizes=MAX-(int)seqdeque.size();
    return newsizes==0?false:true;
}

void sendwindow::writeseq(){
    if(maxseq<windowsize){
        for(auto i=seqdeque.begin();*i!=maxseq;i++)
            timeoutrecord.insert(std::pair<int,time_t>(*i,time(0)));
        //write from begin() to maxseq
    } else{
        for(auto i=seqdeque.begin();i!=rptr;i++)
            timeoutrecord.insert(std::pair<int,time_t(*i,time(0)));
        //write from begin() to rptr
    }
}

bool sendwindow::exists(int seq){
    int begin=0,end=seqdeque.size()-1,mid;
    while(begin<=end){
        mid=(begin+end)/2;
        if(seqdeque[mid]==seq){
            return true;
        }
        else if(seqdeque[mid]<seq)
            begin=mid+1;
        else
            end=mid-1;
    }
    return false; 
}

void * sendwindow::timeout(void *){
    for(;;){
        time_t now=time(0);
        for(auto i=timeoutrecord.begin();i!=timeoutrecord.end();i++){
            if(*i&&now<sendwindow::rto+i->second){
                timeoutseq.push_back(i->first);
                sendwindow::readack(i->first,sendwindow::windowsize);
            }
        }
    }
}  

sendwindow::~sendwindow(){
      pthread_cancel(sendwindow::timeoutid);
}
