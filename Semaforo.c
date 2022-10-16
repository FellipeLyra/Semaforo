#include <stdlib.h>
#include <stdio.h>//bibliotecas padrão
#include <pthread.h>//biblioteca pra theadres
#include <semaphore.h>//biblioteca pra semáforos
#include <locale.h>//linguagem em português aceitando acentuação
#include <unistd.h>//acesso a ficheiros e diretorios

#define LEITORES 10// constante de 10 leitores
#define ESCRITORES 5//constante de 5 escritores
pthread_mutex_t db;// controlar a região critica, exclusão multipla
pthread_mutex_t mutex;// controlar o acesso ao banco de dados,e o acesso a variavel leitor_lendo
int leitor_lendo;//guarda quantidade de leitores lendo ou querendo ler 

//prototipos de funções
void ler_base_de_dados(void);
void usar_banco_de_dados(void);
void pensando_nos_dados(void);
void escrever_no_banco_de_dados(void);

void leitor(){ //se passa as principais ações dos leitores 
    while(1){
        pthread_mutex_lock(&mutex);// acessso total a variavel leitor_lendo
        leitor_lendo = leitor_lendo + 1;//cria novo leitor

        if(leitor_lendo==1) pthread_mutex_lock(&db);//se for o primeiro leitor é liberado o acesso a variavel leitor
        pthread_mutex_unlock(&mutex);

        ler_base_de_dados();
        pthread_mutex_lock(&mutex);// da acesso exclusivo leitor_lendo
        leitor_lendo = leitor_lendo - 1;//decrementa o leitor quando terminar de ler

        if(leitor_lendo==0) pthread_mutex_unlock(&db);//caso ele seja o ultimo leitor ele libera o acesso a leitor_lendo
        pthread_mutex_unlock(&mutex);

        usar_banco_de_dados();//utiliza as informações da base de dados para algum outro trabalho, pra ação dos escritores
    }
}

void escritor(){
    while(1){
        pensando_nos_dados(); 
        pthread_mutex_lock(&db);//garantem o aceesso exclusivo a base de dados
        escrever_no_banco_de_dados();

        pthread_mutex_unlock(&db);//libera o acesso para outros trabalhos, leitores
    }
}

//representa o tempo que o leitor permanecerá lendo a base de dadose tb para mostrar o total de leitores no determinado momento da execução do código
void ler_base_de_dados(){
    setlocale(LC_ALL, "Portuguese");

    int tempo_de_leitura;//armazena o tempo do leitor lendo
    tempo_de_leitura = rand() % 5;

    printf("Usuário LENDO banco de dados. Total de %d leitores LENDO agora.\n",leitor_lendo);// tem usuarios lendo o banco de dados, e o total de leitores no momento 
    sleep(tempo_de_leitura);//pausa na execução por milisegundos
}

void usar_banco_de_dados(){
    setlocale(LC_ALL, "Portuguese");

    int tempo_de_uso;//armazena o tempo de uso
    tempo_de_uso = rand() % 15;// recebendo até 15 segundos no máximo

    printf("Usuário utilizando conhcimento adquirido no banco de dados.\n");
    sleep(tempo_de_uso);
}

void pensando_nos_dados(){
    setlocale(LC_ALL, "Portuguese");

    int tempo_para_pensar;//armazena um certo tempo na função pensando nos dados
    tempo_para_pensar = rand() % 10;// tempo máximo de 10 segundos

    printf("Excritor PENSANDO no que irá escrever.\n");
    sleep(tempo_para_pensar);
}

void escrever_no_banco_de_dados(){
    setlocale(LC_ALL, "Portuguese");

    int tempo_de_escrita;//armazena tempo da escrita
    tempo_de_escrita = rand() % 10;//tempo maximo de 10 segundos

    printf("Escritor ESCREVENDO no banco de dados.\n");
    sleep(tempo_de_escrita);
}

int main(){
    pthread_t threadsdosescritores[ESCRITORES], threadsdosleitores[LEITORES];// criação de duas thread recebendo os parametros escritores e leitores
    int i;

    pthread_mutex_init(&db,NULL);//inicia o semáforo
    pthread_mutex_init(&mutex,NULL);
    
    for(i=0;i<ESCRITORES;i++){
        pthread_create(&threadsdosescritores[i], NULL, (void *) escritor, NULL);//vai criar 5 escritores 
    }
    //pthread create permite a passagem de um argumento para a thread, para passar mais de um argumento, deve-se utilizar uma struct
    //e passar a mesma como um ponteiro, todos os argumentos devem passar por referencia, e convertido em ponteiro de void

    for(i=0;i<LEITORES;i++){
        pthread_create(&threadsdosleitores[i], NULL, (void *) leitor,NULL);// vai criar os 10 leitores
    }

    for(i=0;i<ESCRITORES;i++){
        pthread_join(threadsdosescritores[i],NULL);//escritores criados são chamados por essa função
    }
    //join serve pra realizar sicronização entre threads

    for(i=0;i<LEITORES;i++){
        pthread_join(threadsdosleitores[i],NULL);
    }
    return(0);
}
