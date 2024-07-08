#include <ncurses.h>
#include "pipeline.h"

int main() {
    initscr(); // Inicializar ncurses
    curs_set(0); // Desativar visibilidade do cursor
    noecho(); // Desativar eco de caracteres
    cbreak(); // Ativar modo de caractere por caractere

    MemoriaUnica memoria_unica;
    BancoRegistradores banco_registradores;
    PC pc;
    descritor *desc;

    // Inicializar estruturas
    inicializarBancoRegistradores(&banco_registradores);
    inicializarPC(&pc);
    inicializePilha(desc); // Corrigido para inicializePilha

    // Desenhar o menu
    mvprintw(1, 1, "MINI-MIPS 8 BITS - UNIPAMPA");
    mvprintw(2, 1, "Selecione uma opção:");
    mvprintw(3, 1, "1. Carregar memoriaUnica");
    mvprintw(4, 1, "2. Imprimir memoriaUnica");
    mvprintw(5, 1, "3. Imprimir registradores");
    mvprintw(6, 1, "4. Imprimir todo o simulador");
    mvprintw(7, 1, "5. Decodificar instrucao atual");
    mvprintw(8, 1, "6. Salvar .asm");
    mvprintw(9, 1, "7. Salvar .data");
    mvprintw(10, 1, "8. Executa Programa (run)");
    mvprintw(11, 1, "9. Executa uma instrução (step)");
    mvprintw(12, 1, "10. Executa uma instrução anterior");

    refresh(); // Atualizar a tela

    // Tratar entrada do usuário
    int choice;
    choice = getch();
    switch (choice) {
        case '1':
            // Tratar opção 1: Carregar memoriaUnica
            WINDOW *carregar_window = newwin(10, 30, 5, 5);
            box(carregar_window, 0, 0);
            mvwprintw(carregar_window, 1, 1, "Carregar memoriaUnica");
            mvwprintw(carregar_window, 2, 1, "Insira o nome do arquivo:");
            wrefresh(carregar_window);
            char arquivo[256]; // Alocar memória para a string
            wgetnstr(carregar_window, arquivo, 256); // Ler a string
            carregarMemoriaUnica(arquivo);
            delwin(carregar_window);
            break;
        case '2':
            // Tratar opção 2: Imprimir memoriaUnica
            WINDOW *imprimir_window = newwin(10, 30, 5, 5);
            box(imprimir_window, 0, 0);
            mvwprintw(imprimir_window, 1, 1, "Imprimir memoriaUnica");
            mvwprintw(imprimir_window, 2, 1, "MemoriaUnica:");
            wrefresh(imprimir_window);
            imprimirMemoriaUnica(&memoria_unica);
            wgetch(imprimir_window);
            delwin(imprimir_window);
            break;
        case '3':
            // Tratar opção 3: Imprimir registradores
            mvprintw(13, 1, "Imprimindo registradores...");
            refresh();
            imprimirRegistradores(&banco_registradores);
            break;
        case '4':
            // Tratar opção 4: Imprimir todo o simulador
            mvprintw(13, 1, "Imprimindo todo o simulador...");
            refresh();
            // Adicione o código para imprimir todo o simulador aqui
            break;
        case '5':
            // Tratar opção 5: Decodificar instrucao atual
            mvprintw(13, 1, "Decodificando instrucao atual...");
            refresh();
            codificarInstrucao((char *)memoria_unica.instrucoes[pc.endereco_atual]);
            break;
        case '6':
            // Tratar opção 6: Salvar .asm
            mvprintw(13, 1, "Salvando .asm...");
            refresh();
            salvar_asm();
            break;
        case '7':
            // Tratar opção 7: Salvar .data
            mvprintw(13, 1, "Salvando .data...");
            refresh();
            salvar_data();
            break;
        case '8':
            // Tratar opção 8: Executa Programa (run)
            mvprintw(13, 1, "Executando programa...");
            refresh();
                while (pc.endereco_atual < TAM_MEMORIA) {
                    executarCicloInstrucao(&pc, &banco_registradores, &memoria_unica);
                }
                imprimirMemoriaUnica(&memoria_unica);
                imprimirRegistradores(&banco_registradores);
            break;
        case '9':
            // Tratar opção 9: Executa uma instrução (step)
            mvprintw(13, 1, "Executando uma instrução...");
            refresh();
	    save_backup(&pc, desc, &banco_registradores, &memoria_unica);
            executarCicloInstrucao(&pc, &banco_registradores, &memoria_unica);
            break;
        case 10:
            // Tratar opção 10: Executa uma instrução anterior
            mvprintw(13, 1, "Executando uma instrução anterior...");
            refresh();
            undo(&pc, desc, &banco_registradores, &memoria_unica);
            break;
        default:
            mvprintw(13, 1, "Opção inválida. Tente novamente.");
            refresh();
            break;
    }

    // Criar uma janela para a animação
    WINDOW *animacao_window = newwin(20, 40, 15, 5);
    box(animacao_window, 0, 0);
    mvwprintw(animacao_window, 1, 1, "Pipeline Animation");
    wrefresh(animacao_window);

    // Simular o pipeline
    int ciclo = 0;
    while (ciclo < 10) {
        // Desenhar o diagrama do pipeline
        mvwprintw(animacao_window, 3, 1, "Ciclo %d:", ciclo);

        // Converter o ponteiro de inteiro para uma string
        char instrucao_str[256];
        sprintf(instrucao_str, "%d", *(memoria_unica.instrucoes[pc.endereco_atual])); // Desreferenciar o ponteiro

        mvwprintw(animacao_window, 4, 1, "Fetch: %s", instrucao_str);
        mvwprintw(animacao_window, 5, 1, "Decode: %s", instrucao_str);
        mvwprintw(animacao_window, 6, 1, "Execute: %s", instrucao_str);
        mvwprintw(animacao_window, 7, 1, "Memory: %s", instrucao_str);
        mvwprintw(animacao_window, 8, 1, "Write Back: %s", instrucao_str);
        wrefresh(animacao_window);

        printPC(pc.endereco_atual);
        printRegInst();
        printDataRegisters();
        printRegsAB();
        printULA();
        printDataMemoria();
        printMulticiclo();
        // Incrementar o ciclo
        ciclo++;
        napms(500); // Pausar por 500ms
    }

    // Salvar backup
    save_backup(&pc, desc, &banco_registradores, &memoria_unica); // Desreferenciar o ponteiro desc

    delwin(animacao_window);
    endwin(); // Limpar a tela e restaurar o modo de texto
    return 0;
}




