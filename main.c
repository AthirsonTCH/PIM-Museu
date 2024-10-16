#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

#define CLEAN_BUFF do{ int c; while((c = getchar()) != '\n' && c != EOF);}while(0)//usado para limpar o buff e evitar bugs

#define NUM_MAX_CAD 50
#define COMP_MAX_SENHA 50
#define COMP_MAX_USUARIO 10
#define NUM_MAX_COMPRA 50


//FUNÇÃO PARA EXIBIR A TELA DE MENU
void telamenu(){
printf(" -------------------- ESCOLHA A OPÇÃO DESEJADA ----------------------\n");
printf(" |                                                                  |\n");
printf(" | 1-100 anos semana da arte moderna                                |\n");
printf(" |                                                                  |\n");
printf(" | 2-158-anos santos dumont                                         |\n");
printf(" |                                                                  |\n");
printf(" | 3-Vender ingresso                                                |\n");
printf(" |                                                                  |\n");
printf(" | 4-Visualizar registro de vendas                                  |\n");
printf(" |                                                                  |\n");
printf(" | 5-Listar cadastros                                               |\n");
printf(" |                                                                  |\n");
printf(" | 6-Pesquisar por cadastros                                        |\n");
printf(" |                                                                  |\n");
printf(" | 7-Validar ingressos                                              |\n");
printf(" |                                                                  |\n");
printf(" | 0-Sair da conta                                                  |\n");
printf(" |                                                                  |\n");
printf(" --------------------------------------------------------------------\n");
}

// Função para remover espaços à esquerda de uma string
void ltrim(char *str)
{
    int i = 0;
    while (isspace(str[i]))
    {
        i++;
    }
    memmove(str, str + i, strlen(str) - i + 1);
}

// Função para remover espaços à direita de uma string
void rtrim(char *str)
{
    int i = strlen(str) - 1;
    while (i >= 0 && isspace(str[i]))
    {
        i--;
    }
    str[i + 1] = '\0';
}

// Função para remover espaços tanto à esquerda quanto à direita de uma string
void trim(char *str)
{
    ltrim(str);
    rtrim(str);
}

//LIMPA A STRING, USADO PARA EXCLUIR CADASTRO
void limparString(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        str[i] = '\0';
        i++;
    }
}

//ESTRUTURA PARA O CASASTRO
struct cadastro{
    char email[50];
    char cel[50];
    char pais[50];
    char cidade[50];
    char nome[COMP_MAX_USUARIO];
    char senha[COMP_MAX_SENHA];
};

//ESTRUTURA PARA OS DADOS DE COMPRA
struct dados_compra{
    char nomeCompleto[50];
    char cpf[50];
    char celCliente[50];
    char emailCliente[50];
    int numIngressosComprados;
    int ingressosComprados[50];
};

