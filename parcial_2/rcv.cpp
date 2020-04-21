#include "rcv.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

rcv::rcv(vector<int> &r, vector<int> &c, vector<double> &v, pair<int, int> dim){
	//Constructor, receives a vector of rows, columns and values, also a pair with dimensions (n, m).

	rows = move(r);
	columns = move(c);
	values = move(v);
	dimension = dim;
}

void rcv::swap_rows(int row_a, int row_b){
	//Swaps elements from row a and row b

	if(row_a >= dimension.first || row_b >= dimension.first || row_a < 0 || row_b < 0) return;

	for(int i=0; i<rows.size(); i++){
		if(rows[i] == row_a) rows[i] = row_b;
		else if(rows[i] == row_b) rows[i] = row_a;
	}
}

void rcv::multiply(double scalar, int row_a){
	//Multiplies a row by a scalar

	for(int i=0; i<rows.size(); i++){
		if(rows[i] == row_a){
			values[i] *= scalar;
		}
	}
}

void rcv::add(int row_a, int row_b, double scalar){
	//Adds into row a row b multiplied by scalar (row_a += scalar*row_b) 

	if(row_a == row_b) return;

	//Stores the pair (column, value) in its own bucket (row)
	map<int, double> a;
	vector<pair<int, double> > b;

	auto row_it = rows.begin();
	auto col_it = columns.begin();
	auto val_it = values.begin();

	while(row_it != rows.end()){
		if(*row_it == row_a){
			a[*col_it] = *val_it;
			row_it = rows.erase(row_it);
			col_it = columns.erase(col_it);
			val_it = values.erase(val_it);
		}
		else if(*row_it == row_b){
			b.push_back(make_pair(*col_it, *val_it));
			row_it++;
			col_it++;
			val_it++;
		}
		else{
			row_it++;
			col_it++;
			val_it++;
		}
	}


	//Add multiplied by scalar
	for(int i=0; i<b.size(); i++){
		a[b[i].first] += b[i].second*scalar;
	}

	for(auto it=a.begin(); it!=a.end(); ++it){
		rows.push_back(row_a);
		columns.push_back(it->first);
		values.push_back(it->second);
	}
}

int rcv::pivot_swap(int step){
	//Finds the largest value in column under that row, this will be the pivot, now swap the current row and the new row.

	//Returns row swapped

	double max_val = INT_MIN;
	int max_row = -1;
	double current_val = INT_MIN;
	for(int i=0; i<rows.size(); i++){
		if(columns[i] == step && columns[i] == rows[i]){
			current_val = values[i];
			break;
		}
	}

	for(int i=0; i<rows.size(); i++){	
		if(columns[i] == step && values[i] > max_val && rows[i] > step){
			max_val = values[i];
			max_row = rows[i];
		}
	}	
	
	if(max_row != -1){
		this->swap_rows(step, max_row);
		return max_row;
	}
	return -1; // No swap
}

double rcv::reduce(int step){
	//Find all values in the same row, then multiplies them by 1/pivot (current_val)
	//Returns pivot which divides all rows, then later used by void rcv::reduce

	//Find the current value
	double current_val = INT_MIN;
	//Find indexes of all values in the same row, then update.
	vector<int> remaining;

	for(int i=0; i<rows.size(); i++){
		if(rows[i] == step && columns[i] == step){
			current_val = values[i];
			if(current_val < 1.000001 && current_val > 0.999999){
				//If pivot is 1, don't do anything
				return 1.0;
			}
			remaining.push_back(i);
		}
		else if(rows[i] == step){
			remaining.push_back(i);	
		}
	}

	if(current_val == INT_MIN){
		throw "reudce: element is 0";
	}
	else{
		for(int i=0; i<remaining.size(); i++){
			values[remaining[i]] /= current_val; 
		}
	}
	return current_val;
}


void rcv::reduce(int step, double current_val){
	//Find all values in the same row, then multiplies them by 1/pivot (current_val)
	
	vector<int> remaining;
	if(current_val < 1.0000001 && current_val > 0.9999999){
		//If pivot is 1, don't do anything.
		return;
	}

	for(int i=0; i<rows.size(); i++){
		if(rows[i] == step){
			remaining.push_back(i);	
		}
	}

	if(current_val == INT_MIN){
		cerr<<"reudce: element is 0"<<endl;
		return;
	}
	else{
		for(int i=0; i<remaining.size(); i++){
			values[remaining[i]] /= current_val; 
		}
	}
}

