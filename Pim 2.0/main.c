#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#define users 2 //define quantos usuários podem ser cadastrados
#define maxp 100 // define o máximo de produtos que  podem ser cadastrados
#define maxCupom 75 //define o máximo de cupons fiscais que podem ser gerados

/*
    Estruturas
*/

//--- Estrutura Usuários ---
typedef struct{
    char usuario[16];
    char senha[16];
}ficha_login;

//--- Estrutura Produtos ---
typedef struct{
    bool peso;
    char produto[24];
    float valor;
    float quantidade;
}ficha_produtos;

//--- Estrutura Nota Fiscal ---
typedef struct{
    bool peso;
    char produto[24];
    float valor;
    float quantidade;
}ficha_nota_fiscal;

/*
    Váriavel global
*/

float totalDia = 0;
int numVendas = 0;

/*
    Funções
*/

//--- Limpar tela ---
void limparTela(){
    system("cls || clear");
}

//--- Verificar se nome já existe ---
int verificarNome(char nome[], int total, ficha_produtos produtos[]){
    char produto[24];
    char guard[24];
    char teste;
    int i, cont = 0;

    //Salvo a entrada inicial do gerente
    strcpy(guard, nome);

    for(i = 0;i < strlen(nome);i++){
        teste = tolower(nome[i]);
        nome[i] = teste;


        if((nome[i] < 97 || nome[i] > 122) && nome[i] != 32 && nome[i] != 45){
            return 2;
        }

    }

    for(i = 0;i < total;i++){
        strcpy(produto, produtos[i].produto);

        for(int j = 0;j < strlen(produto);j++){
            teste = tolower(produto[j]);
            produto[j]= teste;
        }

        if(strcmp(nome, produto) == 0){
            cont++;
        }
    }

    if(cont >= 1){
        return 1;
    }else{
        //Devolvo a entrada original do usuário para ser salva na estrutura
        strcpy(nome, guard);
        return 0;
    }

}

//--- Verifica se o Id não é cadastrado ---
int verificar(int id, int total){

    if(id >= 0 && id < total){
        return 1;
    }else{
        return 0;
    }

}

//--- Verificação se o número é <= 0 ---
int verificarNumero(float valor){
    if(valor <= 0){
        return 0;
    }else{
        return 1;
    }
}

//--- Verificar decimal ---
int verificarDecimal(float quantia){

    if(quantia - trunc(quantia) == 0){
        return 1;
    }else{
        return 0;
    }

}

