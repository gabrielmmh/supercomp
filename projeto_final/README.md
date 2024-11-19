# Projeto de Supercomputação 2024.2: Análise de Redes Sociais

## Introdução

O objetivo deste projeto foi encontrar a **clique máxima** em um grafo, ou seja, um subconjunto de vértices onde todos estão diretamente conectados entre si. Esse problema é essencial na análise de redes sociais, permitindo identificar grupos coesos com aplicações práticas em áreas como segmentação de mercado e detecção de comunidades. No entanto, devido à sua complexidade computacional, especialmente em grafos grandes, diferentes abordagens foram exploradas para lidar com o problema de maneira eficiente.

Foram implementadas quatro abordagens distintas: (1) uma abordagem exaustiva sequencial, (2) uma paralelização com OpenMP, (3) uma distribuição de processamento com MPI e (4) uma combinação de MPI e OpenMP. O projeto também incluiu a exploração de heurísticas para otimizar a abordagem exaustiva e reduzir o número de combinações analisadas. Os resultados demonstraram diferenças importantes entre as abordagens, especialmente no impacto do overhead de comunicação no MPI e nos ganhos proporcionados pelo OpenMP.

---

## Estrutura e Implementação

O projeto foi estruturado em diretórios, com `/src` contendo os códigos-fonte das implementações, `/input` armazenando os grafos utilizados nos testes e `/outputs` guardando os resultados das execuções. Scripts SLURM para execução em clusters foram organizados em `/submits`, e este relatório está centralizado no arquivo `README.md`.

Os grafos utilizados foram gerados com o script `generate_graph.py`, utilizando a biblioteca NetworkX, e representados como listas de adjacência no formato:

```plaintext
número_de_vértices número_de_arestas
vértice1 vértice2
vértice3 vértice4
...
```

Foram utilizados os seguintes grafos para os testes:
- **`grafo_10.txt`**: 10 vértices, densidade média.
- **`grafo_50.txt`**: 50 vértices, densidade alta.
- **`grafo_100.txt`**: 100 vértices, densidade alta.

---

## Abordagem Exaustiva e Heurísticas de Poda

A abordagem exaustiva, implementada em `exhaustive.cpp`, realiza uma busca completa para encontrar a clique máxima. Embora funcional, sua natureza combinatória limita o desempenho em grafos maiores. Durante os testes, observou-se que o tempo de execução é aceitável para grafos com até **100 vértices**, mas extrapolando os resultados, estimamos que a abordagem se torna inviável para grafos com **mais de 500 vértices**, considerando o limite de 15 minutos para execução.

Para melhorar o desempenho, implementamos uma heurística de poda que reduz o espaço de busca. A ideia é evitar explorar combinações que não podem resultar em uma clique maior do que a já encontrada. O pseudo-código da heurística é apresentado a seguir:

```plaintext
Função EncontrarCliqueMaxima(grafo, numVertices, cliqueAtual, melhorClique)
    Se tamanho(cliqueAtual) + (numVertices - verticeAtual) <= tamanho(melhorClique) Então
        Retorne  # Poda: Não é possível formar uma clique maior que a atual
    Fim Se

    Se tamanho(cliqueAtual) > tamanho(melhorClique) Então
        melhorClique = cliqueAtual
    Fim Se

    Para i de verticeAtual até numVertices - 1 Faça
        podeAdicionar = Verdadeiro
        Para cada v em cliqueAtual Faça
            Se grafo[v][i] == 0 Então
                podeAdicionar = Falso
                Pare o loop
            Fim Se
        Fim Para

        Se podeAdicionar for Verdadeiro Então
            cliqueAtual.Adicionar(i)
            EncontrarCliqueMaxima(grafo, numVertices, cliqueAtual, melhorClique, i + 1)
            cliqueAtual.Remover(i)
        Fim Se
    Fim Para
Fim Função
```

Essa heurística demonstrou ser eficaz em reduzir o número de combinações analisadas, particularmente em grafos densos.

---

## Resultados e Análise

Os resultados das execuções, com tempos médios por grafo e abordagem, estão apresentados na tabela abaixo:

| Grafo       | Exaustiva (ms) | OpenMP (ms) | MPI (ms) | MPI + OpenMP (ms) |
|-------------|----------------|-------------|----------|-------------------|
| `grafo_10`  | 0              | 0           | 0        | 0                 |
| `grafo_50`  | 27             | 6           | 20       | 7                 |
| `grafo_100` | 10.750         | 2.601       | 9.904    | 3.460             |

A abordagem exaustiva foi a mais lenta, especialmente em grafos grandes, mas serviu como base para medir os ganhos proporcionados pelas técnicas de paralelismo. O OpenMP se destacou como a solução mais eficiente, reduzindo o tempo para **2.601 ms** no `grafo_100.txt`, enquanto o MPI teve desempenho limitado pelo overhead de comunicação, com tempo médio de **9.904 ms** para o mesmo grafo. A combinação MPI + OpenMP conseguiu melhorar o desempenho em relação ao MPI isolado, mas o custo adicional de comunicação ainda limitou os ganhos.

---

## Conclusão

Este projeto demonstrou que o desempenho de abordagens paralelas depende fortemente da natureza do problema e da infraestrutura utilizada. A abordagem exaustiva, embora funcional, é limitada para grafos maiores, mas a inclusão de heurísticas de poda mostrou-se uma estratégia eficaz para otimizar o processo. A paralelização com OpenMP foi a solução mais eficiente, especialmente para tarefas executadas em uma única máquina, enquanto o MPI apresentou ganhos mais modestos devido ao overhead de comunicação.

### Recomendações

1. **Priorizar OpenMP para problemas paralelizáveis localmente.**
2. **Utilizar MPI para problemas que justifiquem o custo de comunicação.**
3. **Explorar melhorias no algoritmo exaustivo, como heurísticas adicionais e critérios de poda.**
4. **Evitar sincronizações excessivas em problemas distribuídos com alto grau de dependência.**

Este projeto evidenciou como diferentes abordagens paralelas podem ser ajustadas às características do problema e da infraestrutura, contribuindo para um uso mais eficiente dos recursos computacionais.
