import time

def multiply_matrices(A, B):
    result = [[0 for _ in range(len(B[0]))] for _ in range(len(A))]
    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(B)):
                result[i][j] += A[i][k] * B[k][j]
    return result

# Tamanhos das matrizes para teste
sizes = [300, 900, 1300]

# Lista para armazenar os tempos de execução
times_python = []

for N in sizes:
    A = [[i + j for j in range(N)] for i in range(N)]
    B = [[i * j for j in range(N)] for i in range(N)]

    start_time = time.time()
    result = multiply_matrices(A, B)
    end_time = time.time()

    execution_time = end_time - start_time
    times_python.append(execution_time)
    print(f"Tamanho da matriz: {N}x{N}, Tempo de execução em Python: {execution_time:.2f} segundos")
