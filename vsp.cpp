#include<bits/stdc++.h>
#define cin std::cin
#define cout std::cout

using namespace std;

class Process {

    public:

    // stores an id associated with the process.
    int pid;
    // stores the arrival time of the process.
    int arrTime;
    // store the duration for which the process needs to be in the memory.
    int processDuration;
    // stores number of addresses reqd for the process.
    int addrCount;
    // stores all the addresses size.
    vector<int> addrs;
    // effectiveAddr is the sum of all addrs size, since in vsp, we add up all processes.
    int effectiveAddr=0;

    void setAddresses(vector<int> addresses) {
        addrs=addresses;
        for(int i=0; i<addrs.size(); i++) {
            effectiveAddr+=addrs[i];
        }
    }

};

// a comparator to be passed as a parameter to sort function, that will sort pair<int,pair<int,Process>> as it is reqd.
bool comp(pair<int,pair<int,Process>> p1, pair<int,pair<int,Process>> p2) {
    if(p1.first<p2.first)
        return true;
    else if(p1.first==p2.first && p1.second.first<p2.second.first)
        return true;
    else if(p1.first==p2.first && p1.second.first==p2.second.first && p1.second.second.pid<p2.second.second.pid)
        return true;

    return false;
}

// stores the partition of the memory.
class Partition {

    public:

    vector<int> ids;
    vector<int> val;

    // initial state there is a hole from 0 to MEM_SIZE-1
    Partition(int MEM_SIZE) {
        ids.push_back(-1);
        ids.push_back(-1);
        val.push_back(0);
        val.push_back(MEM_SIZE-1);
    }

};

// code for FirstFit.
void firstFit(vector<Process> p, int MEM_SIZE, map<int,Process> pmap) {
    
    // stores the process that are in the queue.
    vector<Process> ps;
    // takes care about partition structure.
    Partition pt(MEM_SIZE);
    int n=p.size();
    // sums up all the turnAroundTimes.
    double totalTurnAroundTime=0;

    // making a vector of time which will store a - pair of  < int and pair of < Process and int > > which represents 
    // the time at which event occurs and the process and '1' if arrival time, '0' means leaving time
    vector<pair<int,pair<int,Process>>> time;

    for(int i=0; i<n; i++) {
        time.push_back({p[i].arrTime,{1,p[i]}});
    }

    // will sort time according to the comparator that we defined..
    sort(time.begin(),time.end(),comp);

    n=time.size();

    // iterate through all time indices, since we will have a change only at one of those.
    for(int i=0; i<time.size(); i++) {
        
        int j=i;

        // check for all processes that are getting finished at this value of time[i]. 
        // and for them just make appropriate changes in Partition class.
        while(j<n && time[j].first==time[i].first && time[j].second.first==0 ) {
            
            for(int k=0; k<pt.ids.size(); k++) {
                if(pt.ids[k]==time[j].second.second.pid) {
                    pt.ids[k]=-1;
                    totalTurnAroundTime+=time[j].first-time[j].second.second.arrTime;
                    break;
                }
            }
            
            j++;
        }
        
        // push all the processes to the queue
        while(j<n && (time[j].first==time[i].first)) {
            ps.push_back(time[j].second.second);
            j++;
        }
        
        int index=0;
            
        while(index<ps.size()) {
            Process ffront = ps[index];

            for(int k=0; k<(int)(pt.ids.size())-1; k++) {
            
                int pp=k;

                // skip all pp where pt ids is -1.
                while(pp<pt.ids.size() && pt.ids[pp]==-1) {
                    pp++;

                }
                
                // In firstfit whichever hole comes first, fill that if hole size is big enough.
                if(pt.ids[k]==-1 && pt.ids[k+1]==-1 && pt.val[pp-1]-pt.val[k]+1>=ffront.effectiveAddr) {
                    pt.ids.insert(pt.ids.begin()+k+1,{ffront.pid,-1});
                    pt.val.insert(pt.val.begin()+k+1,{ffront.effectiveAddr+pt.val[k]-1, ffront.effectiveAddr+pt.val[k]});
                    
                    ps.erase(ps.begin()+index,ps.begin()+index+1);
                    time.push_back({ffront.processDuration+time[i].first,{0,ffront}});
                    sort(time.begin(),time.end(),comp);
                    n=time.size();
                    index--;
                    break;
                }
            }

            index++;
        }

        int cnt=0;
        for(int k=0; k<(int)pt.ids.size(); k++) {
            if(pt.ids[k]!=-1){
                cnt++;
            }
        }

        if(cnt>0) {
            cout<<"The partition at"<<" t = "<<time[i].first<<" is: "<<endl;
            cout<<"PID From To\n";
            for(int k=0; k<(int)pt.ids.size(); k++) {
                if(pt.ids[k]!=-1){
                    cout<<pt.ids[k]<<"   "<<1+pt.val[k]-pmap[pt.ids[k]].effectiveAddr<<"   "<<pt.val[k]<<endl;
                }
            }
            cout<<endl;
        }

        i=j-1;

    }

    // Avg turnaround time will be total turnaround divided by number of processes.
    cout<<setprecision(5)<<"Avg turnaround time = "<<(totalTurnAroundTime/p.size())<<endl;


}

