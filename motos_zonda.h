#ifndef MOTOS_ZONDA_H
#define MOTOS_ZONDA_H
#define MAX_SERVICOS 100

// Incluindo as bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//estrutura para o Historico Financeiro
typedef struct {
    char Data[11];  // DD/MM/AAAA + \0
    float ValorTotal;
} HistoricoFinanceiro;

//estrutura de dados para os registros das motos
struct RegMoto {
    char Nome[100];
    char Modelo[50];
    char Placa[10];
    char Defeito[200];
    char Status;
    float Preco;
};
typedef struct RegMoto TpRegMoto;

// Declaração das funções
void SolicitaServico(); //1-solicitar um novo serviço
void IniciaServico(); // Iniciar um serviço
void RemoverSolicitacao(); // Remover a soliitação de serviço
void ConsultarSolicitacoes(); // Exibir todas as solicitações ativas contidas no arquivo VzondaInicio.dat
void ConcluirServico();
void EncerrarExpediente();
void LimparBuffer();
void SalvarServicoNaoConcluido(TpRegMoto servico);
void CarregarServicosNaoConcluidos();
void LimparArquivo(const char *nomeArquivo);
void ObterDataAtual(char *data);
void AplicarDesconto(float valorSemDesconto, float *PrecoDesconto);
void ExibirHistoricoFinanceiro();
int PlacaExiste(const char *placa);
void VisualizarRegistrosServicos();



#endif // MOTOS_ZONDA_H