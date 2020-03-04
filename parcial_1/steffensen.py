# Método de Steffensen para encontrar raíces de funciones
import math

def g(f, x_0):
    """Función que retorna el valor de g(x) dados un f(x) y un x"""
    return lambda x_0: f(x_0 + f(x_0))/f(x_0) -1

def steffensen(f, x_0, tol=0.01, max_iter=20, debug=False):
    g_local = g(f, x_0)
    x_i = x_0
    for i in range(max_iter):
        # Guardo g(x_i) y f(x_i) para no calcularlos de nuevo
        g_x_i = g_local(x_i)
        f_x_i = f(x_i)
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
    f = lambda x: x**10-10*x+4
    x_0 = -1
    print(steffensen(f, x_0, 0, 20, debug=True))

