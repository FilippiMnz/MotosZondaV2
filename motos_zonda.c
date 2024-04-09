#include "motos_zonda.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Definindo variáveis globais
#define MAX_SERVICOS 100
TpRegMoto VZonda[MAX_SERVICOS];
float PrecoServicos[50] = {0};
int QuantidadeServicosConcluidos = 0;
int Quant= -1;


void ObterDataAtual(char *data) {
    time_t tempo_atual;
    struct tm *info_tempo;
    time(&tempo_atual);
    info_tempo = localtime(&tempo_atual);
    sprintf(data, "%02d/%02d/%d", info_tempo->tm_mday, info_tempo->tm_mon + 1, info_tempo->tm_year + 1900);
}


void SalvarServicoNaoConcluido(TpRegMoto servico) {
    FILE *arquivo = fopen("ServicosNaoConcluidos.dat", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo ServicosNaoConcluidos.dat.\n");
        exit(1);
    }
    fwrite(&servico, sizeof(TpRegMoto), 1, arquivo);
    fclose(arquivo);
}

// Implementação das funções
void LimparBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void SolicitaServico() {
    FILE *arquivo = fopen("VzondaInicio.dat", "ab");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return;
    }

    system("cls");
    char Sair = 'S';
    do {
        Quant = Quant + 1;
        LimparBuffer();
        printf("\n\n >>> Motos Zonda <<< \n\n");
        printf("Qual o nome do cliente? ");
        scanf(" %[^\n]s", VZonda[Quant].Nome);
        LimparBuffer();
        printf("Qual o modelo da moto? ");
        scanf(" %[^\n]s", VZonda[Quant].Modelo);
        LimparBuffer();

        // Verificar se a placa já existe no arquivo
        printf("Qual a placa da moto? ");
        fgets(VZonda[Quant].Placa, sizeof(VZonda[Quant].Placa), stdin);
        strtok(VZonda[Quant].Placa, "\n");
        while (PlacaExiste(VZonda[Quant].Placa)) {
            printf("Essa placa ja foi registrada. Por favor, insira outra placa: ");
            fgets(VZonda[Quant].Placa, sizeof(VZonda[Quant].Placa), stdin);
            strtok(VZonda[Quant].Placa, "\n");
        }
        LimparBuffer();

        printf("Qual o defeito da moto? ");
        fgets(VZonda[Quant].Defeito, sizeof(VZonda[Quant].Defeito), stdin);
        strtok(VZonda[Quant].Defeito, "\n");

        // Preencher o campo de Data
        ObterDataAtual(VZonda[Quant].Data);

        VZonda[Quant].Status = '0';
        VZonda[Quant].Preco = 0;

        // Escrever os dados no arquivo
        fwrite(&VZonda[Quant], sizeof(TpRegMoto), 1, arquivo);

        printf("\n\n Deseja inserir novo servico? S|N: ");
        scanf(" %c", &Sair);
        Sair = toupper(Sair);
        getchar();
    } while (Sair != 'N');

    // Fechar o arquivo
    fclose(arquivo);
}

int PlacaExiste(const char *placa) {
    FILE *arquivo = fopen("VzondaInicio.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return 0; // Considera que a placa não existe se não puder abrir o arquivo
    }

    TpRegMoto servico;

    // Procurar a placa no arquivo
    while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
        if (strcmp(servico.Placa, placa) == 0) {
            fclose(arquivo);
            return 1; // A placa existe
        }
    }

    fclose(arquivo);
    return 0; // A placa não existe
}
void AplicarDesconto(float valorSemDesconto, float *PrecoDesconto){ //Implementação 1 sistema de Desconto
    *PrecoDesconto = valorSemDesconto * 0.90; //Desconto de 10%
    return;
}
void IniciaServico() {
    FILE *arquivo = fopen("VzondaInicio.dat", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return;
    }

    // Variável para armazenar temporariamente os dados lidos do arquivo
    TpRegMoto servico;

    char placa[8];
    printf("Digite a placa da moto a ser iniciado o servico: ");
    scanf("%s", placa);

    // Procurar a moto pelo código (placa) no arquivo
    rewind(arquivo); // Voltar para o início do arquivo
    while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
        if (strcmp(servico.Placa, placa) == 0) {
            // Modificar o status para '1' (serviço iniciado)
            servico.Status = '1';
            // Retornar o ponteiro do arquivo para o início do registro atual
            fseek(arquivo, -sizeof(TpRegMoto), SEEK_CUR);

            // Reescrever o registro atualizado no arquivo
            fwrite(&servico, sizeof(TpRegMoto), 1, arquivo);

            printf("Servico iniciado com sucesso.\n");
            break; // Parar de procurar depois de encontrar a moto
        }
    }
    if (feof(arquivo)) {
        printf("Placa nao encontrada no arquivo VzondaInicio.dat.\n");
    }

    // Fechar o arquivo
    fclose(arquivo);
}

