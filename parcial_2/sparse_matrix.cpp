#include <iostream>
#include <vector>
#include <set>
#include "rcv.h"

using namespace std;

int main(){
	int matrix_size;
	cin>>matrix_size;

	
	vector<int> rows(matrix_size);
	vector<int> columns(matrix_size);
	vector<double> values(matrix_size);

	set<pair<int, int> > occupied;
	int max_row = -1;
	int max_col = -1;

	//Read values
	for(int i=0; i<matrix_size; i++){
		cin>>values[i];
	}
	//Read rows
	for(int i=0; i<matrix_size; i++){
		cin>>rows[i];
		max_row = max(max_row, rows[i]);
	}
	//Read columns
	for(int i=0; i<matrix_size; i++){
		cin>>columns[i];
		max_col = max(max_col, columns[i]);

		if( occupied.find(make_pair(rows[i],columns[i])) == occupied.end()){
				occupied.insert(make_pair(rows[i], columns[i]));
		}
		else{
			return -2;
		}
	}
	
	rcv caca(rows, columns, values, make_pair(max_row+1, max_col+1));
	rcv inverse = caca.inverse();

	cout<<"Matriz original: "<<endl;
	caca.print_matrix();
	cout<<endl<<"Inversa: "<<endl;
	inverse.print_matrix();
}