//--- Adicionar produto ---
void addProduto(ficha_produtos produtos[], int *total){
    int opcaoAdd;
    int repete;
    float valor, quantia;
    char nome[24];

    if(*total < maxp){
        //--- Verificação se já consta o nome na estrutura ---
        do{
            fflush(stdin);//limpa o buffer

            printf("\n  Insira o nome do produto: ");
            scanf("%[^\n]", &nome);

            if(verificarNome(nome, *total, produtos) == 2){
                printf("\n  O nome só pode conter letras! Tente de novo como no exemplo!");
                printf("\n  Exemplo: Banana Prata\n           Maca\n");
            }

            if(verificarNome(nome, *total, produtos) == 1){
                printf("\n  Já existe esse nome! Tente outro nome para o produto!\n");
            }

            if(verificarNome(nome, *total, produtos) == 0){
                strcpy(produtos[*total].produto, nome);
            }
        }while(verificarNome(nome, *total, produtos) != 0);

        do{
            printf("\n  A quantidade vai ser tratada em unidade ou kg?");
            printf("\n  [1]-Unidade");
            printf("\n  [2]-Kg\n  :");
            scanf("%d", &opcaoAdd);
        }while(opcaoAdd < 1 || opcaoAdd > 2);

        switch(opcaoAdd){
            case 1:
                //--- Produtos por unidade ---
                produtos[*total].peso = false;

                //--- Verificacação do valor do produto ---
                do{
                    printf("\n  Insira o valor do produto por unidade: ");
                    scanf("%f", &valor);

                    if(verificarNumero(valor) > 0){
                        produtos[*total].valor = valor;
                    }else{
                        printf("\n  Valor negativo ou igual a 0! Insira novamente outro valor!\n");
                    }
                }while(verificarNumero(valor) < 1);

                //--- Verificação de quantidade em números sem casa decimal ---
                do{
                    printf("\n  Insira a quantidade do produto em unidade: ");
                    scanf("%f", &quantia);

                    if((verificarDecimal(quantia) > 0) && (verificarNumero(quantia) > 0)){
                        produtos[*total].quantidade = quantia;
                        repete = 1;
                    }else{
                        printf("\n  Valor negativo ou igual a 0! Ou valor com casa decimal!\n  Insira novamente!\n");
                        repete = 0;
                    }
                }while(repete < 1);

                break;
            case 2:
                produtos[*total].peso = true;

                //--- Verificacação do valor do produto ---
                do{
                    printf("\n  Insira o valor do produto por kg: ");
                    scanf("%f", &valor);

                    if(verificarNumero(valor) > 0){
                        produtos[*total].valor = valor;
                    }else{
                        printf("\n  Valor negativo ou igual a 0! Insira novamente outro valor!\n");
                    }
                }while(verificarNumero(valor) < 1);

                //--- Verificação de quantidade ---
                do{
                    printf("\n  Insira a quantidade do produto em kg: ");
                    scanf("%f", &quantia);

                    if(verificarNumero(quantia) > 0){
                        produtos[*total].quantidade = quantia;
                        repete = 1;
                    }else{
                        printf("\n  Valor negativo ou igual a 0! Insira novamente!\n");
                        repete = 0;
                    }
                }while(repete < 1);

                break;
        }

        printf("\n  Produto cadastrado com sucesso!\n\n  ");
        system("pause");

        (*total)++;
    }else{
        printf("\n  O número máximo de produtos cadastrados foi atingido!\n  ");
        system("pause");
    }
}