vector<pair<int, double> > rcv::make_column_0(int col, bool goes_up){
	//Makes 0 all the elements under the pivot (by adding linear combinations to the vector)
	//Returns the coefficients used to multiply in the identity
	vector<pair<int, double> > used;
	vector<double> values_temp(values);
	vector<int> rows_temp(rows);
	vector<int> cols_temp(columns);
	for(int i=0; i<rows_temp.size(); i++){
		if(goes_up){
			if(cols_temp[i] == col && rows_temp[i] < col){
				add(rows_temp[i], col, values_temp[i]*-1);
				used.push_back(make_pair(rows_temp[i], values_temp[i]));
			}
		}
		else{
			if(cols_temp[i] == col && rows_temp[i] > col){
				add(rows_temp[i], col, values_temp[i]*-1);
				used.push_back(make_pair(rows_temp[i], values_temp[i]));
			}
		}
	}	
	return used;
}

void rcv::make_column_0(int col, vector<pair<int, double> > &coef){
	//Makes 0 all the elements under the pivot (by adding linear combinations to the vector)
	//Used only for identity, multiplies by passed coefficients instead of own	

	vector<int> rows_temp(rows);
	vector<int> cols_temp(columns);

	for(int i=0; i<coef.size(); i++){
		add(coef[i].first, col, coef[i].second*-1);
	}	
}



rcv rcv::inverse(){
	//Returns the rcv of the inverse of the matrix
	
	vector<int> rows_temp(this->rows);
	vector<int> cols_temp(this->columns);
	vector<double> vals_temp(this->values);

	rcv id = this->create_identity();

	if(dimension.first != dimension.second){
		cerr<<"no se puede inversa, dims no iguales";
		throw "inv";
	}


	for(int i=0; i<dimension.first; i++){
		this->print_matrix();

		cerr<<" ITER "<< i<<endl;
		int r = this->pivot_swap(i);
		id.swap_rows(i, r);
			
		cerr<<"after swap"<<endl;
		this->print_matrix();

		double r2 = this->reduce(i);
		id.reduce(i, r2);

		cerr<<"after reduce"<<endl;
		this->print_matrix();

		vector<pair<int, double> > coefs = this->make_column_0(i, false);	
		id.make_column_0(i, coefs);
		cerr<<"after trim"<<endl;
		this->print_matrix();
	}

	this->print_matrix();

	for(int i=dimension.first-1; i>=1; i--){
		vector<pair<int, double> > coefs = this->make_column_0(i, true);
		id.make_column_0(i, coefs);
	}

	this->print_matrix();

	this->rows = rows_temp;
	this->columns = cols_temp;
	this->values = vals_temp;

	return id;
}

rcv rcv::create_identity(){
	//Returns an rcv representing an identity matrix with the dimensions of the original matrix

	if(dimension.first != dimension.second){
		cerr<<"identity cant"<<endl;
		cerr<<dimension.first<<" x "<<dimension.second<<endl;
		throw "no se puede hacer identidad";
	}
	else{	
		vector<int> rows_temp(dimension.first);
		vector<int> cols_temp(dimension.second);
		vector<double> vals_temp(dimension.first, 1);
		for(int i=0; i<dimension.first; i++){
			rows_temp[i] = i;
			cols_temp[i] = i;
		}

		return rcv(rows_temp, cols_temp, vals_temp, make_pair(dimension.first, dimension.second));
	}	
}

void rcv::print(){
	//Prints the matrix in rcv form

	cout<<"ROW\tCOL\tVAL"<<endl;
	for(int i=0; i<rows.size(); i++){
		cout<<rows[i]<<"\t"<<columns[i]<<"\t"<<values[i]<<endl;
	}
}

void rcv::print_matrix(){
	//Prints the matrix in matrix form

	vector<vector<double> > M(dimension.first, vector<double>(dimension.second));
	for(int i=0; i<rows.size(); i++){
		M[rows[i]][columns[i]] = values[i];
	}
	cout<<"{";
	for(int i=0; i<dimension.first; i++){
		if(i != 0) cout<<" ";
		cout<<"[";
		for(int j=0; j<dimension.second; j++){
			cout<<M[i][j]<<", ";
		}
		cout<<"]";
		if(i == dimension.first-1) cout<<"}";
		cout<<endl;
	}
}

pair<int, int> rcv::size(){
	//Returns a pair with the calculated dimension of the matrix
	return dimension;
}
