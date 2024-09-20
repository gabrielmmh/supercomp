### Relatório: Análise de Heurísticas para o Problema da Mochila

#### Introdução
Neste relatório, implementamos e testamos duas heurísticas para resolver o problema da mochila: **Mais Caro Primeiro** e **Mais Leve Primeiro**. O objetivo é comparar essas heurísticas em termos de eficiência e qualidade das soluções, utilizando três arquivos de entrada e comparando com o gabarito fornecido.

#### Heurísticas Implementadas
1. **Mais Caro Primeiro**: Prioriza a inclusão dos itens de maior valor, ignorando itens que não cabem na mochila.
2. **Mais Leve Primeiro**: Prioriza a inclusão dos itens mais leves, tentando maximizar a quantidade de itens na mochila.

#### Resultados Obtidos

##### Entrada 1:
- **Gabarito**: Valor = 601
- **Mais Caro Primeiro**: Valor = 73
- **Mais Leve Primeiro**: Valor = 46

##### Entrada 2:
- **Gabarito**: Valor = 870
- **Mais Caro Primeiro**: Valor = 321
- **Mais Leve Primeiro**: Valor = 361

##### Entrada 3:
- **Mais Caro Primeiro**: Valor = 1109
- **Mais Leve Primeiro**: Valor = 1162

#### Análise
- **Entrada 1**: Ambas as heurísticas falharam em se aproximar do valor ótimo, sugerindo que itens valiosos e leves não foram combinados de forma eficiente.
- **Entrada 2**: As duas heurísticas apresentaram resultados similares, com "Mais Leve Primeiro" levemente superior.
- **Entrada 3**: "Mais Leve Primeiro" foi superior, indicando que, neste caso, priorizar quantidade de itens foi uma abordagem mais eficaz.

#### Conclusão
As heurísticas **Mais Caro Primeiro** e **Mais Leve Primeiro** proporcionam soluções rápidas, mas longe dos valores ótimos, principalmente nas primeiras entradas. Embora eficientes em termos de tempo, essas abordagens falham em situações mais complexas, sugerindo que soluções exaustivas ou heurísticas mais avançadas são necessárias para atingir o valor ótimo. Ambas as heurísticas são limitadas e seu desempenho depende das características dos itens disponíveis.