// Code for BestFit.
void bestFit(vector<Process> p, int MEM_SIZE, map<int,Process> pmap) {
    
    // stores the process that are in the queue.
    vector<Process> ps;
    // takes care about partition of memory.
    Partition pt(MEM_SIZE);
    int n=p.size();

    // sums up all the turnAroundTimes
    double totalTurnAroundTime=0;

    // making a vector of time which will store a - pair of  < int and pair of < Process and int > > which represents 
    // the time at which event occurs and the process and '1' if arrival time, '0' means leaving time
    vector<pair<int,pair<int,Process>>> time;

    for(int i=0; i<n; i++) {
        time.push_back({p[i].arrTime,{1,p[i]}});
    }

    // will sort time according to the comparator we defined.
    sort(time.begin(),time.end(),comp);

    n=time.size();

    // iterate through all time indices, since we will have a change only at one of those.
    for(int i=0; i<time.size(); i++) {
        
        int j=i;

        // check for all processes that are getting finished at this value of time[i]. 
        // and for them just make appropriate changes in Partition class.
        while(j<n && time[j].first==time[i].first && time[j].second.first==0 ) {
            for(int k=0; k<pt.ids.size(); k++) {
                if(pt.ids[k]==time[j].second.second.pid) {
                    pt.ids[k]=-1;
                    totalTurnAroundTime+=time[j].first-time[j].second.second.arrTime;
                    break;
                }
            }
            
            j++;
        }
        
        // push all the processes to the queue
        while(j<n && (time[j].first==time[i].first)) {
            ps.push_back(time[j].second.second);
            j++;
        }
        
        int index=0;
            
        while(index<ps.size()) {
            
            Process ffront = ps[index];

            // mmap will store details of all eligible holes
            map<int,pair<int,Process>> mmap;
            
            // f is a flag varibale which will tell if there are any eligible holes.
            int f=-1;

            ffront = ps[index];

            for(int k=0; k<(int)(pt.ids.size())-1; k++) {
                int pp=k;

                while(pp<pt.ids.size() && pt.ids[pp]==-1) {
                    pp++;
                }

                // if eligible, store in the map and put f=1.
                if(pt.ids[k]==-1 && pt.ids[k+1]==-1 && pt.val[pp-1]-pt.val[k]+1>=ffront.effectiveAddr) {
                    mmap[pt.val[pp-1]-pt.val[k]+1]=make_pair(k,ffront);
                    f=1;
                    k=pp-1;
                }
            }

            if(f==-1) {
                index++;
                continue;
            } else {
                // iterate map in ascending order of key, and break after first iteration
                // to get the min eligible hole size.
                // and update the partition.
                for(auto ii: mmap) {

                    int k=ii.second.first;
                    ffront=ii.second.second;


                    pt.ids.insert(pt.ids.begin()+k+1,{ffront.pid,-1});
                    pt.val.insert(pt.val.begin()+k+1,{ffront.effectiveAddr+pt.val[k]-1, ffront.effectiveAddr+pt.val[k]});
                    ps.erase(ps.begin()+index,ps.begin()+index+1);
                    time.push_back({ffront.processDuration+time[i].first,{0,ffront}});

                    sort(time.begin(),time.end(),comp);
                    n=time.size();
                    index--;

                    break;
                }
            }
    
            index++;
        }

        int cnt=0;
        for(int k=0; k<(int)pt.ids.size(); k++) {
            if(pt.ids[k]!=-1){
                cnt++;
            }
        }

        if(cnt>0) {
            cout<<"The partition at"<<" t = "<<time[i].first<<" is: "<<endl;
            cout<<"PID From To\n";
            for(int k=0; k<(int)pt.ids.size(); k++) {
                if(pt.ids[k]!=-1){
                    cout<<pt.ids[k]<<"   "<<1+pt.val[k]-pmap[pt.ids[k]].effectiveAddr<<"   "<<pt.val[k]<<endl;
                }
            }
            cout<<endl;
        }

        i=j-1;

    }

    cout<<setprecision(5)<<"Avg turnaround time = "<<(totalTurnAroundTime/p.size())<<endl;

}

