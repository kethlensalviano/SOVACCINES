/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef SOSIGNAL_H_GUARD
#define SOSIGNAL_H_GUARD


/*
* Funcao que apanha o sinal ctrl+c lançado no terminal da main e fecha o SOVACCINES.
* Para fechar realiza o stop_execution com as limpezas necessárias e escreve no ficheiro de stats.
*/
void signal_handler();

/*
* Funcao que apanha o sinal ctrl+c e fecha o processo filho.
* Devolve o contador de operacoes.
*/
void child_handler();

/*
* Funcao que ativa o alarme no intervalo de tempo definido.
*/
void signal_alarm();

/*
* Funcao que imprime para o ecra o estado atual de todas as operacoes quando o alarme eh ativado.
*/
void alarm_handler();

#endif