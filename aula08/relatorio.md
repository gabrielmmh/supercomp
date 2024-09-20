## Relatório de Análise de Algoritmos para o Problema da Mochila

### Introdução

O problema da mochila é um clássico problema de otimização combinatória, onde o objetivo é maximizar o valor total dos itens que podem ser colocados em uma mochila, sem exceder a capacidade máxima de peso da mochila. Neste relatório, comparo três algoritmos diferentes para resolver o problema da mochila: **Mochila Cheia (Full)**, **Substituição de Objeto (Substitution)** e **Hill Climbing**. Utilizei três arquivos de entrada diferentes (`entrada1.txt`, `entrada2.txt` e `entrada3.txt`) para testar a eficiência e a eficácia de cada algoritmo.

### Metodologia

Três algoritmos foram implementados:

1. **Mochila Cheia (Full)**: Este algoritmo gera uma solução inicial aleatória e preenche a mochila com itens até que não seja possível adicionar mais nenhum item sem exceder a capacidade.

2. **Substituição de Objeto (Substitution)**: Começa com uma solução inicial e tenta melhorar iterativamente substituindo um item da mochila por outro que aumente o valor total, respeitando a capacidade da mochila.

3. **Hill Climbing**: Este algoritmo começa com uma solução inicial e explora "vizinhos" da solução atual, ou seja, pequenas modificações na seleção de itens, buscando aumentar o valor total sem exceder a capacidade.

### Resultados

Os resultados para cada algoritmo e cada arquivo de entrada foram registrados da seguinte maneira:

| Arquivo de Entrada | Algoritmo        | Valor Máximo | Tempo Real (s) |
|--------------------|------------------|--------------|----------------|
| entrada1.txt       | Full             | 164          | 0.058          |
| entrada1.txt       | Substitution     | 164          | 0.051          |
| entrada1.txt       | Hill Climbing    | 164          | 0.051          |
| entrada2.txt       | Full             | 206          | 0.058          |
| entrada2.txt       | Substitution     | 206          | 0.049          |
| entrada2.txt       | Hill Climbing    | 206          | 0.050          |
| entrada3.txt       | Full             | 23           | 0.059          |
| entrada3.txt       | Substitution     | 23           | 0.050          |
| entrada3.txt       | Hill Climbing    | 23           | 0.049          |

### Análise dos Resultados

1. **Valores Máximos Obtidos**: Os três algoritmos, **Full**, **Substitution**, e **Hill Climbing**, encontraram o mesmo valor máximo para todos os arquivos de entrada. Isso sugere que, para os casos testados, todos os algoritmos conseguiram encontrar uma solução ótima ou perto do ótimo.

2. **Tempos de Execução**: Todos os algoritmos apresentaram tempos de execução muito semelhantes para cada entrada. O algoritmo **Substitution** foi ligeiramente mais rápido na maioria dos casos, mas a diferença é mínima e pode ser considerada negligenciável para o tamanho dos dados testados.

### Conclusão

Os resultados indicam que todos os três algoritmos são eficazes em encontrar a solução ótima ou perto da ótima para o problema da mochila nas entradas testadas. No entanto, é importante notar que a eficiência de cada algoritmo pode variar com a complexidade e o tamanho das entradas. Para entradas maiores ou mais complexas, o algoritmo de **Hill Climbing** ou a **Substituição de Objeto** pode oferecer vantagens significativas em termos de eficiência, pois ambos tentam melhorar iterativamente uma solução, enquanto o algoritmo **Full** simplesmente preenche a mochila até não ser mais possível adicionar itens.

Além disso, a diferença mínima nos tempos de execução pode ser mais pronunciada com um número maior de itens e uma capacidade de mochila maior, o que poderia destacar ainda mais a eficiência dos algoritmos de busca local (Hill Climbing e Substitution) em comparação com o algoritmo básico (Full).