// code for WorstFit.
void worstFit(vector<Process> p, int MEM_SIZE, map<int,Process> pmap) {

    // stores the process that are in the queue.
    vector<Process> ps;
    // takes care about partition structure.
    Partition pt(MEM_SIZE);
    int n=p.size();
    // sums up all the turnAroundTimes.
    double totalTurnAroundTime=0;

    // making a vector of time which will store a - pair of  < int and pair of < Process and int > > which represents 
    // the time at which event occurs and the process and '1' if arrival time, '0' means leaving time
    vector<pair<int,pair<int,Process>>> time;

    for(int i=0; i<n; i++) {
        time.push_back({p[i].arrTime,{1,p[i]}});
    }

    // will sort time according to the comparator we defined.
    sort(time.begin(),time.end(),comp);

    n=time.size();

    for(int i=0; i<time.size(); i++) {
        
        int j=i;
        
        // check for all processes that are getting finished at this value of time[i]. 
        // and for them just make appropriate changes in Partition class.
        while(j<n && time[j].first==time[i].first && time[j].second.first==0 ) {
            for(int k=0; k<pt.ids.size(); k++) {
                if(pt.ids[k]==time[j].second.second.pid) {
                    pt.ids[k]=-1;
                    totalTurnAroundTime+=time[j].first-time[j].second.second.arrTime;
                    break;
                }
            }
            
            j++;
        }
        
        // push all the new processes to the queue.
        while(j<n && (time[j].first==time[i].first)) {
            ps.push_back(time[j].second.second);
            j++;
        }
        int index=0;
            
        while(index<ps.size()) {

            Process ffront = ps[index];
            map<int,pair<int,Process>> mmap;
            int f=-1;

            ffront = ps[index];

            for(int k=0; k<(int)(pt.ids.size())-1; k++) {
                int pp=k;

                while(pp<pt.ids.size() && pt.ids[pp]==-1) {
                    pp++;
                }

                // In worstfit whichever eligible hole comes, add that to the map and make f=1;
                if(pt.ids[k]==-1 && pt.ids[k+1]==-1 && pt.val[pp-1]-pt.val[k]+1>=ffront.effectiveAddr) {
                    mmap[pt.val[pp-1]-pt.val[k]+1]=make_pair(k,ffront);
                    f=1;
                    k=pp-1;
                }
            }

            if(f==-1) {
                index++;
                continue;
            } else {

                // travesrse the map reversely, and break; after getting the first key and value pair.
                // make appropriate changes to the partition.
                for(map<int,pair<int,Process>>::reverse_iterator ii = mmap.rbegin(); ii!=mmap.rend(); ii++) {

                    int k=ii->second.first;
                    ffront=ii->second.second;

                    pt.ids.insert(pt.ids.begin()+k+1,{ffront.pid,-1});
                    pt.val.insert(pt.val.begin()+k+1,{ffront.effectiveAddr+pt.val[k]-1, ffront.effectiveAddr+pt.val[k]});
                    ps.erase(ps.begin()+index,ps.begin()+index+1);
                    time.push_back({ffront.processDuration+time[i].first,{0,ffront}});

                    sort(time.begin(),time.end(),comp);
                    n=time.size();
                    index--;

                    break;
                }
            }

            index++;
            
        }


        int cnt=0;
        for(int k=0; k<(int)pt.ids.size(); k++) {
            if(pt.ids[k]!=-1){
                cnt++;
            }
        }

        if(cnt>0) {
            cout<<"The partition at"<<" t = "<<time[i].first<<" is: "<<endl;
            cout<<"PID From To\n";
            for(int k=0; k<(int)pt.ids.size(); k++) {
                if(pt.ids[k]!=-1){
                    cout<<pt.ids[k]<<"   "<<1+pt.val[k]-pmap[pt.ids[k]].effectiveAddr<<"   "<<pt.val[k]<<endl;
                }
            }
            cout<<endl;
        }

        i=j-1;

    }

    cout<<setprecision(5)<<"Avg turnaround time = "<<(totalTurnAroundTime/p.size())<<endl;
}

