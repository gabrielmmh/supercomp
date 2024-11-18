# Projeto de Supercomputação 2024.2: Análise de Redes Sociais

## Introdução

O objetivo deste projeto é encontrar a **clique máxima** em um grafo, um subconjunto onde todos os vértices estão diretamente conectados entre si. Essa análise é fundamental para entender grupos coesos em redes sociais e tem aplicações práticas em diversas áreas, como segmentação de mercado e análise de comunidades.

Devido à sua complexidade computacional, especialmente para grafos grandes, o problema será abordado de três formas: (1) uma abordagem exaustiva, (2) uma paralelização com OpenMP e (3) uma distribuição com MPI. Cada método será avaliado em termos de eficiência, considerando o tempo de execução e o tamanho do grafo suportado. Espera-se que as abordagens paralelas apresentem melhor desempenho e permitam a análise de grafos maiores. Além disso, heurísticas e técnicas de otimização serão exploradas para aprimorar a execução.

Os resultados serão apresentados por meio de gráficos e tabelas, destacando o ganho de eficiência e as diferenças entre as abordagens.

---

## Estrutura do Projeto

- `/src`: Contém o código-fonte das implementações.

- `/input`: Arquivos de entrada (grafos gerados para testes).

- `/output`: Resultados gerados pelas implementações.

- `/reports`: Gráficos e tabelas para a análise de resultados.

- `README.md`: Este arquivo, contendo o relatório completo do projeto.

---

## Implementação

### Geração do Grafo

A geração do grafo será feita utilizando a biblioteca `networkx` em Python. Os grafos serão representados como listas de adjacência e salvos no formato:

```
número_de_vértices número_de_arestas
vértice1 vértice2
vértice1 vértice3
...
```

O código correspondente pode ser encontrado em `/src/generate_graph.py`.
