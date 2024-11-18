import networkx as nx
import os

def gerar_grafo(nome_arquivo, num_vertices=100, probabilidade_conexao=0.7):
    """
    Gera um grafo aleatório densamente conectado e salva em um arquivo.
    
    Args:
        nome_arquivo (str): Caminho para salvar o arquivo do grafo.
        num_vertices (int): Número de vértices no grafo.
        probabilidade_conexao (float): Probabilidade de conexão entre dois vértices.
    """
    # Criar o grafo
    grafo = nx.fast_gnp_random_graph(num_vertices, probabilidade_conexao)

    # Garantir que o diretório existe
    os.makedirs(os.path.dirname(nome_arquivo), exist_ok=True)

    # Salvar o grafo no formato esperado
    with open(nome_arquivo, 'w') as arquivo:
        arquivo.write(f"{num_vertices} {grafo.number_of_edges()}\n")
        for aresta in grafo.edges():
            arquivo.write(f"{aresta[0] + 1} {aresta[1] + 1}\n")  # +1 para índices começarem em 1

    print(f"Grafo gerado com {num_vertices} vértices e salvo em '{nome_arquivo}'.")

# Exemplo de uso
if __name__ == "__main__":
    gerar_grafo("../input/grafo.txt", num_vertices=100, probabilidade_conexao=0.7)
