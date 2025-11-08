import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os

# Configurações de visualização
plt.style.use('seaborn-v0_8-darkgrid')
sns.set_palette("husl")

# Verifica se o arquivo existe
arquivo_csv = 'resultados_ordenacao.csv'

if not os.path.exists(arquivo_csv):
    print("=" * 60)
    print("ERRO: Arquivo CSV nao encontrado!")
    print("=" * 60)
    print(f"Procurando por: {arquivo_csv}")
    print(f"Diretorio atual: {os.getcwd()}")
    print("\nArquivos .csv no diretorio atual:")
    arquivos_csv = [f for f in os.listdir('.') if f.endswith('.csv')]
    if arquivos_csv:
        for f in arquivos_csv:
            print(f"  - {f}")
    else:
        print("  Nenhum arquivo CSV encontrado")
    print("\n" + "=" * 60)
    print("SOLUCOES:")
    print("1. Execute o programa C primeiro para gerar o CSV")
    print("2. Certifique-se que o CSV esta no mesmo diretorio do script Python")
    print("3. Ou mova o script Python para o diretorio onde esta o CSV")
    print("=" * 60)
    exit(1)

# Lê o arquivo CSV
print(f"[OK] Arquivo encontrado: {arquivo_csv}")
df = pd.read_csv(arquivo_csv)

# Remove espaços extras dos nomes das colunas
df.columns = df.columns.str.strip()

# Exibe informações básicas
print("\n" + "=" * 60)
print("DATAFRAME - RESULTADOS DE ORDENACAO")
print("=" * 60)
print(df.head(20))
print(f"\nTotal de linhas: {len(df)}")
print(f"Colunas disponiveis: {df.columns.tolist()}")
print("\n")

# Calcula média por configuração (ignorando repetições)
df_media = df.groupby(['Tamanho', 'Tipo_Vetor', 'Algoritmo'])['Tempo'].mean().reset_index()
df_media.columns = ['Tamanho', 'Tipo_Vetor', 'Algoritmo', 'Tempo_Medio']

print("=" * 60)
print("TEMPO MEDIO POR CONFIGURACAO")
print("=" * 60)
print(df_media)
print("\n")

# Estatísticas descritivas
print("=" * 60)
print("ESTATISTICAS DESCRITIVAS (por algoritmo)")
print("=" * 60)
for alg in df['Algoritmo'].unique():
    print(f"\n{alg}:")
    print(df[df['Algoritmo'] == alg]['Tempo'].describe())

# ===================================================================
# VISUALIZAÇÕES - GRÁFICOS DE COLUNA
# ===================================================================

# Pivota os dados para facilitar plotagem
df_pivot = df_media.pivot_table(
    values='Tempo_Medio',
    index=['Tamanho', 'Tipo_Vetor'],
    columns='Algoritmo'
).reset_index()

# 1. Gráfico de colunas agrupadas por tipo de vetor
fig, axes = plt.subplots(1, 3, figsize=(18, 6))

tipos = df_media['Tipo_Vetor'].unique()
algoritmos = df_media['Algoritmo'].unique()
cores = {'InsertionSort': '#FF6B6B', 'MergeSort': '#4ECDC4', 'QuickSort': '#45B7D1'}

for idx, tipo in enumerate(tipos):
    ax = axes[idx]
    dados_tipo = df_media[df_media['Tipo_Vetor'] == tipo]
    
    tamanhos_unicos = sorted(dados_tipo['Tamanho'].unique())
    x = np.arange(len(tamanhos_unicos))
    width = 0.25
    
    for i, alg in enumerate(algoritmos):
        dados_alg = dados_tipo[dados_tipo['Algoritmo'] == alg].sort_values('Tamanho')
        tempos = [dados_alg[dados_alg['Tamanho'] == t]['Tempo_Medio'].values[0] 
                  if len(dados_alg[dados_alg['Tamanho'] == t]) > 0 else 0 
                  for t in tamanhos_unicos]
        
        ax.bar(x + i*width, tempos, width, label=alg, color=cores.get(alg, 'gray'), alpha=0.8)
    
    ax.set_xlabel('Tamanho do Vetor', fontsize=12)
    ax.set_ylabel('Tempo Medio (segundos)', fontsize=12)
    ax.set_title(f'Vetor {tipo}', fontsize=14, fontweight='bold')
    ax.set_xticks(x + width)
    ax.set_xticklabels(tamanhos_unicos, rotation=45)
    ax.legend()
    ax.grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('grafico_colunas_por_tipo.png', dpi=300, bbox_inches='tight')
print("\n[OK] Grafico salvo: grafico_colunas_por_tipo.png")

# 2. Comparação de algoritmos - um painel por algoritmo
fig, axes = plt.subplots(1, 3, figsize=(18, 6))

for idx, alg in enumerate(algoritmos):
    ax = axes[idx]
    dados_alg = df_media[df_media['Algoritmo'] == alg]
    
    tamanhos_unicos = sorted(dados_alg['Tamanho'].unique())
    x = np.arange(len(tamanhos_unicos))
    width = 0.25
    
    for i, tipo in enumerate(tipos):
        dados_tipo = dados_alg[dados_alg['Tipo_Vetor'] == tipo].sort_values('Tamanho')
        tempos = [dados_tipo[dados_tipo['Tamanho'] == t]['Tempo_Medio'].values[0] 
                  if len(dados_tipo[dados_tipo['Tamanho'] == t]) > 0 else 0 
                  for t in tamanhos_unicos]
        
        ax.bar(x + i*width, tempos, width, label=tipo, alpha=0.8)
    
    ax.set_xlabel('Tamanho do Vetor', fontsize=12)
    ax.set_ylabel('Tempo Medio (segundos)', fontsize=12)
    ax.set_title(f'{alg}', fontsize=14, fontweight='bold')
    ax.set_xticks(x + width)
    ax.set_xticklabels(tamanhos_unicos, rotation=45)
    ax.legend()
    ax.grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('grafico_colunas_por_algoritmo.png', dpi=300, bbox_inches='tight')
