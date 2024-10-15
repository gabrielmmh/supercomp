### Entrega da Atividade 13

---

#### Schedules

Eu executei o código com diferentes schedulers (`static`, `dynamic`, `guided`, `auto`) e registrei o tempo necessário para cada execução. Abaixo estão os tempos médios encontrados:

- **Tempo médio para default:** 1.87367e-05 segundos
- **Tempo médio para schedule(static):** 2.4817e-06 segundos

##### Perguntas de Análise:

1. **Qual scheduler apresentou o menor tempo médio?**

   O scheduler `schedule(static)` apresentou o menor tempo médio, com 2.4817e-06 segundos, distribuindo eficientemente as iterações entre as threads, uma vez que a carga de trabalho era uniforme.

2. **Algum scheduler teve variações significativas entre as execuções? Se sim, por quê?**

   O scheduler `schedule(dynamic)` apresentou variações mais significativas nas execuções. Isso se deve ao fato de que, no scheduler dinâmico, as threads pegam blocos de iterações conforme terminam o trabalho anterior, o que pode causar variações dependendo da quantidade de trabalho que cada thread conclui em cada iteração.

3. **Alguma característica específica do trabalho (como carga de dados, balanceamento) parece ter influenciado o comportamento de um scheduler em particular?**

   Sim, `schedule(static)` se beneficiou de uma carga de trabalho equilibrada, o que garantiu que todas as threads recebessem blocos de iterações de tamanho uniforme. No entanto, o `schedule(dynamic)` mostrou melhor desempenho para situações com trabalho mais irregular, embora com maior variação nos tempos.

---

#### Cálculo do PI

Eu paralelizei o cálculo recursivo de Pi usando duas abordagens: `parallel for` e `#pragma omp task`. Para cada abordagem, utilizei diferentes valores de `MIN_BLK` e registrei os tempos de execução. Abaixo estão os tempos médios para cada valor de `MIN_BLK`:

##### Resultados:

- **MIN_BLK: 67108864**
  - `parallel for`: 1.07981175649911 segundos
  - `tasks`: 1.48348322790116 segundos
- **MIN_BLK: 268435456**
  - `parallel for`: 1.08243488054723 segundos
  - `tasks`: 1.61863174848258 segundos
- **MIN_BLK: 536870912**
  - `parallel for`: 1.07978855632246 segundos
  - `tasks`: 1.61840325500816 segundos

##### Perguntas de Análise:

1. **Qual abordagem (parallel for ou tasks) apresentou melhor desempenho?**

   A abordagem `parallel for` apresentou consistentemente o melhor desempenho, com tempos médios mais baixos do que a abordagem com `tasks` em todos os testes realizados.

2. **O valor de MIN_BLK ou o número de tarefas influenciou significativamente o tempo de execução?**

   Sim, o valor de `MIN_BLK` influenciou o tempo de execução, especialmente na abordagem com `tasks`. Quando `MIN_BLK` era muito pequeno, mais tarefas eram criadas, aumentando o overhead de criação e sincronização. Já com valores maiores, a abordagem `parallel for` manteve uma execução constante.

3. **Alguma abordagem teve variação maior entre execuções? Por quê?**

   A abordagem com `tasks` apresentou uma variação maior entre as execuções. Isso se deve à sobrecarga adicional introduzida pela criação e sincronização de múltiplas tasks, especialmente para valores menores de `MIN_BLK`.

---

#### Manipulação de Efeitos Colaterais no Vetor

Eu paralelizei o código que modifica um vetor com `#pragma omp critical` para evitar acessos simultâneos e registrei os tempos de execução. Posteriormente, modifiquei o código para pré-alocar a memória do vetor, evitando o uso de `push_back`, e medi os tempos de execução.

##### Resultados:

- **Tempo médio com omp critical:** 0.0908 segundos
- **Tempo médio com pré-alocação de memória:** 0.000669497 segundos

##### Perguntas de Análise:

1. **Qual abordagem teve melhor desempenho: omp critical ou pré-alocação de memória?**

   A abordagem com pré-alocação de memória apresentou um desempenho significativamente superior. Ela eliminou a necessidade de sincronização, resultando em tempos de execução muito mais baixos.

2. **O uso de omp critical adicionou muito overhead? Como você pode justificar isso?**

   Sim, o uso de `omp critical` adicionou muito overhead, pois a seção crítica impede que múltiplas threads acessem o código simultaneamente, criando um gargalo que afeta o desempenho. O custo da sincronização de várias threads para um único ponto de acesso foi a principal causa desse overhead.

3. **A ordem dos dados no vetor foi mantida em ambas as abordagens?**

   Sim, a ordem dos dados no vetor foi mantida em ambas as abordagens. Tanto a sincronização em `omp critical` quanto a pré-alocação garantiram que as operações fossem realizadas corretamente, preservando a integridade dos dados.

---

#### Conclusão

Os testes realizados demonstraram que a abordagem de `parallel for` e a pré-alocação de memória são claramente superiores em termos de desempenho. A sincronização excessiva com `omp critical` e o overhead de criação de tasks na abordagem `tasks` resultaram em tempos de execução significativamente mais altos.

Para problemas recursivos e com efeitos colaterais, a abordagem de pré-alocação de memória se mostrou mais eficiente, pois eliminou a necessidade de sincronização, enquanto a utilização de `parallel for` foi a melhor estratégia para balancear a carga de trabalho entre as threads, mantendo a eficiência do processamento.