int main(){

//CRIANO UM TYPEDEFF PARA ESTRUTURA CADASTRO E CRIANDO UM VETOR COM ESSE TYPEDEF PARA TER VARIAS ESTRUTURAS DO TIPO CADASTRO
struct cadastro cad[NUM_MAX_CAD];

//CRIANO UM TYPEDEFF PARA ESTRUTURA DOS DADOS DA COMPRA E CRIANDO UM VETOR COM ESSE TYPEDEF PARA TER VARIAS ESTRUTURAS DO TIPO DE DADOS COMPRA
struct dados_compra dc[1000];

int numvendas=0;//VARIAVEL PARA CONTROLAR O NUMERO TOTAL DE INGRESSOS VENDIDOS
int numDaVenda=0;//VARIAVEL PARA SABER QUAL É O NUMERO DA ULTIMA VENDA FEITA

char opcao;//VARIAVEL USADA EM TODAS AS DECISOS ENVOLVENDO SWITCH CASE
int numcad=0;//VARIAVEL PARA CONTAR O NUMERO DE CADASTRO
char senhaC[COMP_MAX_SENHA];//VARIAVEL USADA NA CONFIRMAÇÃO DE SENHA, NA HORA DA CRIANÇÃ DE UM CADASTRO OU QUANDO O USUARIO VAI ALTERAR A SENHA

//variaveis para verificação de login
int loginSucesso=0;//VARIAVEL USADA PARA SABER SE O LOGIN FOI FEITO, ENQUANTO FOR 0 O LOGIN NÃO FOI FEITO, SE MUDAR PARA 1 O LOGIN FOI FEITO,
char senhalogin[COMP_MAX_SENHA];//VARIAVEL USADA NO LOGIN PARA VERIFICAR SE A SENHA DIGITADA CORRESPONDE A SENHA DAQUELE CADASTRO
char nomelogin[COMP_MAX_USUARIO];//VARIAVEL USADA NO LOGIN PARA VERIFICAR SE O NOME DIGITADO CORRESPONDE COM ALGUM NOME DE ALGUM CADASTRO EXISTENTE

int numIngressoValidar;//VETOR USADDO NA VALIDAÇÃO DE INGRESSOS

//VETORES PARA FACILITAR A TESTAGEM DO CODIGO, ELES TORNAM POSSIVEL FAZER LOGIN SEM TER CADASTRO
char adm[] = "adm";
char senha_adm[] = "123";

FILE *filenumeCad;//DECLARAÇÃO DO ARQUIVO USADO PARA GRAVAR O NUMERO DE CADASTRO
FILE *dadosCadastroLeitura;//DECLARAÇÃO DO ARQUIVO USADO PARA GRAVAR OS DADOS DO CADASTRO SO QUE ESSE ARQUIVO É USADO PARA COLOCAR AS INFORMAÇOES DE NOVO NO CODIGO EM CADAS STRUCT E VETOR DE STRING CORRESPONDENTE DEPOIS QUE O PROGRAMA É FECHADO E ABERTO NOVAMENTE
FILE *filedadosCompra;//DECLARAÇÃO DO ARQUIVO USADO PARA GRAVAR OS DADOS A COMPRA

FILE *filetemporarioLeitura;//ARQUIVO TEMPORARIO USADO PARA ATUALIZAR OS DADOS DO CADASTRO NO ARQUIVO DE LEITURA

//LENDO O ARQUIVO USADO PARA GUARDA O NUMERO DE CADASTRO E RECOLOCA ESSE NUMERO DE NOVO NA VARIAVEL NUMCAD QUANDO O PROGRAMA É ABERTO
filenumeCad = fopen("numecad.csv", "r");//ABRE O ARQUIVO PARA LEITURA
    if (filenumeCad == NULL)//VERIFICA SE O ARQUIVO FOI ABERTO CORRETAMENTE
    {
        printf("Erro ao ler o arquivo do numecad");
        system("pause");
    }
fscanf(filenumeCad, "%d", &numcad);
fclose(filenumeCad);//FECHANDO O ARQUIVO

//LENDO O ARQUIVO DE LEITURA DOS DADOS DO CADASTRO E COLOCANDO CADA DADO EM SUA STRUCT E VETOR DE STRING CORRESPONDENTE
dadosCadastroLeitura = fopen("dados_cadastro_leitura.csv", "r");
    if (dadosCadastroLeitura == NULL)
    {
        printf("Erro ao abrir o arquivo");
        system("pause");
    }
char linha[NUM_MAX_CAD*50];//VETOR USAO PARA LER OS DADOS DO ARQUIVO
int i=0;
while(fgets(linha, sizeof(linha), dadosCadastroLeitura)!=NULL && i<numcad){
    sscanf(linha, "%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49s",
        cad[i].email, cad[i].cel, cad[i].pais, cad[i].cidade, cad[i].nome, cad[i].senha);
        i++;
}
fclose(dadosCadastroLeitura);
//FIM DO LENDO O ARQUIVO DE LEITURA DOS DADOS DO CADASTRO E COLOCANDO CADA DADO EM SUA STRUCT E VETOR DE STRING CORRESPONDENTE


//LENDO O ARQUIVO DOS DADOS DA COMPRA
filedadosCompra = fopen("dados_compra.csv", "r");
    if (filedadosCompra == NULL)
    {
        printf("Arquivo dados compra não pode ser aberto\n");
        system("pause");
    }
char linha2[NUM_MAX_COMPRA*50*100];

i=0;
while(fgets(linha2, sizeof(linha2), filedadosCompra)!=NULL && i<NUM_MAX_COMPRA){
    //printf("\nlinha lida:%s\n", linha2);ESSE TRECHO TEM O PROPOSITO APENAS PARA TESTAR O CODIGO, POR ISSO ESTA COMENTADO

    sscanf(linha2, "%49[^,],%49[^,],%49[^,],%49[^,],%d,%d,%d,%d",
           dc[i].nomeCompleto, dc[i].cpf, dc[i].celCliente, dc[i].emailCliente, &numDaVenda, &numvendas, &dc[i].numIngressosComprados, &dc[i].ingressosComprados[0]);

    //atribui o resto dos valores dos ingresos com base no primeiro
    int numeroIngressosComprados = dc[i].numIngressosComprados;
    for (int j=0;j<=numeroIngressosComprados-1;j++)
    {
        dc[i].ingressosComprados[j]= dc[i].ingressosComprados[0]+j;
    }
    //final

    //ESSE TRECHO TEM O PROPOSITO APENAS PARA TESTAR O CODIGO, POR ISSO ESTA COMENTADO
    /*printa o numero de cada ingresso para verifivcar se foram colocados corretamente no vetor de cada struct
    for (int j=0;j<numeroIngressosComprados;j++)
    {
        printf("ingresso da estrutura %d e posição %d é:%d\n", i,j, dc[i].ingressosComprados[j]);
    }*/

    //final

    i++;
}
fclose(filedadosCompra);

//ESSE TRECHO TEM O PROPOSITO APENAS PARA TESTAR O CODIGO, POR ISSO ESTA COMENTADO
/*printf("\n [%s] [%s] [%s] [%s] [%d] [%d] [%d] [%d]\n",
       dc[5].nomeCompleto, dc[5].cpf, dc[5].celCliente, dc[5].emailCliente,
       numDaVenda, numvendas, dc[6].numIngressosComprados, dc[6].ingressosComprados[4]);
system("pause");*/

//FIM DO LENDO O ARQUIVO DE LEITURA DOS DADOS DO DADOS DA COMPRA

inicio:

system("cls");

setlocale(LC_ALL, "portuguese");

    //SESSÃO POS LOGIN
    if (loginSucesso==1)//CONDIÇÃO PARA SABER SE O LOGIN ACONTECEU OU NÃO
    {
        system("cls");
        telamenu();
        opcao=getch();

        //VARIAVEIS QUE SERAO USADAS NA VENDA DE INGRESSOS
        int total;
        int numIngressos;

        switch(opcao)
        {
            case '1':

                system("cls");
                printf("-------------------------------------------------------------------------------------------------------------");
                printf("\nExemplo:\nArtistas reuniram-se em fevereiro de 1922 e um evento que gerou uma renovação intelectual\ne artística no Brasil. Este mês a Semana de Arte Moderna, que ocorreu entre\n13 e 18 de fevereiro de 1922, completa 100 anos.");
                printf("\n-------------------------------------------------------------------------------------------------------------\n");
                CLEAN_BUFF;
                goto inicio;
                break;

            case '2':

                system("cls");
                printf("-------------------------------------------------------------------------------------------------------------");
                printf("\nExemplo:\nO “14 BIS” tem capacidade para um tripulante. Foi o primeiro avião mais\npesado que o ar a conseguir decolar por seus próprios meios.\nEsse fato histórico teve lugar em Bagatelle(centro de Paris), no\ndia 23 de outubro de 1906.");
                printf("\n-------------------------------------------------------------------------------------------------------------\n");
                CLEAN_BUFF;
                goto inicio;
                break;

            case '3':

                system("cls");
                printf(" --------------------------COMPRAR INGRESSO-------------------------\n");
                printf(" 1-Meia entrada = 5 reais                                           \n");
                printf(" 2-Inteira = 10 reais                                               \n");
                printf(" -------------------------------------------------------------------\n");
                printf(" Digite sua opção:");
                opcao=getch();


                switch(opcao)
                {
                    case '1':
                        printf(" \n--------------------------------------------------------------------\n");
                        printf(" Digite a quantidade:");

                       if (scanf("%d", &numIngressos) != 1 || getchar() != '\n') {
                            printf(" Digite um número inteiro válido.\n");
                            while (getchar() != '\n');
                            CLEAN_BUFF;
                            goto inicio;
                        }


                        total = numIngressos*5;

                        printf(" -------------------------------------------------------------------\n");
                        printf(" Total a pagar:%d\n", total);
                        break;

                    case '2':
                        printf(" \n-------------------------------------------------------------------\n");
                        printf(" Digite a quantidade:");

                        if (scanf("%d", &numIngressos) != 1 || getchar() != '\n') {
                            printf(" Digite um número inteiro válido.\n");
                            while (getchar() != '\n');
                            CLEAN_BUFF;
                            goto inicio;
                        }

                        total = numIngressos*10;

                        printf(" -------------------------------------------------------------------\n");
                        printf(" Total a pagar:%d\n", total);
                        break;

                    default:
                        printf(" \n-------------------------------------------------------------------\n");
                        printf(" Digite uma opção valida\n\n");
                        CLEAN_BUFF;
                        goto inicio;
                }

            //FORMA DE PAGAMENTO
            printf(" ------------------------FORMA DE PAGAMENTO-------------------------\n");
            printf(" 1-Cartão de crédito\n 2-cartão de débito\n 3-Dinheiro\n 0-Cancelar compra\n");
            printf(" -------------------------------------------------------------------\n");
            opcao=getch();

                switch(opcao)
                {
                    case '1':
                        system("cls");
                        printf(" -------------------------CARTÃO DE CRÉDITO-------------------------\n");

                        printf(" Nome completo do comprador:");
                        gets(dc[numDaVenda].nomeCompleto);

                        printf(" cpf:");
                        gets(dc[numDaVenda].cpf);

                        printf(" cel:");
                        gets(dc[numDaVenda].celCliente);

                        printf(" email:");
                        gets(dc[numDaVenda].emailCliente);

                        printf(" -------------------------------------------------------------------\n");
                        printf(" Compra concluida\n");
                        printf(" -------------------------------------------------------------------\n");

                        //ATRIBUI UM NUMERO AO INGRESSOS COMPRADOS
                        printf(" Numero dos seus ingressos:\n");
                        for(int i=0;i<numIngressos;i++)
                        {
                            dc[numDaVenda].ingressosComprados[i] = numvendas+(i+1);
                            printf(" ingresso:%04d\n", dc[numDaVenda].ingressosComprados[i]);
                        }
                        dc[numDaVenda].numIngressosComprados = numIngressos;

                        printf(" -------------------------------------------------------------------\n");
                        printf(" total pago:%d\n", total);
                        printf(" -------------------------------------------------------------------\n");
                        numvendas=numvendas+numIngressos;

                        printf(" numero total de ingressos vendidos:%d\n numero dessa venda especifica:%d\n", numvendas, numDaVenda+1);
                        printf("\n\n");

                            //SALVAR REGISTRO DE VENDAS EM ARQUIVO
                            filedadosCompra = fopen("dados_compra.csv", "a");
                            if (filedadosCompra == NULL)
                            {
                                printf("Erro ao gravar dados");
                            }

                            fprintf(filedadosCompra, "%s,%s,%s,%s,", dc[numDaVenda].nomeCompleto, dc[numDaVenda].cpf, dc[numDaVenda].celCliente, dc[numDaVenda].emailCliente);

                            numDaVenda++;
                            fprintf(filedadosCompra, "%d,%d,%d,", numDaVenda, numvendas, numIngressos);
                            numDaVenda--;

                            for(int i=0;i<numIngressos;i++)
                            {
                                fprintf(filedadosCompra, "%d,", dc[numDaVenda].ingressosComprados[i]);
                            }
                            fprintf(filedadosCompra, "\n");
                            fclose(filedadosCompra);

                        numDaVenda++;
                        system(" pause");
                        goto inicio;
                        break;

                    case '2':
                        system("cls");
                        printf(" ------------------------CARTÃO DE DÉBITO------------------------\n");

                        printf(" Nome completo do comprador:");
                        gets(dc[numDaVenda].nomeCompleto);

                        printf(" cpf:");
                        gets(dc[numDaVenda].cpf);

                        printf(" cel:");
                        gets(dc[numDaVenda].celCliente);

                        printf(" email:");
                        gets(dc[numDaVenda].emailCliente);

                        printf(" -------------------------------------------------------------------\n");
                        printf(" Compra concluida\n");
                        printf(" -------------------------------------------------------------------\n");

                        //atribuir um numero aos ingressos comprados
                        printf(" Numero dos seus ingressos:\n");
                        for(int i=0;i<numIngressos;i++)
                        {
                            dc[numDaVenda].ingressosComprados[i] = numvendas+(i+1);
                            printf(" ingresso:%04d\n", dc[numDaVenda].ingressosComprados[i]);
                        }
                        dc[numDaVenda].numIngressosComprados = numIngressos;

                        //mostra o total pago, o numero total de vendas que foi feito pelo codigo ate o momento e o numero da venda atual
                        printf("\n total pago:%d\n\n", total);
                        numvendas=numvendas+numIngressos;
                        printf(" numero total de ingressos vendidos:%d\n numero dessa venda especifica:%d\n", numvendas, numDaVenda+1);
                        printf("\n\n");

                            //SALVAR REGISTRO DE VENDAS EM ARQUIVO
                            filedadosCompra = fopen("dados_compra.csv", "a");
                            if (filedadosCompra == NULL)
                            {
                                printf("Erro ao gravar dados");
                            }

                            //numvendas é o numero total de ingressos vendidos
                            fprintf(filedadosCompra, "%s,%s,%s,%s,", dc[numDaVenda].nomeCompleto, dc[numDaVenda].cpf, dc[numDaVenda].celCliente, dc[numDaVenda].emailCliente);

                            numDaVenda++;
                            fprintf(filedadosCompra, "%d,%d,%d,", numDaVenda, numvendas, numIngressos);
                            numDaVenda--;

                            for(int i=0;i<numIngressos;i++)
                            {
                                fprintf(filedadosCompra, "%d,", dc[numDaVenda].ingressosComprados[i]);
                            }
                            fprintf(filedadosCompra, "\n");
                            fclose(filedadosCompra);

                        numDaVenda++;
                        system(" pause");
                        goto inicio;
                        break;

                    case '3':
                        system("cls");
                        printf(" -------------------------------DINHEIRO-------------------------------\n");

                        printf(" Nome completo do comprador:");
                        gets(dc[numDaVenda].nomeCompleto);

                        printf(" cpf:");
                        gets(dc[numDaVenda].cpf);

                        printf(" cel:");
                        gets(dc[numDaVenda].celCliente);

                        printf(" email:");
                        gets(dc[numDaVenda].emailCliente);

                        printf(" --------------------------------------------------------------------\n");
                        printf(" Total a ser pago:%d\n", total);
                        printf(" --------------------------CALCULO DE TROCO--------------------------\n");

                        float pagamentocliente;
                        printf("\n Insira o pagamento do cliente:");
                        if (scanf("%f", &pagamentocliente) != 1)
                        {
                            printf("você deve digitar um número");
                            system("pause");
                            CLEAN_BUFF;
                            goto inicio;
                        }

                        if(pagamentocliente<total)
                        {
                            printf(" \n Quantia insuficiente, venda cancelada\n\n");
                            system("pause");
                            CLEAN_BUFF;
                            goto inicio;
                        }
                        printf(" \n Troco do cliente:%.2f", pagamentocliente-total);
                        printf(" \n\n Compra concluida\n");
                        printf(" -------------------------------------------------------------------\n");

                        //atribuir um numero aos ingressos comprados
                        printf(" Numero dos seus ingressos:\n");
                        for(int i=0;i<numIngressos;i++)
                        {
                            dc[numDaVenda].ingressosComprados[i] = numvendas+(i+1);
                            printf(" ingresso:%04d\n", dc[numDaVenda].ingressosComprados[i]);
                        }
                        dc[numDaVenda].numIngressosComprados = numIngressos;

                        //mostra o total pago, o numero total de vendas que foi feito pelo codigo ate o momento e o numero da venda atual
                        printf(" \n total pago:%d\n\n", total);
                        numvendas=numvendas+numIngressos;
                        printf(" numero total de ingressos vendidos:%d\n numero dessa venda especifica:%d\n", numvendas, numDaVenda+1);
                        printf("\n\n");

                            //SALVAR REGISTRO DE VENDAS EM ARQUIVO
                            filedadosCompra = fopen("dados_compra.csv", "a");
                            if (filedadosCompra == NULL)
                            {
                                printf("Erro ao gravar dados");
                            }

                           //numvendas é o numero total de ingressos vendidos
                            fprintf(filedadosCompra, "%s,%s,%s,%s,", dc[numDaVenda].nomeCompleto, dc[numDaVenda].cpf, dc[numDaVenda].celCliente, dc[numDaVenda].emailCliente);

                            numDaVenda++;
                            fprintf(filedadosCompra, "%d,%d,%d,", numDaVenda, numvendas, numIngressos);
                            numDaVenda--;

                            for(int i=0;i<numIngressos;i++)
                            {
                                fprintf(filedadosCompra, "%d,", dc[numDaVenda].ingressosComprados[i]);
                            }
                            fprintf(filedadosCompra, "\n");
                            fclose(filedadosCompra);

                        numDaVenda++;
                        system(" pause");
                        CLEAN_BUFF;
                        goto inicio;
                        break;

                    case '0':
                        goto inicio;
                        break;

                    default:
                        printf(" Digite uma opção valida\n\n");
                        system(" pause");
                        goto inicio;
                }
                break;

            case '4':
                //LISTA TODAS AS COMPRAS FEITAS
                system("cls");
                printf(" -------------------------------------------------------------------\n");
                for (int i=0;i<numDaVenda;i++)
                {
                    printf(" Dados do comprador\n");
                    printf(" Nome:%s\n", dc[i].nomeCompleto);
                    printf(" cpf:%s\n", dc[i].cpf);
                    printf(" cel:%s\n", dc[i].celCliente);
                    printf(" email:%s\n", dc[i].emailCliente);
                    printf(" numero de ingressos comprados:%d\n\n", dc[i].numIngressosComprados);

                    printf(" ingressos Comprados:\n");
                    for(int j=0; j<dc[i].numIngressosComprados ; j++)
                    {
                        printf(" ingresso:%04d\n", dc[i].ingressosComprados[j]);
                    }
                printf(" \n-------------------------------------------------------------------\n");
                }
                system("pause");
                goto inicio;
                break;

            case '5':
                //LISTA CADASTRO FEITOS
                system("cls");
                printf(" --------------------------CADASTRO FEITOS--------------------------\n");
                for (int i=0;i<numcad;i++)
                {
                    printf(" Dados do cadastro\n\n");
                    printf(" email:%s\n", cad[i].email);
                    printf(" cel:%s\n", cad[i].cel);
                    printf(" pais:%s\n", cad[i].pais);
                    printf(" cidade:%s\n", cad[i].cidade);
                    printf(" nome de usuário:%s\n\n", cad[i].nome);
                    printf(" -------------------------------------------------------------------\n");
                }

                getch();
                goto inicio;
                break;

            case '6':
                //PESQUISA CADASTRO PARA EDIÇÃO
                system("cls");

                char nomepesquisa[COMP_MAX_USUARIO];

                printf(" --------------------------PESQUISA DE REGISTRO--------------------------\n");
                printf(" digite o nome do registro que você quer pesquisar:");
                gets(nomepesquisa);

                trim(nomepesquisa);
                if(strlen(nomepesquisa) == 0)
                {
                    printf(" Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }

                for(int i=0;i<numcad;i++)
                {
                    if(strcmp(nomepesquisa, cad[i].nome)==0)
                    {
                        printf("\n Registro encontradro\n\n");
                        printf(" nome:%s\n email:%s\n cel:%s\n nacionalidade:%s\n cidade:%s\n\n", cad[i].nome, cad[i].email, cad[i].cel, cad[i].pais, cad[i].cidade);

                        printf(" -------------------------------------------------------------------\n");
                        printf(" O que você deseja fazer\n\n");
                        printf(" 1-editar cadastro, 2-Excluir cadastro, 0-voltar\n\n");
                        opcao=getch();

                        switch(opcao){
                            case '1':
                                printf(" para editar esse registro você deve inserir a senha atual desse registro\n");
                                printf(" senha:");
                                gets(senhalogin);

                                if (strcmp(senhalogin,cad[i].senha)==0){
                                    printf(" -------------------------------------------------------------------\n");
                                    printf(" qual dado você pretende alterar, digite 0 para cancelar a alteração\n\n");
                                    printf(" 1-nome:%s\n 2-email:%s\n 3-cel:%s\n 4-nacionalidade:%s\n 5-cidade:%s\n 6-senha:%s\n\n", cad[i].nome, cad[i].email, cad[i].cel, cad[i].pais, cad[i].cidade, cad[i].senha);
                                    opcao=getch();

                                    char novonome[COMP_MAX_USUARIO];
                                    char novoemail[50];
                                    char novocel[50];
                                    char novopais[50];
                                    char novacidade[50];
                                    char novasenha[COMP_MAX_SENHA];

                                    switch(opcao){
                                        case '1':
                                            printf(" novo nome:");
                                            gets(novonome);
                                            trim(novonome);
                                            if(strlen(novonome) == 0){
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].nome, novonome);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL){
                                                printf(" gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '2':
                                            printf(" novo email:");
                                            gets(novoemail);
                                            trim(novoemail);
                                            if(strlen(novoemail) == 0){
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].email, novoemail);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL){
                                                printf(" gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '3':
                                            printf(" novo cel:");
                                            gets(novocel);
                                            trim(novocel);
                                            if(strlen(novocel) == 0){
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].cel, novocel);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL){
                                                printf("gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '4':
                                            printf(" nova nacionalidade:");
                                            gets(novopais);
                                            trim(novopais);
                                            if(strlen(novopais) == 0){
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].pais, novopais);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL){
                                                printf("gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '5':
                                            printf(" nova cidade:");
                                            gets(novacidade);
                                            trim(novacidade);
                                            if(strlen(novacidade) == 0)
                                            {
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].cidade, novacidade);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL){
                                                printf("gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '6':
                                            printf(" nova senha:");
                                            gets(novasenha);
                                            trim(novasenha);
                                            if(strlen(novasenha) == 0)
                                            {
                                                printf("Você deve digitar algo\n\n");
                                                system("pause");
                                                goto inicio;
                                            }

                                            printf(" confirme a nova senha:");
                                            gets(senhaC);
                                            if (strcmp(senhaC, novasenha)!=0)
                                            {
                                                printf(" as senhas não são iguais, não foi possivel atualizar a senha");
                                                system("pause");
                                                goto inicio;
                                            }
                                            strcpy(cad[i].senha, novasenha);

                                            //REGRAVAÇÃO DOS DADOS
                                            //gravação arquivo de leitura
                                            filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                            if(filetemporarioLeitura == NULL)
                                            {
                                                printf("gravação não foi possivel");
                                                fclose(filetemporarioLeitura);
                                                system("pause");
                                                goto inicio;
                                            }

                                            for(int j=0;j<numcad;j++){
                                                fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                            }
                                            fclose(filetemporarioLeitura);

                                            remove("dados_cadastro_leitura.csv");
                                            rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                            printf(" Registro atualizado com sucesso");
                                            getch();
                                            goto inicio;

                                            break;

                                        case '0':
                                        goto inicio;
                                        break;

                                        default:
                                        printf("digite uma opção valida");
                                        break;

                                    }

                                }
                                else {
                                    printf("Senha incorreta\n");
                                    system("pause");
                                    goto inicio;
                                }
                                break;

                            case '2':
                                printf(" para excluir esse registro você deve inserir a senha atual dele\n");
                                printf(" senha:");
                                gets(senhalogin);

                                if (strcmp(senhalogin,cad[i].senha)==0)
                                {
                                    limparString(cad[i].email);
                                    limparString(cad[i].cel);
                                    limparString(cad[i].pais);
                                    limparString(cad[i].cidade);
                                    limparString(cad[i].nome);
                                    limparString(cad[i].senha);

                                    filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                                        if(filetemporarioLeitura == NULL)
                                        {
                                            printf("gravação não foi possivel");
                                            fclose(filetemporarioLeitura);
                                            system("pause");
                                            goto inicio;
                                        }

                                    for(int j=0;j<numcad;j++)
                                    {
                                        fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[j].email, cad[j].cel, cad[j].pais, cad[j].cidade, cad[j].nome, cad[j].senha);
                                    }
                                    fclose(filetemporarioLeitura);

                                    remove("dados_cadastro_leitura.csv");
                                    rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                                    printf(" Registro Excluido com sucesso");
                                    getch();
                                    goto inicio;
                                }
                                else
                                {
                                    printf("Senha incorreta\n");
                                    system("pause");
                                    goto inicio;
                                }
                                break;

                            case '0':
                                goto inicio;
                                break;

                            default:
                                printf("Digite uma opção valida");
                                break;

                        }

                    }
                }
                if (strcmp(nomepesquisa, cad[i].nome)!=0)
                {
                    printf(" REGISTRO NÃO ENCONTRADO\n\n");
                    system("pause");
                    goto inicio;
                }
                break;

            case '7':
                validaringresso:
                system("cls");
                printf(" ------------------------------VALIDAR INGRESSOS------------------------------\n");
                printf(" Digite o número do ingresso:");

                if (scanf("%d", &numIngressoValidar) != 1 || getchar() != '\n')
                {
                    printf(" Digite um número inteiro válido.\n");
                    while (getchar() != '\n');
                    CLEAN_BUFF;
                    goto inicio;
                }

                for(int i=0;i<numDaVenda;i++)
                {
                    for(int j=0;j<dc[i].numIngressosComprados;j++)
                    {
                        if(numIngressoValidar==dc[i].ingressosComprados[j])
                        {
                            printf("\n ingresso %04d foi validado\n", dc[i].ingressosComprados[j]);
                            printf(" Dados do comprador\n\n nome:%s\n cpf:%s\n cel:%s\n email:%s\n\n",
                                       dc[i].nomeCompleto, dc[i].cpf, dc[i].celCliente, dc[i].emailCliente);

                            printf(" 1-validar mais ingressos\n 2-voltar para o menu");
                            opcao=getch();

                            switch(opcao)
                            {
                                case '1':
                                    goto validaringresso;
                                    break;

                                case '2':
                                    goto inicio;
                                    break;

                                default:
                                    printf("\n Digite uma opção valida\n");
                                    system("pause");
                                    CLEAN_BUFF;
                                    goto validaringresso;
                                    break;
                            }
                        }
                    }
                }
                printf("\n Ingresso não foi encontrado\n");
                system("pause");
                CLEAN_BUFF;
                goto inicio;
                break;

            case '0':
                loginSucesso=0;
                printf(" saindo da conta...");
                getch();
                goto inicio;
                break;

            default:
                printf(" Digite uma opção valida\n");
                system("pause");
                goto inicio;

            CLEAN_BUFF;
        }
    }
//FINAL DA SESSÃO POS LOGIN


//SESSÃO PRE LOGIN
setlocale(LC_ALL, "portuguese");

printf(" \n ------------------------------FAÇA SEU CADASTRO------------------------------\n");
printf(" |                                                                           |\n");
printf(" | 1-cadastro                                                                |\n");
printf(" |                                                                           |\n");
printf(" | 2-login                                                                   |\n");
printf(" |                                                                           |\n");
printf(" | 3-listar cadastros                                                        |\n");
printf(" |                                                                           |\n");
printf(" | 0-sair                                                                    |\n");
printf(" |                                                                           |\n");
printf(" -----------------------------------------------------------------------------\n");
opcao=getch();

switch (opcao){
    case '1':
        //CADASTRO
        system("cls");

        if(numcad>NUM_MAX_CAD)
        {
            printf(" Número maximo de cadastros atingindo\n\n");
            system("pause");
            goto inicio;
        }

        //PESQUISA POR UM CADASTRO VAZIO QUE FOI EXCLUIDO PARA GRAVAR AS NOVAS INFORMAÇOES NELE
        for (int j=0;j<numcad;j++)
        {
            setlocale(LC_ALL, "portuguese");
            if (strlen(cad[j].nome)==0)
            {
                printf(" -----------------------------CADASTRO-----------------------------\n");
                printf(" Email:");
                gets(cad[j].email);
                trim(cad[j].email);
                if(strlen(cad[j].email) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }

                printf(" Celular:");
                gets(cad[j].cel);
                trim(cad[j].cel);
                if(strlen(cad[j].cel) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }

                printf(" pais:");
                gets(cad[j].pais);
                trim(cad[j].pais);
                if(strlen(cad[j].pais) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }

                printf(" Cidade:");
                gets(cad[j].cidade);
                trim(cad[j].cidade);
                if(strlen(cad[j].cidade) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }

                printf(" Nome:");
                gets(cad[j].nome);
                trim(cad[j].nome);
                if(strlen(cad[j].nome) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                    system("pause");
                    goto inicio;
                }
                for(int i=0;i<j;i++)
                {
                    if(strcmp(cad[j].nome, cad[i].nome)==0)
                    {
                        printf(" Usuário ja existe, tente novamente e escolha outro nome\n\n");
                        system("pause");
                        goto inicio;
                    }
                }

                printf(" Senha:");
                gets(cad[j].senha);
                trim(cad[j].senha);
                if(strlen(cad[j].senha) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                        limparString(cad[j].nome);
                        limparString(cad[j].email);
                        limparString(cad[j].cel);
                        limparString(cad[j].pais);
                        limparString(cad[j].cidade);
                    system("pause");
                    goto inicio;
                }

                printf(" Confirme a senha:");
                gets(senhaC);
                trim(senhaC);
                if(strlen(senhaC) == 0)
                {
                    printf("Você deve digitar algo\n\n");
                        limparString(cad[j].nome);
                        limparString(cad[j].email);
                        limparString(cad[j].cel);
                        limparString(cad[j].pais);
                        limparString(cad[j].cidade);
                    system("pause");
                    goto inicio;
                }

                if(strcmp(cad[j].senha, senhaC) == 0)
                {

                    //gravação arquivo de leitura
                    filetemporarioLeitura = fopen("dados_cadastro_leituraTemp.csv", "a");
                            if(filetemporarioLeitura == NULL)
                            {
                                printf("gravação não foi possivel");
                                fclose(filetemporarioLeitura);
                                system("pause");
                                goto inicio;
                            }

                    for(int k=0;k<numcad;k++)
                    {
                        fprintf(filetemporarioLeitura, "%s,%s,%s,%s,%s,%s\n", cad[k].email, cad[k].cel, cad[k].pais, cad[k].cidade, cad[k].nome, cad[k].senha);
                    }
                    fclose(filetemporarioLeitura);

                    remove("dados_cadastro_leitura.csv");
                    rename("dados_cadastro_leituraTemp.csv", "dados_cadastro_leitura.csv");

                    printf(" ------------------------------------------------------------------\n");
                    printf(" cadastro concluido com suscesso\n\n");
                    system("pause");
                    goto inicio;
                }
                else if( strcmp( cad[numcad].senha, senhaC ) != 0)
                {
                    printf(" ------------------------------------------------------------------\n");
                    printf(" as senhas nao são iguais, tente novamente\n\n");
                        limparString(cad[j].nome);
                        limparString(cad[j].email);
                        limparString(cad[j].cel);
                        limparString(cad[j].pais);
                        limparString(cad[j].cidade);
                    system(" pause");
                    goto inicio;
                }
                goto inicio;
            }
        }

        //FAZ UM CADASTRO NOVO, CASO NAO EXISTA UM CADASTRO QUE FOI EXCLUIDO
        printf(" -----------------------------CADASTRO-----------------------------\n");
        printf(" Email:");
        gets(cad[numcad].email);
        trim(cad[numcad].email);
        if(strlen(cad[numcad].email) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        printf(" Celular:");
        gets(cad[numcad].cel);
        trim(cad[numcad].cel);
        if(strlen(cad[numcad].cel) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        printf(" pais:");
        gets(cad[numcad].pais);
        trim(cad[numcad].pais);
        if(strlen(cad[numcad].pais) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        printf(" Cidade:");
        gets(cad[numcad].cidade);
        trim(cad[numcad].cidade);
        if(strlen(cad[numcad].cidade) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        printf(" Nome:");
        gets(cad[numcad].nome);
        trim(cad[numcad].nome);
        if(strlen(cad[numcad].nome) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }
            for(int i=0;i<numcad;i++)
            {
                if(strcmp(cad[numcad].nome, cad[i].nome)==0)
                {
                    printf(" Usuário ja existe, tente novamente e escolha outro nome\n\n");
                    system("pause");
                    goto inicio;
                }
            }

        printf(" Senha:");
        gets(cad[numcad].senha);
        trim(cad[numcad].senha);
        if(strlen(cad[numcad].senha) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        printf(" Confirme a senha:");
        gets(senhaC);
        trim(senhaC);
        if(strlen(senhaC) == 0)
        {
            printf("Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        if(strcmp(cad[numcad].senha, senhaC) == 0)
        {

            //gravação arquivo de leitura
            dadosCadastroLeitura = fopen("dados_cadastro_leitura.csv", "a");

            if(dadosCadastroLeitura==NULL)
            {
                printf("gravação não foi possivel");
                fclose(dadosCadastroLeitura);
                system("pause");
                goto inicio;
            }
            fprintf(dadosCadastroLeitura, "%s,%s,%s,%s,%s,%s\n", cad[numcad].email, cad[numcad].cel, cad[numcad].pais, cad[numcad].cidade, cad[numcad].nome, cad[numcad].senha);
            fclose(dadosCadastroLeitura);

            printf(" ------------------------------------------------------------------\n");
            printf(" cadastro concluido com suscesso\n\n");
            system("pause");

            //gravação arquivo do numcad
            filenumeCad = fopen("numecad.csv", "w");

            if(filenumeCad==NULL)
            {
                printf("gravação do numcad não foi possivel");
                fclose(filenumeCad);
                system("pause");
                goto inicio;
            }
            numcad+=1;
            fprintf(filenumeCad, "%d", numcad);
            fclose(filenumeCad);


        }
        else if( strcmp( cad[numcad].senha, senhaC ) != 0)
        {
            printf(" ------------------------------------------------------------------\n");
            printf(" as senhas nao são iguais, tente novamente\n\n");
            system(" pause");
            goto inicio;
        }
        goto inicio;
        break;

    case '2':
        //LOGIN
        system("cls");
        printf(" -----------------------------LOGIN-----------------------------\n");
        printf(" Usuário:");
        gets(nomelogin);

        trim(nomelogin);
        if(strlen(nomelogin) == 0)
        {
            printf(" Você deve digitar algo\n\n");
            system("pause");
            goto inicio;
        }

        //login adm, possibilita fazer login sem exixtir cadastro
        if(strcmp(adm, nomelogin)==0)
        {
            printf(" Senha:");
            gets(senhalogin);

            if(strcmp(senha_adm, senhalogin)==0)
            {
                printf(" ------------------------------------------------------------------\n");
                printf(" login bem sucedido, seja bem vindo ADM\n\n");
                loginSucesso = 1;
                system("pause");
                goto inicio;
            }
            else
            {
                printf("senha incorreta\n\n");
                system("pause");
                goto inicio;
            }
        }

        for(int i=0;i<numcad;i++)
        {
            if(strcmp(cad[i].nome, nomelogin)==0)
            {
                printf(" Senha:");
                gets(senhalogin);

                    if(strcmp(cad[i].senha, senhalogin)==0)
                    {
                        printf(" ---------------------------------------------------------------\n");
                        printf(" login bem sucedido, seja bem vindo %s\n\n", cad[i].nome);
                        loginSucesso = 1;
                        CLEAN_BUFF;
                        goto inicio;
                    }
                    else if(loginSucesso==0)
                    {
                        printf(" ---------------------------------------------------------------\n");
                        printf(" Senha incorreta\n\n");
                        CLEAN_BUFF;
                        goto inicio;
                    }
            }
        }
        printf(" ---------------------------------------------------------------\n");
        printf(" Usuário não existe\n\n");
        CLEAN_BUFF;
        goto inicio;

    case '0':
        exit(0);

    case '3':

        //LISTA CADASTRO FEITOS
        system("cls");
        printf(" --------------------------CADASTRO FEITOS--------------------------\n");
        for (int i=0;i<numcad;i++)
            {
                printf(" Dados do cadastro\n\n");
                printf(" email:%s\n", cad[i].email);
                printf(" cel:%s\n", cad[i].cel);
                printf(" pais:%s\n", cad[i].pais);
                printf(" cidade:%s\n", cad[i].cidade);
                printf(" nome de usuário:%s\n\n", cad[i].nome);
                printf(" -------------------------------------------------------------------\n");
            }

        getch();
        goto inicio;

    default:
        printf("digite uma opção valida\n\n");
        system("pause");
        goto inicio;

    CLEAN_BUFF;

}
return 0;
}
