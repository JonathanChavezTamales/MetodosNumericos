#include <iostream>
#include <cmath>
#include <complex>
#include <vector>

std::vector<std::complex<double> > moivre(double a, double n){
	std::vector<std::complex<double> > raices(n);
	double r = abs(a); //a al pasarlo a r solo se potencia y se saca raiz ya que no tiene imaginario, por lo que queda abs(a)
	double theta = M_PI; //Debido a que nunca hay b porque a (ecuacion) siempre es un real.
	if(a > 0){
		theta += M_PI;
	}

	for(int i=0; i<n; i++){
		std::complex<double> mv(cos((theta+ 2*M_PI*i)/n), sin((theta + 2*M_PI*i)/n));
		std::complex<double> res = pow(r, 1.0/n)*mv; //Al parecer habia error, r de más
		//Redondeo a 3 decimales
		res.real(round(res.real() * 1000) / 1000);
		res.imag(round(res.imag() * 1000) / 1000);
		raices[i] = res;
	}
	return raices;
}

int main(){
	int n;
	double a;
	std::cout<<"Soluciona x^n-a = 0"<<std::endl;
	std::cout<<"Ingrese n y a (separados por linea o espacio)"<<std::endl;
	std::cin>>n>>a;
	std::vector<std::complex<double> >mv = moivre(a, n);
	for(int i=0; i<n; i++){
		std::cout<<"Raíz "<<i<<": "<<mv[i].real()<<" + i("<<mv[i].imag()<<")"<<std::endl;
	}
}