int main() {
    
    int processCount;
    int choice=-1;
    int MEM_SIZE=2000;
    // a map that maps process id to the process.
    map<int,Process> pmap;
    int t;

    cout<<"Enter number of test cases: ";
    cin>>t;

    while(t--) {
    
        while(choice!=1 && choice!=2 && choice!=3) {
            cout<<"Enter the type of fit (1: First-Fit, 2: Best-Fit, 3: Worst-Fit): ";
            cin>>choice;
        }

        cout<<"Enter memory size: ";
        cin>>MEM_SIZE;
        cout<<"Enter number of processes: ";
        cin>>processCount;

        vector<Process> p(processCount);

        cout<<"\nNow give the input\n";
        cout<<"On the first line, enter process id\n";
        cout<<"On the second line, enter arrival time and the duration of process\n";
        cout<<"On the third line, enter number of addresses and that many sizes\n\n";

        for(int i=0; i<processCount; i++) {
            
            int id,at,lt,acnt;

            // cout<<"Enter process id: ";
            cin>>id;
            // cout<<"Enter arrival time: ";
            cin>>at;
            // cout<<"Enter duration of process in mem: ";
            cin>>lt;
            // cout<<"Enter address count: ";
            cin>>acnt;
            // cout<<"Enter mem size of addresses: ";

            vector<int> addrs(acnt);
            for(int j=0; j<acnt; j++) {
                cin>>addrs[j];
            }

            p[i].pid = (id);
            p[i].arrTime = (at);
            p[i].processDuration = (lt);
            p[i].addrCount = (acnt);
            p[i].setAddresses(addrs);
            pmap[id]=p[i];
            cout<<endl;

        }

        switch(choice) {
            case 1:
            firstFit(p,MEM_SIZE,pmap);
            break;

            case 2:
            bestFit(p,MEM_SIZE,pmap);
            break;

            case 3:
            worstFit(p,MEM_SIZE,pmap);
            break;

            default:
            cout<<"Something went wrong!\n";
            break;
        }

        choice=-1;
    }

    return 0;

}
