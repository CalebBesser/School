#include<iostream>
#include<string>
#include<vector>
#include <algorithm>
using namespace std;

//This function fills in table P 
void is_pal(vector< vector<bool> > &P, const string &s);

void printSubStrings(const string &s, vector<int> &B, int start, vector<string> &substrings){
    int to = B[start];
    if(B[start] == -1){
        if(start == 0){
            char temp = s[0];
            string sub;
            sub += temp;
            substrings.push_back(sub);
            return;
        }else{
            string sub = s.substr(0, start-B[start]);
            substrings.push_back(sub);
            return;
        }
        
    }else{
        string sub = s.substr(to + 1, start-B[start]);
        substrings.push_back(sub);
        return printSubStrings(s, B, to, substrings);
        
    }
}

int main(){
        string myString;
        cin >> myString;
        vector<string> substrings;
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

        printSubStrings(myString, B, len-1, substrings);

        reverse(substrings.begin(), substrings.end());

        for(int i = 0; i < (int)substrings.size(); i++){
            cout << substrings[i] << "\n";
        }
        cout << "C: ";
        for(int i = 0; i < len; i++){
            cout << C[i] << " ";
        }
        cout << "\n";
        if(B[0] == -1){
            cout << "B:";
        }else{
            cout << "B: ";
        }
        
        for(int i = 0; i < len; i++){
            if(B[i+1] == -1){
                cout << B[i];
            }else{
                cout << B[i] << " ";
            }
            
        }
        cout << "\n";
        cout << "  ";
        for(int i = 0; i < len; i++){
            cout << myString[i] << " ";
        }
        cout << "\n";
        for(int i = 0; i < len; i++){
            cout << myString[i] << " ";
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