print("[OK] Grafico salvo: grafico_colunas_por_algoritmo.png")

# 3. Gráfico de linhas mostrando crescimento
fig, axes = plt.subplots(1, 3, figsize=(18, 6))

for idx, tipo in enumerate(tipos):
    ax = axes[idx]
    dados_tipo = df_media[df_media['Tipo_Vetor'] == tipo]
    
    for alg in algoritmos:
        dados_alg = dados_tipo[dados_tipo['Algoritmo'] == alg].sort_values('Tamanho')
        ax.plot(dados_alg['Tamanho'], dados_alg['Tempo_Medio'], 
                marker='o', linewidth=2, label=alg, color=cores.get(alg, 'gray'))
    
    ax.set_xlabel('Tamanho do Vetor', fontsize=12)
    ax.set_ylabel('Tempo Medio (segundos)', fontsize=12)
    ax.set_title(f'Crescimento - Vetor {tipo}', fontsize=14, fontweight='bold')
    ax.legend()
    ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('grafico_linhas_crescimento.png', dpi=300, bbox_inches='tight')
print("[OK] Grafico salvo: grafico_linhas_crescimento.png")

# 4. Boxplot mostrando variabilidade entre repetições
fig, axes = plt.subplots(1, 3, figsize=(18, 6))

for idx, alg in enumerate(algoritmos):
    ax = axes[idx]
    dados_alg = df[df['Algoritmo'] == alg]
    
    # Cria dados para boxplot
    dados_box = []
    labels_box = []
    
    for tamanho in sorted(dados_alg['Tamanho'].unique()):
        for tipo in tipos:
            dados_config = dados_alg[(dados_alg['Tamanho'] == tamanho) & 
                                     (dados_alg['Tipo_Vetor'] == tipo)]['Tempo']
            if len(dados_config) > 0:
                dados_box.append(dados_config.values)
                labels_box.append(f'{tamanho}\n{tipo[:3]}')
    
    bp = ax.boxplot(dados_box, labels=labels_box, patch_artist=True)
    
    # Colorir boxes
    for patch in bp['boxes']:
        patch.set_facecolor(cores.get(alg, 'gray'))
        patch.set_alpha(0.6)
    
    ax.set_xlabel('Tamanho e Tipo', fontsize=12)
    ax.set_ylabel('Tempo (segundos)', fontsize=12)
    ax.set_title(f'Variabilidade - {alg}', fontsize=14, fontweight='bold')
    ax.tick_params(axis='x', rotation=45, labelsize=8)
    ax.grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('grafico_boxplot_variabilidade.png', dpi=300, bbox_inches='tight')
print("[OK] Grafico salvo: grafico_boxplot_variabilidade.png")

# 5. Heatmap completo
plt.figure(figsize=(14, 10))

heatmap_data = df_pivot.set_index(['Tamanho', 'Tipo_Vetor'])[algoritmos]
sns.heatmap(heatmap_data, annot=True, fmt='.4f', cmap='YlOrRd', 
            cbar_kws={'label': 'Tempo Medio (s)'}, linewidths=0.5)
plt.title('Heatmap - Tempo Medio de Execucao', fontsize=14, fontweight='bold')
plt.tight_layout()
plt.savefig('heatmap_completo.png', dpi=300, bbox_inches='tight')
print("[OK] Grafico salvo: heatmap_completo.png")

# ===================================================================
# ANÁLISE DE COMPLEXIDADE
# ===================================================================

print("\n" + "=" * 60)
print("ANALISE DE COMPLEXIDADE (Razao de crescimento)")
print("=" * 60)

for alg in algoritmos:
    print(f"\n{alg}:")
    for tipo in tipos:
        dados = df_media[(df_media['Algoritmo'] == alg) & 
                         (df_media['Tipo_Vetor'] == tipo)].sort_values('Tamanho')
        tempos = dados['Tempo_Medio'].values
        tamanhos = dados['Tamanho'].values
        
        print(f"  {tipo}:")
        for i in range(1, len(tempos)):
            razao_tempo = tempos[i] / tempos[i-1] if tempos[i-1] > 0 else 0
            razao_tamanho = tamanhos[i] / tamanhos[i-1]
            print(f"    {tamanhos[i-1]:>8} -> {tamanhos[i]:>8}: "
                  f"Tempo x{razao_tempo:>6.2f} | Tamanho x{razao_tamanho:.1f}")

# Tabela resumo final
print("\n" + "=" * 60)
print("TABELA RESUMO - TEMPO TOTAL POR CONFIGURACAO")
print("=" * 60)
resumo = df_media.groupby(['Tipo_Vetor', 'Algoritmo'])['Tempo_Medio'].sum().unstack()
print(resumo)
print("\nTempo total geral por algoritmo:")
print(resumo.sum())

print("\n" + "=" * 60)
print("ANALISE CONCLUIDA!")
print("=" * 60)
print("Arquivos gerados:")
print("  - grafico_colunas_por_tipo.png")
print("  - grafico_colunas_por_algoritmo.png")
print("  - grafico_linhas_crescimento.png")
print("  - grafico_boxplot_variabilidade.png")
print("  - heatmap_completo.png")
print("=" * 60)