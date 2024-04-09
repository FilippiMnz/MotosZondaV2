#ifndef MOTOS_ZONDA_H
#define MOTOS_ZONDA_H
#define MAX_SERVICOS 100

// Incluindo bibliotecas necessárias
#include <stdio.h>

typedef struct {
    char Data[11];  // DD/MM/AAAA + \0
    float ValorTotal;
} HistoricoFinanceiro;

// Definindo a estrutura de dados para os registros das motos
struct RegMoto {
    char Nome[100];
    char Modelo[50];
    char Placa[10];
    char Defeito[200];
    char Data[11]; // DD/MM/AAAA + \0
    char Status;
    float Preco;
};
typedef struct RegMoto TpRegMoto;

// Declaração das funções
void SolicitaServico();
void IniciaServico();
void RemoverSolicitacao();
void ConsultarSolicitacoes();
void ConcluirServico();
void EncerrarExpediente();
void LimparBuffer();
void SalvarServicoNaoConcluido();
void CarregarServicosNaoConcluidos();
void LimparVzondaInicio();
void ObterDataAtual(char *data);
void AplicarDesconto(float valorSemDesconto, float *PrecoDesconto);
void ExibirHistoricoFinanceiro();
int PlacaExiste(const char *placa);



#endif // MOTOS_ZONDA_H