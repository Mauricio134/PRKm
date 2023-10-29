import matplotlib.pyplot as plt

with open('output.txt', 'r') as archivo:
    # Inicializa dos listas vacías
    lista1 = []
    lista2 = []

    # Lee el archivo línea por línea
    for linea in archivo:
        # Divide la línea en dos valores y los convierte a enteros
        valor1, valor2 = map(float, linea.strip().split())

        # Agrega los valores a las listas correspondientes
        lista1.append(valor1)
        lista2.append(valor2)


fig, ax = plt.subplots()
ax.plot(lista1, lista2)
plt.show()