void RemoverSolicitacao() {
    char opcao;
    do {
        system("cls");
        char placa[8];
        printf("Placa da moto para remover solicitacao: ");
        scanf(" %[^\n]s", placa);
        LimparBuffer();

        FILE *arquivo = fopen("VzondaInicio.dat", "rb+");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
            return;
        }

        TpRegMoto servico;
        int posicao = -1;

        while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
            if (strcmp(servico.Placa, placa) == 0) {
                posicao = ftell(arquivo) / sizeof(TpRegMoto) - 1;
                break;
            }
        }

        if (posicao == -1) {
            printf("Moto nao cadastrada!\n");
        } else {
            if (servico.Status != '0') {
                printf("Nao e possivel fazer a retirada, servico ja iniciado!\n");
            } else {
                servico.Status = '2'; // Altera o status para '2' (cancelado)
                fseek(arquivo, posicao * sizeof(TpRegMoto), SEEK_SET);
                fwrite(&servico, sizeof(TpRegMoto), 1, arquivo);
                printf("Solicitacao removida com sucesso!\n");
            }
        }

        fclose(arquivo);

        printf("Deseja remover outra solicitacao? (S/N): ");
        scanf(" %c", &opcao);
    } while (toupper(opcao) == 'S');
}

void ConsultarSolicitacoes() {
    printf("\n\n >>> Motos Zonda <<< \n\n");

    // Abrir o arquivo VzondaInicio.dat para leitura
    FILE *arquivo = fopen("VzondaInicio.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return;
    }

    // Variável para armazenar temporariamente os dados lidos do arquivo
    TpRegMoto servico;

    // Lendo e exibindo cada registro do arquivo
    printf("Servicos em andamento ou aguardando atendimento:\n");
    while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
        printf("\n Cliente: %s", servico.Nome);
        printf("\n Modelo: %s", servico.Modelo);
        printf("\n Placa: %s", servico.Placa);
        printf("\n Defeito: %s", servico.Defeito);
        printf("\n Data: %s", servico.Data);
        printf("\n Status: %c", servico.Status);
        if (servico.Preco == 0) {
            printf("\n Preco: 0");
        } else {
            printf("\n Preco: %.2f", servico.Preco);
        }
        printf("\n --------------------- \n\n");
    }

    // Fechar o arquivo
    fclose(arquivo);

    system("pause");
}


