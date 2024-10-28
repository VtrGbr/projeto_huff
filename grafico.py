import matplotlib.pyplot as plt

# Listas para armazenar os dados do arquivo
indices = []
comparacoes_heap = []
comparacoes_fila = []

# Leitura dos dados do arquivo
with open("valores.txt", "r") as file:
    for line in file:

        # Processar linhas de inserção, pois cada linha eh lida e dividida com o "split" Os três valores (índice, comparações da heap, e comparações da fila sem heap) são convertidos para inteiros.
        parts = line.strip().split()

        #verificar se ha 3 elementos

        if len(parts) == 3:
            # As comparações são armazenadas em listas separadas para os índices, comparações da heap, e da fila sem heap.
            index = int(parts[0])
            comparacao_heap = int(parts[1])
            comparacao_fila = int(parts[2])
            
            #Adicionar os valores as listas
            indices.append(index)
            comparacoes_heap.append(comparacao_heap)
            comparacoes_fila.append(comparacao_fila)

# Plotagem

#indices representa o eixo x, enquanto as comparações para heap e fila são plotadas no eixo y.
plt.figure(figsize=(10, 6))
plt.plot(indices, comparacoes_heap, label="Heap", color="blue", marker="o") #plt.plot é usado para criar gráficos de linha com diferentes cores e marcadores.
plt.plot(indices, comparacoes_fila, label="Fila sem heap", color="green", marker="x")

plt.xlabel("Numero de Inserções")
plt.ylabel("Comparações acumuladas")
plt.title("Número de Comparações Acumuladas por Estrutura")
plt.legend()
plt.grid(True)
plt.show()
