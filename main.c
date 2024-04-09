#include <stdio.h>
#include "motos_zonda.h"
int main() {
    int Opcao;
    CarregarServicosNaoConcluidos();
    do {
        printf("\n\n >>> Motos Zonda <<< \n\n");
        printf("1 - Solicitar Servico\n");
        printf("2 - Iniciar Servico\n");
        printf("3 - Remover Solicitacao\n");
        printf("4 - Consultar Solicitacoes\n");
        printf("5 - Concluir Servico\n");
        printf("6 - Encerrar Expediente\n");
        printf("7 - Exibir Historico Financeiro\n");
        printf("8 - Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &Opcao);

        switch (Opcao) {
            case 1: SolicitaServico(); break;
            case 2: IniciaServico(); break;
            case 3: RemoverSolicitacao(); break;
            case 4: ConsultarSolicitacoes(); break;
            case 5: ConcluirServico(); break;
            case 6: EncerrarExpediente(); break;
            case 7: ExibirHistoricoFinanceiro(); break;
            case 8: break;
            case 9: VisualizarRegistrosServicos();break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (Opcao != 8);


    return 0;
}
