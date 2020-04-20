/*
 * RCV: RowColumnValue
 *
 */

#ifndef RCV_H
#define RCV_H
#endif

#include <vector>
using namespace std;

class rcv{
	private:
		vector<int> rows;
		vector<int> columns;
		vector<double> values;
		pair<int, int> dimension; 
	public:
		rcv(vector<int> &r, vector<int> &c, vector<double> &v, pair<int, int> dim);
		void swap_rows(int row_a, int row_b);
		void multiply(double scalar, int row_a);
		void add(int row_a, int row_b, double scalar);
		int pivot_swap(int row_a);
		double reduce(int step);
		void reduce(int step, double current_val);
		vector<double> make_column_0(int col);
		void make_column_0(int col, vector<double> &coef);
		void transpose();
		void invert();
		rcv create_identity();
		void print();
		void print_matrix();
		pair<int, int> size();
};
