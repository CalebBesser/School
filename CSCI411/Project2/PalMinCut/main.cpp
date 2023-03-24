#include<iostream>
#include<string>
#include<vector>
using namespace std;

//This function fills in table P 
void is_pal(vector< vector<bool> > &P, const string &s);

int main(){
        string myString;
        cin >> myString;
        cout << myString << "\n";
        int len = myString.size();
        vector<vector<bool>> P(len, vector<bool>(len, 0));
        vector<int> C(len, 0);
        vector<int> B(len, -1);
        is_pal(P, myString);
        C[0] = 0;
        B[0] = -1;
        for(int i = 1; i < len; i++){
            if(P[0][i] == 1){
                C[i] = 0;
                B[i] = -1;
            }else{
                for(int j = 0; j < i; j++){
                    if(P[j+1][i] == 1){
                        C[i] = C[j] + 1;
                        B[i] = j;
                        break;
                    }
                }
            }
           
        }
        /*
        loop through each element i
        for each i loop from j to i, j from 0
        update C and B for each element based on rules on pdf
        */
        cout << "C: ";
        for(int i = 0; i < len; i++){
            cout << C[i] << " ";
        }
        cout << "\n";
        cout << "B: ";
        for(int i = 0; i < len; i++){
            cout << B[i] << " ";
        }
        cout << "\n";
        cout << "   ";
        for(int j = 0; j < len; j++){
            cout << myString[j] << " ";
        }
        cout << "\n";
        for(int i = 0; i < len; i++){
            cout << myString[i] << ": ";
            for(int j = 0; j < len; j++){
                cout << P[i][j] << " ";
            }
            cout << "\n";
        }
        return 0;
}//main()

void is_pal(vector< vector<bool> > &P, const string &s){
    int j = 0;
    int size = (int)s.length();
    for(int dif = 0; dif <= size; dif++){
        for(int i = 0; i < size - dif; i++){
            j = i + dif;
            if(i < j-1){
                if((s[i] == s[j]) && P[i+1][j-1])
                    P[i][j] = true;
            }//i < j-1
            else if(i == (j-1)){
                if(s[i] == s[j])
                    P[i][j] = true;
            }//i = j-1
            else if(i == j){
                P[i][j] = true;
                }
        }//for i
    }//for dif
}//is_pal