void CarregarServicosNaoConcluidos() {
    FILE *arquivoEntrada = fopen("ServicosNaoConcluidos.dat", "rb");
    FILE *arquivoSaida = fopen("VzondaInicio.dat", "ab");

    if (arquivoEntrada && arquivoSaida) {
        TpRegMoto servico;
        int placaJaExistente = 0; // Flag para indicar se a placa já existe no VzondaInicio.dat

        // Verificar se o arquivo ServicosNaoConcluidos.dat está vazio
        fseek(arquivoEntrada, 0, SEEK_END);
        long tamanho = ftell(arquivoEntrada);
        if (tamanho == 0) {
            printf("O arquivo ServicosNaoConcluidos.dat esta vazio.\n");
            fclose(arquivoEntrada);
            fclose(arquivoSaida);
            return;
        }
        rewind(arquivoEntrada);

        // Copiar os serviços não concluídos para VzondaInicio.dat, excluindo serviços com placas já existentes
        while (fread(&servico, sizeof(TpRegMoto), 1, arquivoEntrada)) {
            fseek(arquivoSaida, 0, SEEK_SET); // Voltar ao início do arquivo de saída para verificar as placas existentes

            // Verificar se a placa do serviço já existe no arquivo de saída
            while (fread(&VZonda[Quant], sizeof(TpRegMoto), 1, arquivoSaida)) {
                if (strcmp(servico.Placa, VZonda[Quant].Placa) == 0) {
                    placaJaExistente = 1;
                    break;
                }
            }

            if (!placaJaExistente) {
                fwrite(&servico, sizeof(TpRegMoto), 1, arquivoSaida);
                // Adicionar o serviço ao vetor temporário VZonda
                if (++Quant >= MAX_SERVICOS) {
                    printf("Limite de servicos atingido. Alguns servicos nao foram copiados para VzondaInicio.dat.\n");
                    break;
                }
            }

            placaJaExistente = 0; // Resetar a flag para o próximo serviço
        }

        fclose(arquivoEntrada);
        fclose(arquivoSaida);

        // Limpar o arquivo ServicosNaoConcluidos.dat
        FILE *arquivoLimpo = fopen("ServicosNaoConcluidos.dat", "wb");
        fclose(arquivoLimpo);
    } else {
        printf("Erro ao abrir os arquivos.\n");
    }
}
void ConcluirServico() {
    system("cls");
    char placa[8];
    printf("Placa da moto para concluir servico: ");
    scanf(" %[^\n]s", placa);
    LimparBuffer();
    int posicao = -1;

    // Abrir o arquivo VzondaInicio.dat para leitura e escrita
    FILE *arquivo = fopen("VzondaInicio.dat", "rb+");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return;
    }

    // Variável para armazenar temporariamente os dados lidos do arquivo
    TpRegMoto servico;

    // Procurar o serviço pelo código (placa) no arquivo
    while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
        if (strcmp(servico.Placa, placa) == 0) {
            posicao = ftell(arquivo) / sizeof(TpRegMoto) - 1; // Calcula a posição do registro no arquivo
            break;
        }
    }

    if (posicao == -1) {
        printf("Moto nao cadastrada!\n");
    } else {
        if (servico.Status == '1') {
            printf("Informe o valor do servico: ");
            scanf("%f", &servico.Preco);
            printf("Deseja Aplicar Desconto? S or N: ");
            char opc;
            scanf(" %c", &opc);
            char opcaoDesc = toupper(opc);
            float valorDesconto;
            if (opcaoDesc == 'S') {
                AplicarDesconto(servico.Preco, &valorDesconto);
                printf("Servico concluido. Preco final com desconto: R$%.2f\n", valorDesconto);
                servico.Preco = valorDesconto;
            } else if (opcaoDesc == 'N') {
                printf("Servico concluido. Preco final: R$ %.2f\n", servico.Preco);
            } else {
                printf("Opcao invalida!\n");
                fclose(arquivo); // Fechar o arquivo antes de retornar
                return;
            }

            // Atualizar o status do serviço para '3' (concluído)
            servico.Status = '3';

            // Retornar o ponteiro do arquivo para o início do registro atual
            fseek(arquivo, posicao * sizeof(TpRegMoto), SEEK_SET);

            // Reescrever o registro atualizado no arquivo
            fwrite(&servico, sizeof(TpRegMoto), 1, arquivo);

            printf("Servico concluido com sucesso.\n");
        } else {
            printf("Nao e possivel concluir o servico.\n");
        }
    }

    // Fechar o arquivo
    fclose(arquivo);

    system("pause");
}

void EncerrarExpediente() {
    // Abrir o arquivo VzondaInicio.dat para leitura
    FILE *arquivo = fopen("VzondaInicio.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat.\n");
        return;
    }

    // Abrir o arquivo HistoricoFinanceiroZonda.dat para leitura e escrita
    FILE *historicoFile = fopen("HistoricoFinanceiroZonda.dat", "rb+");
    if (historicoFile == NULL) {
        printf("Erro ao abrir o arquivo HistoricoFinanceiroZonda.dat.\n");
        fclose(historicoFile); // Fechar o arquivo antes de retornar
        return;
    }

    // Variáveis para armazenar o valor total do dia e a data do dia
    float valorTotalDia = 0;
    char dataDia[11]; // DD/MM/AAAA + \0

    // Lendo e exibindo cada registro do arquivo
    printf("\n\n >>> Motos Zonda <<< \n\n");
    printf("Servicos efetuados:\n");
    TpRegMoto servico;
    while (fread(&servico, sizeof(TpRegMoto), 1, arquivo) == 1) {
        if (servico.Status == '3' || servico.Status == '2') {
            printf("\n Cliente: %s", servico.Nome);
            printf("\n Modelo: %s", servico.Modelo);
            printf("\n Placa: %s", servico.Placa);
            printf("\n Defeito: %s", servico.Defeito);
            printf("\n Data: %s", servico.Data);
            printf("\n Status: %c", servico.Status);
            printf("\n Preco: %.2f", servico.Preco);
            valorTotalDia += servico.Preco;
            printf("\n --------------------- \n\n");
        } else if (servico.Status == '0' || servico.Status == '1') {
            // Salvar apenas os serviços com status = 0 ou 1
            SalvarServicoNaoConcluido(servico);
        }
    }

    // Opção manual para inserção da data
    printf("Deseja inserir manualmente a data do dia? (S/N): ");
    char opcao;
    scanf(" %c", &opcao);
    if (toupper(opcao) == 'S') {
        printf("Qual a data? (DD/MM/AAAA): ");
        scanf(" %[^\n]s", dataDia);
        LimparBuffer();
    } else {
        // Obter a data do dia atual e armazenar em dataDia
        ObterDataAtual(dataDia);
    }

    // Verificar se a data já existe no arquivo HistoricoFinanceiroZonda.dat
    HistoricoFinanceiro historico;
    int dataExistente = 0;
    while (fread(&historico, sizeof(HistoricoFinanceiro), 1, historicoFile) == 1) {
        if (strcmp(historico.Data, dataDia) == 0) {
            // A data já existe, então editar o registro existente
            fseek(historicoFile, -sizeof(HistoricoFinanceiro), SEEK_CUR);
            historico.ValorTotal += valorTotalDia;
            fwrite(&historico, sizeof(HistoricoFinanceiro), 1, historicoFile);
            dataExistente = 1;
            break;
        }
    }

    // Se a data não existir, escrever um novo registro
    if (!dataExistente) {
        strcpy(historico.Data, dataDia);
        historico.ValorTotal = valorTotalDia;
        fwrite(&historico, sizeof(HistoricoFinanceiro), 1, historicoFile);
    }

    // Fechar os arquivos
    fclose(arquivo);
    fclose(historicoFile);

    printf("\nValor total obtido: %.2f\n", valorTotalDia);
    system("pause");
    LimparVzondaInicio();
}
//Fazer a limpeza do VzondaInicio
void LimparVzondaInicio() {
    // Abrir o arquivo VzondaInicio.dat para escrita, o que limpará seu conteúdo
    FILE *arquivo = fopen("VzondaInicio.dat", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo VzondaInicio.dat para limpeza.\n");
        return;
    }
    fclose(arquivo);
}

