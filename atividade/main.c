#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

struct Artista {
    char nome[100];
    char genero[100];
    char local[100];
    char **albums;
    int num_albums;
};


struct Artista lerArtista(FILE *arquivo) {
    struct Artista artista;
    char linha[100];
    
    if (fgets(artista.nome, sizeof(artista.nome), arquivo) == NULL) {
        strcpy(artista.nome, "FIM"); 
        return artista;
    }

    artista.nome[strcspn(artista.nome, "\n")] = '\0'; 
    fgets(artista.genero, sizeof(artista.genero), arquivo);
    artista.genero[strcspn(artista.genero, "\n")] = '\0'; 
    fgets(artista.local, sizeof(artista.local), arquivo);
    artista.local[strcspn(artista.local, "\n")] = '\0'; 
    
    artista.num_albums = 0;
    artista.albums = NULL;
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strcmp(linha, "==========\n") == 0) {
            break;
        }
        artista.num_albums++;
        artista.albums = (char **)realloc(artista.albums, artista.num_albums * sizeof(char *));
        artista.albums[artista.num_albums - 1] = strdup(linha);
    }
    
    return artista;
}

void imprimirArtista(struct Artista artista) {
    printf("Nome: %s\n", artista.nome);
    printf("Genero: %s\n", artista.genero);
    printf("Local: %s\n", artista.local);
    printf("Albuns:\n");
    for (int i = 0; i < artista.num_albums; i++) {
        printf("%s", artista.albums[i]);
    }
    printf("==========\n");
}

void inserirArtista(struct Artista novoArtista, struct Artista **artistas, int *num_artistas) {
   
    *artistas = (struct Artista *)realloc(*artistas, (*num_artistas + 1) * sizeof(struct Artista));

    int posicao = 0;
    while (posicao < *num_artistas && strcmp(novoArtista.nome, (*artistas)[posicao].nome) > 0) {
        posicao++;
    }

    for (int i = *num_artistas; i > posicao; i--) {
        (*artistas)[i] = (*artistas)[i - 1];
    }

    (*artistas)[posicao] = novoArtista;

    (*num_artistas)++;

    FILE *arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\atividade\\artistas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita(verifique se o caminho do arquivo está correto).\n");
        exit(1);
    }

    for (int i = 0; i < *num_artistas; i++) {
        fprintf(arquivo, "%s\n", (*artistas)[i].nome);
        fprintf(arquivo, "%s\n", (*artistas)[i].genero);
        fprintf(arquivo, "%s\n", (*artistas)[i].local);
        for (int j = 0; j < (*artistas)[i].num_albums; j++) {
            fprintf(arquivo, "%s", (*artistas)[i].albums[j]);
        }
        fprintf(arquivo, "\n\n==========\n");
    }

    fclose(arquivo);

    printf("Artista inserido com sucesso!\n");
}

void removerArtista(char *nomeArtista, struct Artista **artistas, int *num_artistas) {
    int posicao = -1;
    
    for (int i = 0; i < *num_artistas; i++) {
        if (strcmp(nomeArtista, (*artistas)[i].nome) == 0) {
            posicao = i;
            break;
        }
    }
    
    if (posicao == -1) {
        printf("ERRO!! Artista nao encontrado.\n");
        return;
    }
    
    for (int i = 0; i < (*artistas)[posicao].num_albums; i++) {
        free((*artistas)[posicao].albums[i]);
    }
    free((*artistas)[posicao].albums);
    
    for (int i = posicao; i < *num_artistas - 1; i++) {
        (*artistas)[i] = (*artistas)[i + 1];
    }
    
    *artistas = (struct Artista *)realloc(*artistas, (*num_artistas - 1) * sizeof(struct Artista));
    
    (*num_artistas)--;

    FILE *arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\atividade\\artistas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita(verifique se o caminho do arquivo está correto).\n");
        exit(1);
    }

    for (int i = 0; i < *num_artistas; i++) {
        fprintf(arquivo, "%s\n", (*artistas)[i].nome);
        fprintf(arquivo, "%s\n", (*artistas)[i].genero);
        fprintf(arquivo, "%s\n", (*artistas)[i].local);
        for (int j = 0; j < (*artistas)[i].num_albums; j++) {
            fprintf(arquivo, "%s", (*artistas)[i].albums[j]);
        }
        fprintf(arquivo, "==========\n");
    }

    fclose(arquivo);

    printf("Artista removido com sucesso!\n");
}

