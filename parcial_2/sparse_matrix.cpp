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
	caca.print();
	caca.print_matrix();
	
	//Find largest value in col and swap to make pivot
	rcv id = caca.create_identity();

	int r = caca.pivot_swap(0);
	id.swap_rows(0, r);
	
	
	double r2 = caca.reduce(0);
	id.reduce(0, r2);

	
	vector<double> coefs = caca.make_column_0(0);	
	id.make_column_0(0, coefs);


	r2 = caca.reduce(1);
	id.reduce(1, r2);

	caca.transpose();
	id.transpose();

	
	//TODO BIEN ----
	 
	//SECOND STEP
	r = caca.pivot_swap(0);
	id.swap_rows(0, r);

	r2 = caca.reduce(0);
	id.reduce(0, r2);

	cerr<<"después de reduce"<<endl;
	
	caca.print_matrix();
	id.print();
	id.print_matrix();


	coefs = caca.make_column_0(0);
	id.make_column_0(0, coefs);

	cerr<<"después de make column 0"<<endl;
	
	caca.print_matrix();
	id.print();
	id.print_matrix();

	r2 = caca.reduce(1);
	id.reduce(1, r2);


	caca.print_matrix();
	id.print_matrix();


}
