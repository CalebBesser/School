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

