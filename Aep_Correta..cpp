#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------

//constantes (Valores n�o v�o mudar)
#define Max_senha 30  //Definir o n�mero m�ximo de senhas que poder� ser armazenadas
#define Cap_senha 100  //Definir o tamanho m�ximo de cada senha
#define Cap_nome 50    //Definir o tamanho m�ximo do nome de usu�rio
#define Arquivo "Salvar_Senhas.txt"  //Nome do arquivo txt onde as senhas ser�o salvas

//--------------------------------------------------------------------------------------------------------------------------------------------------

//struct
typedef struct S { //Usando o typedef para usar o nome da struct sem a necessidade de 'struct' antes.
    char nome[Cap_nome];
    char senha[Cap_senha];
} Usuario;

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o de criptografia com base na tabela ASCII
char *criptograf(char *senha) {
    static char senha_criptografada[Cap_senha];  //Recebe uma senha como entrada e criptografa ela usando uma transforma��o na tabela ASCII
    for (int i = 0; i < strlen(senha); i++) {	//strlen para calcular o comprimento
        senha_criptografada[i] = senha[i] + 6;  //Incrementa 6 no valor ASCII de cada caractere
    }
    senha_criptografada[strlen(senha)] = '\0';  //Garantir o fim da string
    return senha_criptografada;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o de descriptografia (Inverte a criptografia)
char *descriptograf(char *senha) {
    static char senha_descriptografada[Cap_senha];
    for (int i = 0; i < strlen(senha); i++) {
        senha_descriptografada[i] = senha[i] - 6;  //Decrementa 6 no valor ASCII de cada caractere
    }
    senha_descriptografada[strlen(senha)] = '\0';  //Garantir o fim da string
    return senha_descriptografada;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para salvar as senhas 
void salvar_senha(Usuario senhas[Max_senha], int cont) {
    FILE *file = fopen(Arquivo, "w");  //Abre o arquivo para poder escrever
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar as senhas!\n");
        return;
    }
    for (int i = 0; i < cont; i++) {
        fprintf(file, "%s %s\n", senhas[i].nome, senhas[i].senha);  //Salva o nome e senha criptografado
    }
    fclose(file);  //Fecha o arquivo
    printf("Senhas salvas no arquivo com sucesso!\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para carregar as senhas
void carregar_senha(Usuario senhas[Max_senha], int *cont) {	//tipo de dados inteiro; O (*) indica que cont � um ponteiro; cont nome da variavel ponteiro
    FILE *file = fopen(Arquivo, "r");  //Abre o arquivo para ler
    if (file == NULL) {
        printf("Nenhuma senha salva encontrada. Iniciando o programa sem senhas.\n");
        return;
    }
    int i = 0;
    while (fscanf(file, "%s %s", senhas[i].nome, senhas[i].senha) != EOF && i < Max_senha) {
        //Descriptografa a senha ao carregar o arquivo
        strcpy(senhas[i].senha, descriptograf(senhas[i].senha));	//strcpy para copiar conte�do de string
        i++;
    }
    *cont = i;
    fclose(file);  //Fecha o arquivo
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para cadastar as senhas
void cadastrar_senha(Usuario senhas[Max_senha], int *cont) {	//tipo de dados inteiro; O (*) indica que cont � um ponteiro; cont nome da variavel ponteiro
    if (*cont >= Max_senha) {
        printf("Limite de senhas atingido!\n");
        return;
    }
    char nome[Cap_nome], senha[Cap_senha];
    printf("Digite o nome do usu�rio: ");
    scanf("%s", nome);
    printf("Digite a senha a ser cadastrada: ");
    scanf("%s", senha);
    
    //Criptografa a senha e guarda/armazena
    strcpy(senhas[*cont].nome, nome);						
    strcpy(senhas[*cont].senha, criptograf(senha));		//strcpy para copiar conte�do de string
    (*cont)++;
    printf("Senha cadastrada com sucesso!");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para listar as senhas
void listar_senha(Usuario senhas[Max_senha], int cont) { 
    printf("Senhas cadastradas:\n");
    for (int i = 0; i < cont; i++) {
        printf("%d: Usu�rio: %s, Senha: %s\n", i + 1, senhas[i].nome, senhas[i].senha);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para atualizar as senhas
void atualizar_senha(Usuario senhas[Max_senha], int cont) {
    char nome[Cap_nome], senha[Cap_senha];
    int pos = -1;
    
    if (cont == 0) {
        printf("Nenhuma senha cadastrada para atualizar.\n");
        return;
    }

    printf("Digite seu nome de usu�rio: ");
    scanf("%s", nome);

    //Busca o usu�rio
    for (int i = 0; i < cont; i++) {
        if (strcmp(senhas[i].nome, nome) == 0) {	//strcmp para comparar duas strings
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Usu�rio n�o encontrado.\n");
        return;
    }

    printf("Digite a nova senha: ");
    scanf("%s", senha);

    //Criptografa e atualiza a senha
    strcpy(senhas[pos].senha, criptograf(senha));	//strcpy para copiar conte�do de string
    printf("Senha atualizada com sucesso!\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o para deletar as senhas
void deletar_senha(Usuario senhas[Max_senha], int *cont) {	//tipo de dados inteiro; O (*) indica que cont � um ponteiro; cont nome da variavel ponteiro
    char nome[Cap_nome];
    int pos = -1;

    if (*cont == 0) {
        printf("Nenhuma senha cadastrada para deletar.\n");
        return;
    }

    printf("Digite seu nome de usu�rio: ");
    scanf("%s", nome);

    //Busca o usu�rio
    for (int i = 0; i < *cont; i++) {
        if (strcmp(senhas[i].nome, nome) == 0) { 	//strcmp para comparar duas strings
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Usu�rio n�o encontrado.\n");
        return;
    }

    //Usando para mover todas as senhas subsequentes uma posi��o para "tr�s"
    for (int i = pos; i < *cont - 1; i++) {
        senhas[i] = senhas[i + 1];
    }

    (*cont)--;  //Usando para reduzir o contador de senhas
    printf("Senha deletada com sucesso!\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------

//Fun��o principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    Usuario senha[Max_senha];
    int cont = 0;
    int opcao;

    //Carrega as senhas do arquivo no in�cio
    carregar_senha(senha, &cont);

    do {
        printf("    Menu \n");
        printf("1.Cadastrar senha \n");
        printf("2.Listar senhas \n");
        printf("3.Atualizar senha \n");
        printf("4.Deletar senha \n");
        printf("0.Sair \n");
        printf("Escolha a op��o: ");
        scanf("%d", &opcao);
        printf("===================\n");

        switch (opcao) {
            case 1:
                cadastrar_senha(senha, &cont);
                break;
            case 2:
                listar_senha(senha, cont);
                break;
            case 3:
                atualizar_senha(senha, cont);
                break;
            case 4:
                deletar_senha(senha, &cont);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Op��o inv�lida!\n");
        }

        //Salva as senhas no arquivo txt sempre ap�s uma modifica��o
        if (opcao != 0) {
            salvar_senha(senha, cont);
        }

    } while (opcao != 0);

    return 0;
}