void buscarArtista(char *nomeArtista, struct Artista *artistas, int num_artistas) {
    
    nomeArtista[strcspn(nomeArtista, "\n")] = '\0';

    int encontrado = 0;
    
    for (int i = 0; i < num_artistas; i++) {
        if (strcmp(nomeArtista, artistas[i].nome) == 0) {
            printf("\nARTISTA ENCONTRADO:\n");
            imprimirArtista(artistas[i]);
            encontrado = 1;
            break; 
        }
    }
    
    if (!encontrado) {
        printf("ERRO!!ARTISTA NAO ENCONTRADO.\n");
    }
}

void editarArtista(char *nomeArtista, struct Artista *artistas, int num_artistas) {
    
    nomeArtista[strcspn(nomeArtista, "\n")] = '\0';

    int encontrado = 0;
    int indiceArtista = -1;

    for (int i = 0; i < num_artistas; i++) {
        if (strcmp(nomeArtista, artistas[i].nome) == 0) {
            indiceArtista = i;
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("ERRO!! ARTISTA NAO ENCONTRADO.\n");
        return;
    }

    printf("Digite os novos detalhes do artista (pode deixar em branco para manter os detalhes existentes):\n");

    char novoNome[100];
    char novoGenero[100];
    char novoLocal[100];

    printf("Novo Nome (ou pressione Enter para manter '%s'): ", artistas[indiceArtista].nome);
    fgets(novoNome, sizeof(novoNome), stdin);
    novoNome[strcspn(novoNome, "\n")] = '\0';

    printf("Novo Genero (ou pressione Enter para manter '%s'): ", artistas[indiceArtista].genero);
    fgets(novoGenero, sizeof(novoGenero), stdin);
    novoGenero[strcspn(novoGenero, "\n")] = '\0';

    printf("Novo Local (ou pressione Enter para manter '%s'): ", artistas[indiceArtista].local);
    fgets(novoLocal, sizeof(novoLocal), stdin);
    novoLocal[strcspn(novoLocal, "\n")] = '\0';

    if (strcmp(novoNome, "") != 0) {
        strcpy(artistas[indiceArtista].nome, novoNome);
    }
    if (strcmp(novoGenero, "") != 0) {
        strcpy(artistas[indiceArtista].genero, novoGenero);
    }
    if (strcmp(novoLocal, "") != 0) {
        strcpy(artistas[indiceArtista].local, novoLocal);
    }

    FILE *arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\atividade\\artistas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita(verifique se o caminho do arquivo está correto).\n");
        exit(1);
    }

    for (int i = 0; i < num_artistas; i++) {
        fprintf(arquivo, "%s\n", artistas[i].nome);
        fprintf(arquivo, "%s\n", artistas[i].genero);
        fprintf(arquivo, "%s\n", artistas[i].local);
        for (int j = 0; j < artistas[i].num_albums; j++) {
            fprintf(arquivo, "%s", artistas[i].albums[j]);
        }
        fprintf(arquivo, "==========\n");
    }

    fclose(arquivo);

    printf("Detalhes do artista atualizados!\n");
}

void buscaralbum(char *nomealbum, struct Artista *artistas, int num_artistas) {
    int encontrado = 0;
    
    for (int i = 0; i < num_artistas; i++) {
        for (int j = 0; j < artistas[i].num_albums; j++) {
            if (strstr(artistas[i].albums[j], nomealbum) != NULL) {
                printf("Álbum encontrado:\n");
                printf("Artista: %s\n", artistas[i].nome);
                printf("Álbum: %s", artistas[i].albums[j]);
                encontrado = 1;
            }
        }
    }
    
    if (!encontrado) {
        printf("Album nao encontrado.\n");
    }
}

