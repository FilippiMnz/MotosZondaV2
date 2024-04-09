#include <stdio.h>
#include "motos_zonda.h"
#include <time.h>

int main() {
    int Opcao;
    do {
        printf("\n\n >>> Motos Zonda <<< \n\n");
        printf("1 - Solicitar Servico\n");
        printf("2 - Carregar Servicos Nao Concluidos\n");
        printf("3 - Iniciar Servico\n");
        printf("4 - Remover Solicitacao\n");
        printf("5 - Consultar Solicitacoes\n");
        printf("6 - Concluir Servico\n");
        printf("7 - Encerrar Expediente\n");
        printf("8 - Exibir Historico Financeiro\n");
        printf("9 - Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &Opcao);

        switch (Opcao) {
            case 1: SolicitaServico(); break;
            case 2: CarregarServicosNaoConcluidos(); break;
            case 3: IniciaServico(); break;
            case 4: RemoverSolicitacao(); break;
            case 5: ConsultarSolicitacoes(); break;
            case 6: ConcluirServico(); break;
            case 7: EncerrarExpediente(); break;
            case 8: ExibirHistoricoFinanceiro(); break;
            case 9: LimparVzondaInicio(); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (Opcao != 9);


    return 0;
}