void ExibirHistoricoFinanceiro() {
    system("cls");
    // Abrir o arquivo HistoricoFinanceiroZonda.dat para leitura
    FILE *historicoFile = fopen("HistoricoFinanceiroZonda.dat", "rb");
    if (historicoFile == NULL) {
        printf("Erro ao abrir o arquivo HistoricoFinanceiroZonda.dat para leitura.\n");
        return;
    }

    // Variáveis para armazenar temporariamente os dados lidos do arquivo
    HistoricoFinanceiro registro;
    float totalVendas = 0;
    char dataAtual[11] = ""; // Variável para armazenar a data atual
    float valorTotalMesmoDia = 0; // Variável para armazenar o valor total de vendas para o mesmo dia
    char diaMaisVendas[11] = ""; // Variável para armazenar o dia com mais vendas
    float maiorValorVendas = 0; // Variável para armazenar o maior valor total de vendas

    // Exibir o cabeçalho
    printf("\nHistorico Financeiro:\n");
    printf("%12s %15s\n", "Data", "Valor Total");

    // Lendo e exibindo cada registro do arquivo
    while (fread(&registro, sizeof(HistoricoFinanceiro), 1, historicoFile) == 1) {
        // Se a data atual for diferente da data do registro atual
        if (strcmp(registro.Data, dataAtual) != 0) {
            // Se não for o primeiro registro, exibir o valor total para o dia anterior
            if (totalVendas > 0) {
                printf("(%s): %.2f\n", dataAtual, valorTotalMesmoDia);
                // Verificar se o valor total de vendas desse dia é maior que o maior valor de vendas atual
                if (valorTotalMesmoDia > maiorValorVendas) {
                    // Atualizar o dia com mais vendas e o valor total de vendas mais alto
                    strcpy(diaMaisVendas, dataAtual);
                    maiorValorVendas = valorTotalMesmoDia;
                }
            }
            // Atualizar a data atual e reiniciar a contagem do valor total para o mesmo dia
            strcpy(dataAtual, registro.Data);
            valorTotalMesmoDia = registro.ValorTotal;
        } else {
            // Se for o mesmo dia, somar o valor total para o mesmo dia
            valorTotalMesmoDia += registro.ValorTotal;
        }

        // Somar o valor total das vendas
        totalVendas += registro.ValorTotal;
    }

    // Exibir o valor total para o último dia, se houver mais de um registro para ele
    if (totalVendas > 0) {
        printf("(%s): %.2f\n", dataAtual, valorTotalMesmoDia);
        // Verificar se o valor total de vendas desse dia é maior que o maior valor de vendas atual
        if (valorTotalMesmoDia > maiorValorVendas) {
            // Atualizar o dia com mais vendas e o valor total de vendas mais alto
            strcpy(diaMaisVendas, dataAtual);
            maiorValorVendas = valorTotalMesmoDia;
        }
    }

    // Exibir a quantidade total de vendas de todos os dias somados
    printf("Quantidade total de vendas: %.2f\n", totalVendas);

    // Exibir o dia com mais vendas
    printf("Dia com mais vendas: %s\n", diaMaisVendas);
    system("pause");

    // Fechar o arquivo
    fclose(historicoFile);
}

/*
  //Opção manual
       printf("Qual a data? (DD/MM/AAAA): ");
       scanf(" %[^\n]s", VZonda[Quant].Data);
       LimparBuffer();
    *
    * */