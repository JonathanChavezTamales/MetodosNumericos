#include "rcv.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

rcv::rcv(vector<int> &r, vector<int> &c, vector<double> &v, pair<int, int> dim){
	rows = move(r);
	columns = move(c);
	values = move(v);
	dimension = dim;
}

void rcv::swap_rows(int row_a, int row_b){

	if(row_a >= dimension.first || row_b >= dimension.first || row_a < 0 || row_b < 0) return;

	//Store the pair(column, value) for each row;
	vector<pair<int, double> > a;
	vector<pair<int, double> > b;
	
	auto row_it = rows.begin();
	auto col_it = columns.begin();
	auto val_it = values.begin();

	while(row_it != rows.end()){
		if(*row_it == row_a || *row_it == row_b){
			pair<int, double> element = make_pair(*col_it, *val_it);
			if(*row_it == row_a){
				a.push_back(element);
			}
			else{
				b.push_back(element);
			}
			row_it = rows.erase(row_it);
			col_it = columns.erase(col_it);
			val_it = values.erase(val_it);
		}
		else{
			row_it++;
			col_it++;
			val_it++;
		}
	}

	//Swap elements
	for(int i=0; i<a.size(); i++){
		rows.push_back(row_b);
		columns.push_back(a[i].first);
		values.push_back(a[i].second);
	}

	for(int i=0; i<b.size(); i++){
		rows.push_back(row_a);
		columns.push_back(b[i].first);
		values.push_back(b[i].second);
	}

}

void rcv::multiply(double scalar, int row_a){
	for(int i=0; i<rows.size(); i++){
		if(rows[i] == row_a){
			values[i] *= scalar;
		}
	}
}

void rcv::add(int row_a, int row_b, double scalar){
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
	//Returns row swapped

	double max_val = INT_MIN;
	int max_row = -1;
	double current_val = INT_MIN;
	for(int i=0; i<rows.size(); i++){
		if(columns[i] == step && columns[i] == rows[i] && values[i] > current_val){
			current_val = values[i];
		}
		if(columns[i] == step && values[i] > max_val){
			max_val = values[i];
			max_row = rows[i];
		}
	}	
	if(max_row != step){
		this->swap_rows(step, max_row);
		return max_row;
	}
	return -1; // No swap
}

double rcv::reduce(int step){
	//Returns pivot which divides all row

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
	//Find indexes of all values in the same row, then update.
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

vector<double> rcv::make_column_0(int col){
	//Returns the coefficients used to multiply in the identity
	vector<double> values_temp(values);
	vector<int> rows_temp(rows);
	vector<int> cols_temp(columns);
	for(int i=0; i<rows_temp.size(); i++){
		if(cols_temp[i] == col && rows_temp[i] > col){
			add(rows_temp[i], col, values_temp[i]*-1);		
		}
	}	
	return values_temp;
}

void rcv::make_column_0(int col, vector<double> &coef){
	//Used only for identity, multiplies by passed coefficients instead of own
	vector<int> rows_temp(rows);
	vector<int> cols_temp(columns);
	for(int i=0; i<rows_temp.size(); i++){
		if(cols_temp[i] == col && rows_temp[i] > col){
			cerr<<"adding to row"<<rows_temp[i]<<" row"<<col<<" * -"<<coef[i]<<endl;
			add(rows_temp[i], col, coef[i]*-1);		
		}
	}	
}

void rcv::transpose(){
	rows.swap(columns);
}

void rcv::invert(){
	cout<<"caca";
}

rcv rcv::create_identity(){
	if(dimension.first != dimension.second){
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
	cout<<"ROW\tCOL\tVAL"<<endl;
	for(int i=0; i<rows.size(); i++){
		cout<<rows[i]<<"\t"<<columns[i]<<"\t"<<values[i]<<endl;
	}
}

void rcv::print_matrix(){
	vector<vector<double> > M(dimension.first, vector<double>(dimension.second));
	cerr<<"a";
	for(int i=0; i<rows.size(); i++){
		M[rows[i]][columns[i]] = values[i];
	}
	cerr<<"b";
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
	return dimension;
}
