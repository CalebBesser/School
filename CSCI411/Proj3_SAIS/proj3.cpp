#include<iostream>
#include<vector>
#include<string>


using namespace std;

void SAIS(vector<int> &SA, vector<int> &Tint, int AlphSize, string s){
    vector<int> A(AlphSize);
    vector<int> C(AlphSize);
    vector<int> B(AlphSize);
    //------------
    // cout << "T is: " << endl;
    // for(int i = 0; i < (int)Tint.size(); i++){
    //     cout << Tint[i] << " ";
    // }
    // cout << endl;
    //------------

    for(int i = 0; i < (int)Tint.size(); i++){
        A[Tint[i]]++;
    }

    C[0] = 0;
    for(int i = 1; i < (int)C.size(); i++){
        C[i] = C[i - 1] + A[i - 1];
    }

    B[AlphSize-1] = (int)s.size();
    for(int i = (int)B.size() - 2; i > 0; i--){
        B[i] = B[i + 1] - A[i + 1];
    }
    vector<bool> t((int)Tint.size());
    t[(int)Tint.size()-1] = 1;
    for(int i = (int)Tint.size() - 2; i >= 0; i--){
        if(Tint[i] > Tint[i+1]){
            t[i] = 0;
        }else if(Tint[i] < Tint[i+1]){
            t[i] = 1;
        }else if(Tint[i] == Tint[i+1]){
            t[i] = t[i+1];
        }
        if(t[i] == 0){
            if(t[i + 1] == 1){
                SA[B[Tint[i+1]]] = i + 1;
                B[Tint[i+1]]--;
            }
        }
    }
    //------------
    // cout << "Type array t is: " << endl;
    // for(int i = 0; i < (int)t.size(); i++){
    //     cout << t[i] << " ";
    // }
    // cout << endl;
   //--------------

    //------------
    // cout << "SA after placing LMS-suffixes is: " << endl;
    // for(int i = 0; i < (int)SA.size(); i++){
    //     cout << SA[i] << " ";
    // }
    // cout << endl;
   //--------------

   

    for(int i = 0; i < (int)B.size(); i++){
        B[i] = C[i];
    }

    //-------------
    // cout << "The beginning of c-buckets (array B): " << endl;
    // for(int i = 0; i < (int)B.size(); i++){
    //     cout << B[i] << " ";
    // }
    // cout << endl;
    //-------------

    for(int i = 0; i < (int)SA.size(); i++){
        int p = SA[i];
        if(p != -1){
            int Tsize = (int)Tint.size();
            if(p == 0){
                if(t[Tsize-1] == 0){
                    SA[B[Tint[Tsize-1]]] = Tsize-1;
                    B[Tint[Tsize-1]]++;
                }
            }else{
                if(t[p-1] == 0){
                SA[B[Tint[p-1]]] = p-1;
                B[Tint[p-1]]++;
                }
            }
            
        }
    }

    //-------------
    // cout << "SA after induce-sort of L-type suffixes is: " << endl;
    // for(int i = 0; i < (int)SA.size(); i++){
    //     cout << SA[i] << " ";
    // }
    // cout << endl;
    //-------------

    B[AlphSize-1] = (int)s.size();
    for(int i = (int)B.size() - 2; i > 0; i--){
        B[i] = B[i + 1] - A[i + 1];
    }
    
    //-------------
    // cout << "The end of c-buckets (array B): " << endl;
    // for(int i = 0; i < (int)B.size(); i++){
    //     cout << B[i] << " ";
    // }
    // cout << endl;
    //-------------
 

    vector<bool> L((int)SA.size());

    for(int i = (int)SA.size()-1; i > 0; i--){
        int p = SA[i];
        
        if(p != -1){
            int Tsize = (int)Tint.size();
            if(p == 0){
                if(t[Tsize-1] == 1){  
                    if(t[Tsize-2] == 0){
                        L[B[Tint[Tsize-1]]] = 1;
                    }
                    SA[B[Tint[Tsize-1]]] = Tsize-1;
                    B[Tint[Tsize-1]]--;
                } 
            }else{
                if(t[p-1] == 1){   
                    if(t[p-2] == 0){
                        L[B[Tint[p-1]]] = 1;
                    }
                    SA[B[Tint[p-1]]] = p-1;
                    B[Tint[p-1]]--;
                }   
            }
  
        }
    }
    //-------------
    // cout << "SA after induce-sort of S-type suffixes is: " << endl;
    for(int i = 0; i < (int)SA.size(); i++){
        cout << SA[i] << " ";
    }
    cout << endl;
    //-------------

    // for(int i = 0; i < (int)SA.size(); i++){
    //     cout << SA[i] << " ";
    // }
    // cout << endl;

}

int main() {
    string input;
    string s;

    vector<int> count(256, 0);

    while(getline(cin, input)){
        s += input;
    }

    for(int i = 0; i < (int)s.size(); i++){
        count[(int)s[i]] = 1;
    }

    int currName = 1;
    int AlphSize = 1;
    for(int i = 0; i <  (int)count.size(); i++){
        if(count[i] == 1){
            AlphSize++;
            count[i] = currName++;
        }
    }
    vector<int> Tint;
    for(int i = 0; i < (int)s.size(); i++){
        Tint.push_back(count[(int)s[i]]);
    }
    Tint.push_back(0);
   


    vector<int> SA((int)Tint.size(), -1);
    SAIS(SA, Tint, AlphSize, s);
}