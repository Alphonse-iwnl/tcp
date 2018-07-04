#include"sendwindow.h"

sendwindow::sendwindow(){
    for(int i=1;i<MAX+1;i++){
        buf[i]=i;
    }
    optr=nptr=iptr=buf;
    pthread_create(&sendwindow::timeoutid,NULL,sendwindow::timeout,NULL);

}

void sendwindow::readack(int ack,int windowsize){
    if(optr==iptr==buf[MAX-1])
        optr=nptr=iptr=buf;
    if(* (iptr+=windowsize-(iptr-optr))<MAX){
        iptr+=windowsize-(iptr-optr);
        sendwindow::pop(ack-1); 
    }
}  

void sendwindow::pop(int seq){
    if(sendwindow::exists(seq)){
        int *tptr=sendwindow::find(seq);
        while(tptr&&optr++!=tptr);
        nptr=optr;
    }
}

bool sendwindow::exists(int seq){
    if(sendwindow::find(seq)==NULL)
        return false;
    else return true;
}

int * sendwindow::find(int seq){
    int *tptr=optr;
    while(tptr++!=iptr){
        if(*tptr==seq)
            return tptr;
    }
    return NULL;
}

void sendwindow::writemsg(){
    sendwindow::push(*--iptr);
}

void sendwindow::push(int seq){
    while(*nptr<=seq){
        overtime[--(*nptr)]=time(0);
        nptr++;
    }
}  

void sendwindow::recvRTO(int RTO){
    sendwindow::rto=RTO;
}

void* sendwindow::timeout(void *){
    for(;;){
        time_t now=time(0);
        for(auto it=overtime.begin();it!=overtime.end();it++){
            if(*it&&now<sendwindow::rto+*it){
                //do something when timeout 
            } 
        }
    }    
}
 
sendwindow::~sendwindow(){
      delete optr;
      delete iptr;
      delete nptr;
      optr=iptr=nptr=NULL;
      pthread_cancel(sendwindow::timeoutid);
}
