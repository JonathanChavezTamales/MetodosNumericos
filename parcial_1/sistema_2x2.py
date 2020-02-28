def soluciona(a,b,c,d,e,f):
    """Resuelve un sistema de ecuaciones 2x2
        Retorna la tupla (x1, x2)
    """
    det_s = a*e - b*d
    if det_s == 0:
        raise ValueError("No hay solucion o infinitas soluciones")
    else:
        det_x1 = c*e - b*f
        det_x2 = a*f - c*d
        x1 = det_x1/det_s
        x2 = det_x2/det_s

        return (x1, x2)


if __name__ == '__main__':
    print("Resuelve un sistema de ecuaciones 2x2 de la forma:")
    print("ax1 + bx2 = c")
    print("dx1 + ex2 = f\n")
    print("Ingrese sus 6 valores separados por coma:")
    try:
        datos = tuple(map(float,input().split(",")))
        x1, x2 = soluciona(datos[0], datos[1], datos[2],
                datos[3], datos[4], datos[5])
        print(x1,x2)
    except ValueError as error:
        print(error)

