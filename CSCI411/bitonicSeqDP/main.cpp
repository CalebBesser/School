#include<iostream>
#include<string>
#include<vector>
#include<stack>
using std::stack;
using std::vector;
using std::string;
using std::endl;
using std::cout;
using std::cin;
using std::cerr;


void findBitonic(const vector<int> &A);

int main(){
	int size;
	cin >> size;
	vector<int> A;
	for(int i = 0; i < size; i++){
		int x;
		cin >> x;
		A.push_back(x);
	}

	string command;
	cin >> command;
	if(command == "findBitonic"){
		findBitonic(A);//will print the results inside this function
	}else{
		cerr << "ERROR: the command " << command << " is not recognized." << endl;
	}
	return 0;
}

void backtrack(int ind, vector<int> &incr, const vector<int> &backtrackIncreasing, const vector<int> &A);

void findBitonic(const vector<int> &A){
    //Write your code here
	int size = (int)A.size();
	if(size < 3){
		return;
	}
	vector<int> increasing(size, 1);
	vector<int> backtrackIncreasing(size, 0);
	vector<int> decreasing(size, 1);
	vector<int> backtrackDecreasing(size, 0);

	increasing[0] = 1;
	backtrackIncreasing[0] = 0;
	for(int i = 0; i < size; i++){
		backtrackIncreasing[i] = i;
		for(int j = 0; j < i; j++){
			if(A[j] < A[i]){
				if(increasing[j] + 1 > increasing[i]){
					increasing[i] = increasing[j] + 1;
					backtrackIncreasing[i] = j;
				}
			}
		}
	}

	decreasing[0] = 1;
	backtrackDecreasing[0] = 0;
	for(int i = size - 2; i >= 0; i--){
		backtrackDecreasing[i] = i;
		for(int j = size - 1; j > i; j--){
			if(A[j] < A[i]){
				if(decreasing[j] + 1 > decreasing[i]){
					decreasing[i] = decreasing[j] + 1;
					backtrackDecreasing[i] = j;
				}
			}
		}
	}
	int ind = -1;
	int maxLen = 0;
	for( int i = 1; i < size - 1 ; i++){
		if(increasing[i] != 1 && decreasing[i] != 1){
			if(increasing[i] + decreasing[i] - 1 > maxLen){
				maxLen = increasing[i] + decreasing[i] - 1;
				ind = i;
			}
		}
	}
	if(maxLen < 3){
		return;
	}else if(ind == -1){
		return;
	}
	vector<int> incr;
	backtrack(ind, incr, backtrackIncreasing, A);
	vector<int> decr;
	backtrack(backtrackDecreasing[ind], decr, backtrackDecreasing, A);
	for(int i = (int)incr.size()-1; i >= 0; i--){
		cout << incr[i] << " ";
	}
	for(int i = 0; i < (int)decr.size() - 1; i++){
		cout << decr[i] << " ";
	}
	cout << endl;

}//findBitonic()

void backtrack(int ind, vector<int> &incr, const vector<int> &backtrackIncreasing, const vector<int> &A){
	incr.push_back(A[ind]);
	if(backtrackIncreasing[ind] == ind){
		return;
	}
	backtrack(backtrackIncreasing[ind], incr, backtrackIncreasing, A);
}