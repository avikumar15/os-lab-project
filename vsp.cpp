#include<bits/stdc++.h>
#define cin std::cin

using namespace std;

class Process {

    public:

    int pid;
    int arr_time;
    int leave_time;
    int addrCount;
    vector<int> addrs;
    int effectiveAddr=0;
    int actualArrivalTimeInMemory=0;

    bool operator < (const Process &b) {
        return true;
    }

    void setPid(int p) {
        pid=p;
    }

    void setArrTime(int arr) {
        arr_time=arr;
    }

    void setLeaveTime(int leave) {
        leave_time=leave;
    }

    void setAddrCount(int addrCnt) {
        addrCount=addrCnt;
    }

    void setAddresses(vector<int> addresses) {
        addrs=addresses;
        for(int i=0; i<addrs.size(); i++) {
            effectiveAddr+=addrs[i];
        }
    }

};

bool comp(pair<int,pair<int,Process>> p1, pair<int,pair<int,Process>> p2) {
    if(p1.first<p2.first)
        return true;
    else if(p1.first==p2.first && p1.second.first<p2.second.first)
        return true;
    else if(p1.first==p2.first && p1.second.first==p2.second.first && p1.second.second.pid<p2.second.second.pid)
        return true;

    return false;
}

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

void firstFit(vector<Process> p, int MEM_SIZE) {
    
    queue<Process> ps;
    Partition pt(MEM_SIZE);
    int n=p.size();

    // making a vector of time which will store a - pair of  < int and pair of < Process and int > > which represents 
    // the time at which event occurs and the process and '1' if arrival time, '0' means leaving time
    vector<pair<int,pair<int,Process>>> time;

    for(int i=0; i<n; i++) {
        time.push_back({p[i].arr_time,{1,p[i]}});
        time.push_back({p[i].arr_time+p[i].leave_time,{0,p[i]}});
    }

    // will sort time according to first int bby default.
    sort(time.begin(),time.end(),comp);

    n=time.size();

    cout<<"\n******TIME*********\n";
    for(int i=0; i<n; i++) {
        cout<<"Time is- "<<time[i].first<<" and type is "<<time[i].second.first<<endl;
        cout<<"Process pid is "<<time[i].second.second.pid<<endl<<endl;
    }

    for(int i=0; i<n; i++) {
        
        int j=i;

        while(j<n && time[j].first==time[i].first && time[j].second.first==0 ) {
            
            for(int k=0; k<pt.ids.size(); k++) {
                if(pt.ids[k]==time[j].second.second.pid) {
                    pt.ids[k]=-1;
                    break;
                }
            }
            
            j++;
        }

        while(j<n && time[j].first==time[i].first) {
            ps.push(time[j].second.second);
            Process ffront = ps.front();

            for(int k=0; k<(int)(pt.ids.size())-1; k++) {
                if(pt.ids[k]==-1 && pt.ids[k+1]==-1 && pt.val[k+1]-pt.val[k]>=ffront.effectiveAddr) {
                    pt.ids.insert(pt.ids.begin()+k+1,ffront.pid);
                    pt.val.insert(pt.val.begin()+k+1,ffront.effectiveAddr+pt.val[k]);
                    ps.pop();
                    break;
                }
            }
        }

        i=j-1;

    }


}

void bestFit(vector<Process> p, int MEM_SIZE) {
    
}

void worstFit(vector<Process> p, int MEM_SIZE) {
    
}

int main() {
    
    int processCount;
    int choice=-1;
    int MEM_SIZE=2000;
    
    while(choice!=1 && choice!=2 && choice!=3) {
        cout<<"Enter the type of fit (1: First-Fit, 2: Best-Fit, 3: Worst-Fit): ";
        cin>>choice;
        cout<<endl;
    }

    cout<<"Enter memory size: ";
    cin>>MEM_SIZE;

    // cout<<"Enter number of Processes: ";
    cin>>processCount;

    vector<Process> p(processCount);

    for(int i=0; i<processCount; i++) {
        
        int id,at,lt,acnt;

        cin>>id>>at>>lt>>acnt;
        vector<int> addrs(acnt);

        for(int j=0; j<acnt; j++) {
            cin>>addrs[j];
        }

        p[i].setPid(id);
        p[i].setArrTime(at);
        p[i].setLeaveTime(lt);
        p[i].setAddrCount(acnt);
        p[i].setAddresses(addrs);

    }

    switch(choice) {
        case 1:
        firstFit(p,MEM_SIZE);
        break;

        case 2:
        bestFit(p,MEM_SIZE);
        break;

        case 3:
        worstFit(p,MEM_SIZE);
        break;

        default:
        cout<<"Something went wrong!\n";
        break;
    }

    return 0;

}
