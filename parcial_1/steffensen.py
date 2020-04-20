# Método de Steffensen para encontrar raíces de funciones
import math

def g(f, x_0):
    """Función que retorna el valor de g(x) dados un f(x) y un x"""
    return lambda x_0: f(x_0 + f(x_0))/f(x_0) -1

def steffensen(f, x_0, tol=0.01, max_iter=20, debug=False):
    if debug:
        print("-METODO DE STEFFENSEN-")
        print(f"tol: {tol}\nmax_iter: {max_iter}\nx_0:{x_0}")
        print("-----------------")
    g_local = g(f, x_0)
    x_i = x_0

    for i in range(max_iter):
        # Guardo g(x_i) y f(x_i) para no calcularlos de nuevo

        f_x_i = f(x_i)
        try:
            g_x_i = g_local(x_i)
        except:
            print("TERMINATED: Algorithm termination")
            return x_i

        if g_x_i == 0:
            if debug:
                print("TERMINATED: Algorithm termination")
            return x_i
        elif abs(f_x_i) <= tol:
            # Solución entra en tolerancia
            if debug:
                print("TERMINATED: f(x) <= tol")
            return x_i
        else: 
            prev_x = x_i
            x_i = x_i - f_x_i/g_x_i
            if abs(x_i) < 1E-9:
                en = math.inf
            else:
                en = abs((x_i-prev_x)/x_i)
                if en <= tol:
                    # Puede fallar con raiz falsa
                    if debug:
                        print("TERMINATED: en <= tol")
                    return x_i
            if debug:
                print("ITER: ", i, end="\t")
                print("APROX: ", x_i, end="\t")
                print("EN:" , en)

    if debug:
        print("TERMINATED: max_iters")
    return x_i


if __name__=='__main__':
    f = lambda x: .1*x**2-1
    x_0 = 0.2
    print(steffensen(f, x_0, 0.0001, 100, debug=True))