int main() {
    
    UINT CPAGE_UTF8 = 65001;
    UINT CPAGE_DEFAULT = GetConsoleOutputCP();

    SetConsoleOutputCP(CPAGE_UTF8);

    FILE *arquivo;
    struct Artista *artistas = NULL;
    int num_artistas = 0;

    arquivo = fopen("C:\\Users\\Yanns\\OneDrive\\Área de Trabalho\\atividade\\artistas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao tentar abrir o arquivo (verifique se o caminho do arquivo está correto).\n");
        exit(1);
    }

    while (!feof(arquivo)) {
        struct Artista artista = lerArtista(arquivo);
        if (strcmp(artista.nome, "FIM") != 0) {
            artistas = (struct Artista *)realloc(artistas, (num_artistas + 1) * sizeof(struct Artista));
            artistas[num_artistas] = artista;
            num_artistas++;
        }
    }

    fclose(arquivo);

    int escolha;
    char nomeArtista[100];
    char nomealbum[100];

    do {
        printf("\nMenu:\n");
        printf("1. Inserir novo artista\n");
        printf("2. Remover artista\n");
        printf("3. Editar artista\n");
        printf("4. Buscar artista\n");
        printf("5. Buscar album\n");
        printf("6. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &escolha);

        while (getchar() != '\n');

        switch (escolha) {
            case 1:
                printf("Digite os detalhes do novo artista (ou digite 'FIM' para parar):\n");
                struct Artista novoArtista;
                printf("Nome: ");
                fgets(novoArtista.nome, sizeof(novoArtista.nome), stdin);
                novoArtista.nome[strcspn(novoArtista.nome, "\n")] = '\0';
                printf("Genero: ");
                fgets(novoArtista.genero, sizeof(novoArtista.genero), stdin);
                novoArtista.genero[strcspn(novoArtista.genero, "\n")] = '\0';
                printf("Local: ");
                fgets(novoArtista.local, sizeof(novoArtista.local), stdin);
                novoArtista.local[strcspn(novoArtista.local, "\n")] = '\0';
                printf("Albuns (insira um album por linha, digite 'FIM' para finalizar):\n");
                novoArtista.num_albums = 0;
                novoArtista.albums = NULL;
                char album[100];
                while (1) {
                    fgets(album, sizeof(album), stdin);
                    album[strcspn(album, "\n")] = '\0';
                    if (strcmp(album, "FIM") == 0) {
                        break;
                    }
                    novoArtista.num_albums++;
                    novoArtista.albums = (char **)realloc(novoArtista.albums, novoArtista.num_albums * sizeof(char *));
                    novoArtista.albums[novoArtista.num_albums - 1] = strdup(album);
                }
                inserirArtista(novoArtista, &artistas, &num_artistas);
                break;

            case 2:
                printf("Insira o nome do artista que será removido: ");
                fgets(nomeArtista, sizeof(nomeArtista), stdin);
                nomeArtista[strcspn(nomeArtista, "\n")] = '\0'; 
                removerArtista(nomeArtista, &artistas, &num_artistas);
                break;

            case 3:
                printf("Insira o nome do artista que será editado: ");
                fgets(nomeArtista, sizeof(nomeArtista), stdin);
                nomeArtista[strcspn(nomeArtista, "\n")] = '\0'; 
                editarArtista(nomeArtista, artistas, num_artistas);
                break;

            case 4:
                printf("Insira o nome do artista que será buscado: ");
                fgets(nomeArtista, sizeof(nomeArtista), stdin);
                nomeArtista[strcspn(nomeArtista, "\n")] = '\0'; 
                buscarArtista(nomeArtista, artistas, num_artistas);
                break;

            case 5:
                printf("Insira o nome do álbum que será buscado: ");
                fgets(nomealbum, sizeof(nomealbum), stdin);
                nomealbum[strcspn(nomealbum, "\n")] = '\0'; 
                buscaralbum(nomealbum, artistas, num_artistas);
                break;

            case 6:
                printf("Finalizando...\n");
                break;

            default:
                printf("Opção errada. Tente novamente.\n");
                break;
        }
    } while (escolha != 6);

    for (int i = 0; i < num_artistas; i++) {
        for (int j = 0; j < artistas[i].num_albums; j++) {
            free(artistas[i].albums[j]);
        }
        free(artistas[i].albums);
    }
    free(artistas);

    SetConsoleOutputCP(CPAGE_DEFAULT);
    return 0;
}