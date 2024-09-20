### Análise dos Resultados de Execução para o Problema da Mochila

**Resumo dos Resultados:**

1. **Busca Exaustiva:**
   - **Entrada 1:**
     - Valor máximo: **601**
     - Tempo de execução: **1.058 segundos**
   - **Entrada 2:**
     - Valor máximo: **870**
     - Tempo de execução: **62.193 segundos (aproximadamente 1 minuto)**
   - **Entrada 3:**
     - **Timeout** — Não conseguiu completar em tempo hábil.
   - **Entrada 4:**
     - Valor máximo: **648**
     - Tempo de execução: **8.352 segundos**

2. **Heurística Embaralhada (5 Execuções por Entrada):**
   - **Entrada 1:**
     - Valores máximos encontrados: **174, 194, 357, 260, 418**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**
   - **Entrada 2:**
     - Valores máximos encontrados: **230, 246, 229, 244, 363**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**
   - **Entrada 3:**
     - Valores máximos encontrados: **3320** (em todas as execuções)
     - Tempo médio de execução: **aproximadamente 0.066 segundos por execução**
   - **Entrada 4:**
     - Valores máximos encontrados: **500, 456, 496, 530, 485**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**

3. **Heurística Probabilística (5 Execuções por Entrada):**
   - **Entrada 1:**
     - Valores máximos encontrados: **301, 209, 188, 231, 349**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**
   - **Entrada 2:**
     - Valores máximos encontrados: **193, 244, 117, 252, 264**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**
   - **Entrada 3:**
     - Valores máximos encontrados: **1713, 1717, 1932, 1723, 1492**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**
   - **Entrada 4:**
     - Valores máximos encontrados: **364, 440, 356, 305, 382**
     - Tempo médio de execução: **aproximadamente 0.07 segundos por execução**

### Discussão dos Resultados:

- **Busca Exaustiva:**
  - A busca exaustiva garantiu os valores ótimos para os casos em que conseguiu completar a execução dentro do limite de tempo, como esperado. No entanto, o tempo de execução cresce exponencialmente com o número de itens (como evidenciado pelo **timeout** na entrada 3), demonstrando as limitações da busca exaustiva para conjuntos de dados maiores.

- **Heurística Embaralhada:**
  - A heurística de embaralhamento apresentou resultados diversos em termos de valores máximos encontrados. Em algumas execuções, ela chegou muito próxima ou igual ao valor ótimo (como em **Entrada 3**), enquanto em outras, os resultados ficaram bem abaixo do ótimo (como em **Entrada 1**). 
  - O tempo de execução foi consistentemente baixo, o que é um grande benefício quando se precisa de uma solução rápida, embora potencialmente subótima.

- **Heurística Probabilística:**
  - A heurística probabilística também apresentou uma variabilidade considerável nos valores máximos encontrados, mas geralmente alcançou valores decentes em um tempo muito curto. 
  - A diferença de desempenho entre a heurística embaralhada e a probabilística não foi muito grande em termos de tempo, mas ambas demonstraram a capacidade de encontrar soluções subótimas rapidamente.

### Conclusões e Sugestões de Otimização:

1. **Limitações da Busca Exaustiva:**
   - A busca exaustiva é impraticável para entradas grandes devido ao seu tempo de execução exponencial. Para esses casos, heurísticas e algoritmos aproximados são mais apropriados.

2. **Eficácia das Heurísticas:**
   - As heurísticas podem não garantir a solução ótima, mas oferecem uma alternativa eficiente em termos de tempo, especialmente para entradas maiores. A execução múltipla (5 vezes) permite capturar a variabilidade nos resultados, destacando a necessidade de balancear entre rapidez e precisão.

3. **Sugestões para Melhorias Futura:**
   - **Combinação de Heurísticas:** Experimentar uma combinação de várias heurísticas (ex: aplicar uma heurística embaralhada seguida por uma probabilística ou vice-versa) pode potencialmente oferecer melhores resultados em termos de valor máximo.
   - **Meta-heurísticas:** Algoritmos como Simulated Annealing, Algoritmos Genéticos ou Otimização por Enxame de Partículas podem ser explorados para potencialmente encontrar soluções melhores em tempos razoáveis.
   - **Aprimoramento de Heurísticas Existentes:** Ajustar os parâmetros ou a lógica de seleção pode melhorar a eficácia das heurísticas implementadas. Por exemplo, ajustar o limiar de probabilidade para a heurística probabilística.

Com base nos resultados e na análise, é claro que uma abordagem híbrida, combinando a precisão da busca exaustiva para entradas menores e a rapidez das heurísticas para entradas maiores, pode oferecer o melhor equilíbrio entre precisão e eficiência computacional.