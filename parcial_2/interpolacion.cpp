#include <iostream>
#include <vector>

using namespace std;

int fac(int k){
	int res = 1;
	if(k==0) return 1;
	for(int i=1; i<=k; i++) res *= i;
	return res;
}

int main(){
	int n; // Numero de puntos
	cout<<"ingrese numero de puntos: ";
	cin>>n;
	vector<double> x(n);
	vector<double> y(n);
	double xk;
	cout<<"ingrese el xk objetivo: ";
	cin>>xk;
	
	vector<vector<double> > dif(2*n, vector<double>(n+1, 0.0));
	
	cout<<"ingrese los pares x y, separados por espacio, cada par separado por linea: ";
	for(int i=0; i<n; i++){
		cin>>x[i]>>y[i];
		if(i>0){
			dif[i][0] = y[i]-y[i-1];
		}
		for(int j=1; j<i; j++){
			dif[i][j] = dif[i][j-1]-dif[i-1][j-1];
		}	
	}

	double h = x[1]-x[0];

	auto it = lower_bound(x.begin(), x.end(), xk);
	it--;

	int idx = it-x.begin();
	double x0 = x[idx];
	double xf = x[idx+1];

	double k = (xk - x0)/h;

	double yk = 0;


	for(int i=n; i<2*n; i++){
		for(int j=n-1; j>=0; j--){
			dif[i][j] = dif[i-1][j] + dif[i][j+1];
		}
	}
	
	for(int i=idx, r=0; i<n+idx; i++, r++){
		double k2 = 1;
		for(int j=0; j<r; j++) k2 *= k-j;
		double f = fac(r);
		double di = i == idx ? y[idx] : dif[i][r-1];
			
		yk += k2*di/f;
		if(i != idx)
			cout<<"grado "<<r<<": "<<yk<<endl;
	}


}