//--- Listar produtos ---
void listarProduto(ficha_produtos produtos[], int total){
    int i;

    if(total == 0){
        printf("\n  Não há produtos cadastrados!");
    }else{
        printf("\n  ============ PRODUTOS ============\n");

        for(i = 0;i < total;i++){
            printf("  Id: %d", i);
            printf("\n  Produto: %s", produtos[i].produto);

            if(produtos[i].peso != true){
                printf("\n  Valor do produto: %.2f a unidade", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.0f unidades", produtos[i].quantidade);
            }else{
                printf("\n  Valor do produto: %.2f o kg", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.2f kg", produtos[i].quantidade);
            }
            printf("\n  ==================================\n");
        }
    }

}

//--- Exibir produto ---
void exibirProduto(ficha_produtos produtos[], int id){

    printf("\n  =========================");

    printf("\n  Nome do produto: %s", produtos[id].produto);

    if(produtos[id].peso == true){
        printf("\n  Valor do produto: %.2f o kg", produtos[id].valor);
        printf("\n  Quantidade no estoque: %.2f kg", produtos[id].quantidade);
    }else{
        printf("\n  Valor do produto: %.2f a unidade", produtos[id].valor);
        printf("\n  Quantidade no estoque: %.0f unidades", produtos[id].quantidade);
    }

    printf("\n  =========================\n");

}

//--- Alterar produtos ---
void altProduto(ficha_produtos produtos[], int total){
    int idA, opcaoalt, verificaId, repeteNome, repetePeso;
    float valor, quantia;
    char novoNome[24];

    listarProduto(produtos, total);

    do{

        printf("\n  Insira o Id do produto que deseja alterar: ");
        scanf("%d", &idA);

        if(verificar(idA, total) == 1){
            do{
                limparTela();

                exibirProduto(produtos, idA);

                printf("\n\n  Qual dos atributos você deseja alterar?");
                printf("\n  [1]-Nome do produto");
                printf("\n  [2]-Valor do produto");
                printf("\n  [3]-Quantidade");
                printf("\n  [0]-Sair\n  :");
                scanf("%d", &opcaoalt);

                switch(opcaoalt){
                    case 1: //Nome do produto
                        do{
                            fflush(stdin);

                            printf("\n  Insira o novo nome do produto: ");
                            scanf("%[^\n]", &novoNome);

                            if(verificarNome(novoNome, total, produtos) == 2){
                                printf("\n  O nome só pode conter letras! Tente de novo como no exemplo!");
                                printf("\n  Exemplo: Banana Prata\n           Maca\n");
                                repeteNome = 1;
                            }

                            if(verificarNome(novoNome, total, produtos) == 1){
                                printf("\n  Já existe esse nome! Tente outro nome para o produto!\n");
                                repeteNome = 1;
                            }

                            if(verificarNome(novoNome, total, produtos) == 0){
                                strcpy(produtos[idA].produto, novoNome);
                                repeteNome = 0;
                            }
                        }while(repeteNome > 0);

                        break;
                    case 2: //Valor do produto
                        do{
                            printf("\n  Insira o novo valor: ");
                            scanf("%f", &valor);

                            if(verificarNumero(valor) > 0){
                                produtos[idA].valor = valor;
                            }else{
                                printf("\n  Valor negativo ou igual a 0! Insira novamente outro valor!\n");
                            }
                        }while(verificarNumero(valor) < 1);

                        break;
                    case 3: //Alterar quantidade de produto em estoque
                        do{
                            printf("\n  Insira a nova quantidade: ");
                            scanf("%f", &quantia);

                            if(produtos[idA].peso == true){
                                if(verificarNumero(quantia) > 0){
                                    produtos[idA].quantidade = quantia;
                                    repetePeso = 0;
                                }else{
                                    printf("\n  Valor negativo ou igual a 0! Insira novamente!\n");
                                    repetePeso = 1;
                                }
                            }else{
                                if((verificarDecimal(quantia) > 0) && (verificarNumero(quantia) > 0)){
                                    produtos[idA].quantidade = quantia;
                                    repetePeso = 0;
                                }else{
                                    printf("\n  Valor negativo ou igual a 0! Ou valor com casa decimal!\n  Insira novamente!\n");
                                    repetePeso = 1;
                                }
                            }

                        }while(repetePeso > 0);

                        break;
                    case 0:
                        verificaId = 0;
                        break;
                    default:
                        printf("\n  Insira um valor válido!\n  ");
                        system("pause");
                        limparTela();
                        listarProduto(produtos, total);
                        verificaId = 1;
                        break;
                }
            }while(opcaoalt > 0 && opcaoalt < 4);
        }else{
            printf("\n  Número de id inválido! Deseja inserir novamente?");
            printf("\n  [1]-Sim");
            printf("\n  [0]-Não\n  :");
            scanf("%d", &verificaId);
        }

    }while(verificaId > 0 && verificaId < 2);
}

//--- Excluir produtos ---
void exProduto(ficha_produtos produtos[], int *total){
    int idExcluir, opcaoEx, testeEx, testeEx2;
    int k;

    if(*total == 0){
        printf("\n  Não há produtos cadastrados!\n  ");
        system("pause");
    }else{
        do{
            if(*total == 0){
                printf("\n  Não há produtos cadastrados!\n  ");
                system("pause");
                break;
            }


            listarProduto(produtos, *total);

            printf("\n  Insira o id do produto que deseja excluir: ");
            scanf("%d", &idExcluir);

            if(verificar(idExcluir, *total) == 1){
                for(k = idExcluir;k < *total;k++){
                    produtos[k] = produtos[k + 1];
                }

                (*total)--;

                printf("\n  Produto excluído com sucesso!\n");

                do{
                    printf("\n  Deseja excluir outro produto?");
                    printf("\n  [1]-Sim");
                    printf("\n  [0]-Não\n  :");
                    scanf("%d", &testeEx2);
                }while(testeEx2 != 0 && testeEx2 != 1);

                if(testeEx2 == 1){
                    opcaoEx = 1;
                    limparTela();
                }else{
                    opcaoEx = 0;
                    limparTela();
                }

            }else{
                do{
                    printf("\n  Id inválido! Deseja tentar novamente?");
                    printf("\n  [1]-Sim");
                    printf("\n  [0]-Não\n  :");
                    scanf("%d", &testeEx);
                }while(testeEx != 0 && testeEx != 1);

                if(testeEx == 1){
                    opcaoEx = 1;
                    limparTela();
                }else{
                    opcaoEx = 0;
                    limparTela();
                }
            }
        }while(opcaoEx != 0);

    }
}

//--- Listar apenas produtos com quantidade > 0 ---
void listarProdutos_Maior_0(ficha_produtos produtos[], int total){
    int i;

    printf("\n  ======== PRODUTOS DISPONÍVEIS ========\n");

    for(i = 0;i < total;i++){
        if(produtos[i].quantidade > 0){
            printf("  Id: %d", i);
            printf("\n  Produto: %s", produtos[i].produto);

            if(produtos[i].peso != true){
                printf("\n  Valor por unidade: %.2f", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.0f unidades", produtos[i].quantidade);
            }else{
                printf("\n  Valor por kg: %.2f", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.2f kg", produtos[i].quantidade);
            }
            printf("\n  ======================================\n");
        }
    }
}

//--- Relatórios de vendas ---
void relatorio(ficha_produtos produtos[], int total){
    int i;
    int cont = 0; //contador de produtos esgotados

    listarProdutos_Maior_0(produtos, total);

    printf("\n");

    printf("\n  ========= PRODUTOS ESGOTADOS =========");
    for(i = 0;i < total;i++){
        if(produtos[i].quantidade <= 0){
            printf("\n  Id: %d", i);
            printf("\n  Produto: %s", produtos[i].produto);

            if(produtos[i].peso != true){
                printf("\n  Valor por unidade: %.2f", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.0f unidades", produtos[i].quantidade);
            }else{
                printf("\n  Valor por kg: %.2f", produtos[i].valor);
                printf("\n  Quantidade no estoque: %.2f kg", produtos[i].quantidade);
            }
            printf("\n  ======================================\n");
            cont++;
        }
    }

    if(cont < 1){
        printf("\n  ====== NÃO HÁ PRODUTOS ESGOTADOS =====\n");
    }

    printf("\n  Quantidade de compras feitas: %d | Lucro total: %.2f\n\n  ", numVendas, totalDia);
    system("pause");
}

//--- Cupom Fiscal ---
void cupomFiscal(ficha_produtos produtos[], ficha_nota_fiscal nota[], int numNotas, int idVenda, float valorCompra, float quantia){

    strcpy(nota[numNotas].produto, produtos[idVenda].produto);
    nota[numNotas].peso = produtos[idVenda].peso;
    nota[numNotas].valor = valorCompra;
    nota[numNotas].quantidade = quantia;

}

//--- Vender produtos ---
void venderProduto(ficha_produtos produtos[], int total){
    ficha_nota_fiscal nota[maxCupom];

    int numNotas = 0;
    int idVenda;
    int tentarNovamente, repete, repeteCompra;
    float quantia, valorCompra, valorTotal = 0;

    do{
        valorCompra = 0;

        //lista apenas produtos com quantidade > 0
        listarProdutos_Maior_0(produtos, total);

        printf("\n  Insira o id do produto que deseja vender: ");
        scanf("%d", &idVenda);

        //Verificação e mensagem de erro
        if(verificar(idVenda, total) == 1 && produtos[idVenda].quantidade > 0){

            //Exibe o produto selecionado
            exibirProduto(produtos, idVenda);

            repete = 0;

            //----- produto em unidade -----
            if(produtos[idVenda].peso == false){
                do{
                    printf("\n  Insira a quantidade que deseja vender: ");
                    scanf("%f", &quantia);

                    if(verificarDecimal(quantia) == 1 && verificarNumero(quantia) == 1 && quantia <= produtos[idVenda].quantidade){

                        //Valor dessa compra
                        valorCompra = produtos[idVenda].valor * quantia;

                        //Baixa no estoque
                        produtos[idVenda].quantidade -= quantia;

                        //Gravação da nota fiscal
                        cupomFiscal(produtos, nota, numNotas, idVenda, valorCompra, quantia);

                        //Incremento para próxima nota fiscal
                        numNotas++;

                        //Montante
                        valorTotal += valorCompra;

                        printf("\n  Deseja realizar outra venda?");
                        printf("\n  [1]-Sim\n  [2]-Não\n  :");
                        scanf("%d", &repeteCompra);

                        switch(repeteCompra){
                            case 1:
                                tentarNovamente = 1;
                                repete = 1;
                                limparTela();
                                break;
                            case 2:
                                tentarNovamente = 0;
                                repete = 1;
                                limparTela();
                                break;
                            default:
                                tentarNovamente = 0;
                                repete = 1;
                                limparTela();
                                break;
                        }
                    }else{
                        printf("\n  Deu errado! Tente novamente!\n  ");
                        system("pause");
                        repete = 0;
                    }

                }while(repete < 1);
            }

            //----- produto em kg -----
            if(produtos[idVenda].peso == true){
                do{
                    printf("\n  Insira a quantidade que deseja vender: ");
                    scanf("%f", &quantia);

                    if(verificarNumero(quantia) == 1 && quantia <= produtos[idVenda].quantidade){

                        //Valor dessa compra
                        valorCompra = produtos[idVenda].valor * quantia;

                        //Baixa no estoque
                        produtos[idVenda].quantidade -= quantia;

                        //Gravação da nota fiscal
                        cupomFiscal(produtos, nota, numNotas, idVenda, valorCompra, quantia);

                        //Incremento para próxima nota fiscal
                        numNotas++;

                        //Montante
                        valorTotal += valorCompra;

                        printf("\n  Deseja realizar outra venda?");
                        printf("\n  [1]-Sim\n  [2]-Não\n  :");
                        scanf("%d", &repeteCompra);

                        switch(repeteCompra){
                            case 1:
                                tentarNovamente = 1;
                                repete = 1;
                                limparTela();
                                break;
                            case 2:
                                tentarNovamente = 0;
                                repete = 1;
                                limparTela();
                                break;
                            default:
                                tentarNovamente = 0;
                                repete = 1;
                                limparTela();
                                break;
                        }
                    }else{
                        printf("\n  Deu errado! Tente novamente!\n  ");
                        system("pause");
                        repete = 0;
                    }

                }while(repete < 1);
            }

        }else{
            printf("\n  Id inválido!");

            printf("\n  Deseja tentar novamente?");
            printf("\n  [1]-Sim\n  [2]-Não\n  :");
            scanf("%d", &tentarNovamente);

            limparTela();
        }

    }while(tentarNovamente == 1);

    if(numNotas > 0){
        //--- Nota Fiscal ---
        printf("\n  ===================== CUPOM FISCAL =====================\n");
        for(int i = 0;i < numNotas;i++){
            printf("\n  Produto: %s", nota[i].produto);

            if(nota[i].peso == true){
                printf(" | Quantidade: %.2f kg", nota[i].quantidade);
            }else{
                printf(" | Quantidade: %.0f unidades", nota[i].quantidade);
            }

            printf(" | Valor: %.2f\n", nota[i].valor);
            printf("  -------------------------------------------------------");
        }
        printf("\n  Total: %.2f", valorTotal);
        printf("\n  ========================================================\n  ");

        //--- Relátorio ---
        numVendas++;
    }else{
        printf("\n  Nenhuma venda foi feita!\n  ");
    }

    totalDia += valorTotal;

    system("pause");

}

/*
    Programa principal
*/
int main(){
    setlocale(LC_ALL, "Portuguese");

    ficha_login login[users]; //declaração usuários
    ficha_produtos produtos[maxp]; //declaração produtos

    int *total = 0;
    int cont, opcaog, opcaocaixa;
    char luser[16];
    char lsenha[16];

    /*
        USUÁRIOS
    */

    //gerente
    strcpy(login[0].usuario, "gerente");
    strcpy(login[0].senha, "gerente123");

    //caixa
    strcpy(login[1].usuario, "caixa");
    strcpy(login[1].senha, "caixa123");

    /*
        PRODUTOS
    */

    //Laranja
    strcpy(produtos[0].produto, "Laranja");
    produtos[0].peso = true;
    produtos[0].valor = 7.65;
    produtos[0].quantidade = 2;

    //Couve Flor
    strcpy(produtos[1].produto, "Couve-flor");
    produtos[1].peso = false;
    produtos[1].valor = 2.50;
    produtos[1].quantidade = 20;

    total = 2;

    /*
        LOGIN
    */

    do{
        cont = 0; //zera o contador a cada looping

        limparTela();

        printf("\n  Insira o usuário: ");
        scanf("%s", &luser);

        printf("\n  Insira a senha: ");
        scanf("%s", &lsenha);

        //--- Teste login ---
        if(strcmp(luser, login[0].usuario) == 0){
            if(strcmp(lsenha, login[0].senha) == 0){
                do{
                    opcaog = 0; //zera o contador de opção

                    limparTela();

                    printf("\n  ========== GERENTE ==========\n");

                    printf("\n  [1]-Adicionar produto");
                    printf("\n  [2]-Listar produtos");
                    printf("\n  [3]-Alterar produto");
                    printf("\n  [4]-Excluir produto");
                    printf("\n  [5]-Relatório gerencial");
                    printf("\n  [6]-Trocar de usuário");
                    printf("\n  [7]-Finalizar programa\n  :");
                    scanf("%d", &opcaog);

                    switch(opcaog){
                        case 1:
                            addProduto(produtos, &total);
                            break;
                        case 2:
                            listarProduto(produtos, total);
                            printf("\n  ");
                            system("pause");
                            break;
                        case 3:
                            altProduto(produtos, total);
                            break;
                        case 4:
                            exProduto(produtos, &total);
                            break;
                        case 5:
                            relatorio(produtos, total);
                            break;
                        case 6:
                            cont = 1;
                            break;
                        case 7:
                            cont = 0;
                            break;
                    }

                }while(opcaog != 6 && opcaog != 7);
            }
        }else{
            if(strcmp(luser, login[1].usuario) == 0){
                if(strcmp(lsenha, login[1].senha) == 0){
                    do{
                        limparTela();

                        printf("\n  ========== CAIXA ==========\n");

                        printf("\n  [1]-Realizar venda");
                        printf("\n  [2]-Trocar de usuário");
                        printf("\n  [3]-Finalizar programa\n  :");
                        scanf("%d", &opcaocaixa);

                        switch(opcaocaixa){
                            case 1:
                                //printf("\nAqui realiza a venda");
                                venderProduto(produtos, total);
                                break;
                            case 2:
                                cont = 1;
                                break;
                            case 3:
                                break;
                        }

                    }while(opcaocaixa != 3 && opcaocaixa != 2);
                }
            }
        }

        //--- Teste erro ---
        if(strcmp(luser, login[0].usuario) != 0 && strcmp(luser, login[1].usuario) != 0){
            printf("\n  Usuário ou senha inváldos! Tente novamente!");

            printf("\n  Deseja tentar novamente?");
            printf("\n  [1] - Sim\n  [0] - Não\n  :");
            scanf("%d", &cont);
        }else{
            if(strcmp(lsenha, login[0].senha) != 0 && strcmp(lsenha, login[1].senha) != 0){
                printf("\n  Usuário ou senha inváldos! Tente novamente!");

            printf("\n  Deseja tentar novamente?");
            printf("\n  [1] - Sim\n  [0] - Não\n  :");
            scanf("%d", &cont);
            }
        }

    }while(cont == 1);

    printf("\n  Programa finalizado!");

    return 0;
}
