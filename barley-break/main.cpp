#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <stack>
#include <fstream>
#include <limits.h>
#include <queue>

#define SIZE 3

using namespace std;


template <class TVertexID>  class CMyGraph  {
private:
    multimap <TVertexID, TVertexID> links;
    set <TVertexID> vertexes;
public:
    CMyGraph(TVertexID what){
        AddVertex(what);
    }

    vector<TVertexID> getNeighbors(TVertexID who){
        vector<TVertexID> WhoNeighbors = who.getOutgoingPath();
        for (int i = 0; i < WhoNeighbors.size(); ++i){
            AddEdge(who, WhoNeighbors[i]);
        }
    }

    void AddEdge(TVertexID from, TVertexID to){
        this->AddVertex(from);
        this->AddVertex(to);
        links.insert(make_pair(from,to));
    }

    void AddVertex(TVertexID what){
        if (vertexes.find(what) == vertexes.end()){
            vertexes.insert(what);
        }
    }

};



class CState{
public:
    int position[SIZE][SIZE];

    CState(vector<int>& arr){
        for (int i = 0; i < SIZE;++i){
            for (int j = 0; j < SIZE; ++j){
                position[i][j] = arr[i*SIZE+j];
            }
        }
    }

    int hevristics(){
        int hevr = 0;
        for (int i = 0; i< SIZE; ++i){
            for (int j = 0; j <SIZE; ++j){
                if (position[i][j] != 0){
                    hevr += abs(position[i][j] / 3 - i) + abs(position[i][j] %3 - j);
                }
            }
        }
        return hevr;
    }

    vector<CState>&& getOutgoingPath()const{
        vector<CState> output;
        int j = 0;
        int i = 0;
        for (; i < SIZE;++i){
            for (; j < SIZE; ++j){
                if (position[i][j] == 0){
                    break;
                }
            }
            if (position[i][j] == 0){
                break;
            }
        }
        if (i != 0){
            output.push_back(this->moveUp(i,j));
        }

        if (i != SIZE - 1){
            output.push_back(this->moveDown(i,j));
        }

        if (j != 0){
            output.push_back(this->moveLeft(i,j));
        }
        if (j != SIZE - 1){
            output.push_back(this->moveRight(i,j));
        }
        return move(output);
    }
    CState moveRight(int a, int b)const{
        vector<int> arr;
        for (int i = 0; i < SIZE;++i){
            for (int j = 0; j < SIZE; ++j){
                arr[i*SIZE+j] = position[i][j];
            }
        }
        swap(arr[a*SIZE + b], arr[a*SIZE+b+1]);
        return *(new CState(arr));
    }
    CState moveLeft(int a, int b)const{
        vector<int> arr;
        for (int i = 0; i < SIZE;++i){
            for (int j = 0; j < SIZE; ++j){
                arr[i*SIZE+j] = position[i][j];
            }
        }
        swap(arr[a*SIZE + b], arr[a*SIZE+b-1]);
        return *(new CState(arr));
    }
    CState moveUp(int a, int b)const{
        vector<int> arr;
        for (int i = 0; i < SIZE;++i){
            for (int j = 0; j < SIZE; ++j){
                arr[i*SIZE+j] = position[i][j];
            }
        }
        swap(arr[a*SIZE + b], arr[(a+1)*SIZE+b]);
        return *(new CState(arr));
    }

    CState moveDown(int a, int b)const{
        vector<int> arr;
        for (int i = 0; i < SIZE;++i){
            for (int j = 0; j < SIZE; ++j){
                arr[i*SIZE+j] = position[i][j];
            }
        }
        swap(arr[a*SIZE + b], arr[(a-1)*SIZE+b]);
        return *(new CState(arr));
    }
};
template <class TVertexID>
int AStar (TVertexID First, TVertexID Goal) {
    priority_queue < pair<TVertexID, int>> que;
    map <TVertexID, int> dij;
    set<TVertexID> seen;
    set<TVertexID> inQue;
    vector <TVertexID> neib;
    CMyGraph<TVertexID> Graph(First);
    map <TVertexID, TVertexID> parent;


    parent.insert(make_pair(First, First));
    neib.reserve(4);
    dij.insert(make_pair(First, 0));
    que.insert(make_pair(First, First.hevristics()));
    inQue.insert(First);
    while (!(que.empty())){
        neib.clear();
        bool tentative_is_better = false;
        TVertexID  looking= que.pop_back()->first;
        inQue.erase(inQue.find(looking));
        if (looking == Goal){
            return dij[looking];
        }
        seen.insert(looking);
        neib = Graph.getNeighbors(looking);
        for (int i = 0; i < neib.size(); ++i){
            if (seen.find(neib[i])!=seen.end()){
                continue;
            }
            int tmp = dij[First] + 1 ;
            if (inQue.find(neib[i])){
                inQue.insert(neib[i]);
                dij.insert(make_pair(neib[i], dij[First]+1));
                que.push(make_pair(neib[i], neib[i].hevristics()+ dij[neib[i]]));
                tentative_is_better = true;
            }
            else{
                if (tmp < dij[neib[i]]){
                    tentative_is_better = true;
                }
            }
            if(tentative_is_better == true){
                if(parent.find(neib[i])==parent.end()){
                    parent.insert(make_pair(neib[i], looking));
                }
                else {
                    parent[neib[i]] = looking;
                    dij[neib[i]] = tmp;
                }
            }

        }
    }
    return  - 1;
}

template <class TVertexID>
bool compar (TVertexID* a, TVertexID* b){
    return a < b;
}


bool compar(pair<CState, int>* a, pair<CState, int>* b){
    return a->second < b->second;
}





int main()
{
    vector <int> F;
    F.reserve(SIZE*SIZE);
    vector <int> S;
    S.reserve(SIZE*SIZE);
    int x;
    for (int i = 0 ; i < SIZE*SIZE; ++i){
        cin >> x;
        F.push_back(x);
    }

    for (int i = 0 ; i < SIZE*SIZE; ++i){
        cin >> x;
        S.push_back(x);
    }

    cout << AStar(new CState(F), new CState(S));

    return 0;
}
