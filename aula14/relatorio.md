### **Relatório - Cálculo de Pi com Monte Carlo**

#### **Parte 1: Implementação Sequencial**
Na implementação sequencial, o código gerou pontos aleatórios para calcular uma estimativa de Pi. O valor obtido foi **3.14169792**, com um tempo de execução de **2.541818966 segundos**. 

**Reflexão:**  
A sequência de números aleatórios foi gerada corretamente, e a estimativa de Pi está próxima do valor real, o que sugere que o algoritmo é eficaz. A implementação foi relativamente simples, sem dificuldades maiores, já que não houve necessidade de lidar com condições de corrida ou sincronização.

---

#### **Parte 2: Primeira Tentativa de Paralelização**
Nesta primeira tentativa, utilizei uma abordagem com `#pragma omp parallel for` para distribuir a geração de pontos entre várias threads. O valor estimado foi **3.1418408**, com um tempo de execução de **16.135888291 segundos**.

**Reflexão:**  
Um obstáculo significativo na paralelização foi a geração de números aleatórios. Como o gerador foi compartilhado entre várias threads, houve interferência e sincronização, afetando o desempenho. A redução de desempenho ocorre porque, mesmo com threads trabalhando simultaneamente, a proteção do acesso ao gerador introduziu overhead.

---

#### **Parte 3: Melhorando a Paralelização**
A melhoria introduzida nesta fase foi fornecer um gerador de números aleatórios para cada thread, com seeds únicos. O valor estimado foi **3.14162468**, e o tempo de execução foi **4.746257418 segundos**.

**Reflexão:**  
A distribuição de geradores independentes para cada thread melhorou o desempenho, eliminando a necessidade de sincronização constante. A estimativa de Pi continuou precisa, indicando que o método não foi comprometido. O tempo de execução da versão paralela melhorada foi significativamente menor em comparação à primeira tentativa, mas ainda um pouco maior do que a versão sequencial.

---

### **Tabela de Comparação**

| Versão                  | Valor Estimado de Pi | Tempo de Execução (s) |
|-------------------------|----------------------|-----------------------|
| Sequencial              | 3.14169792           | 2.541818966           |
| Primeira Paralelização  | 3.14184080           | 16.135888291          |
| Paralelização Melhorada | 3.14162468           | 4.746257418           |

---

### **Conclusão e Comparação**

1. **Houve uma melhoria significativa no tempo de execução entre a versão sequencial e as versões paralelas?**  
   - A primeira tentativa de paralelização foi mais lenta do que a versão sequencial, devido ao overhead na sincronização dos geradores de números aleatórios. No entanto, a versão paralela com geradores independentes foi mais rápida que a primeira tentativa, mas ainda mais lenta que a versão sequencial.

2. **A estimativa de Pi permaneceu precisa em todas as versões?**  
   - Sim, todas as versões forneceram valores de Pi próximos ao real, com pequenas variações dentro da margem esperada para algoritmos baseados em Monte Carlo.

3. **Quais foram os maiores desafios ao paralelizar o algoritmo, especialmente em relação aos números aleatórios?**  
   - O maior desafio foi evitar condições de corrida na geração de números aleatórios. Usar um gerador compartilhado causou degradação do desempenho devido à necessidade de sincronização. A solução foi fornecer geradores independentes para cada thread, o que eliminou esse problema.

4. **O uso de threads trouxe benefícios claros para este problema específico?**  
   - Sim, mas apenas na versão paralela melhorada. A primeira tentativa mostrou que a paralelização sem uma estratégia adequada pode resultar em desempenho pior. A abordagem com geradores independentes, por sua vez, melhorou o desempenho, embora a vantagem não tenha sido tão significativa quanto esperado para este problema devido ao overhead inerente à criação e coordenação de threads.