Kethlen Salviano 51835
Maria Silva 40140
Tiago Badalo 55311

FASE 1:
Ignorámos os casos em que o buffer_size é igual a 1, como recomendado pelos professores.
Implementámos o makefile de forma a não compilar se estiver atualizado, com a mensagem correspondente.
Não encontrámos limitações na nossa implementação.

FASE 2:
Fizemos as correções necessárias da 1ª fase.
O makefile agora elimina os ficheiros de log e stats usando make clean.
Dá para fazer ctrl+c directamente na main, desligando todos os processos. Ou então, usando
kill -2 nome_processo para parar a execução de um directamente.
O ficheiro de log e stats são criados se não existirem, se existirem acrescenta. Cada execução da
main fica separada por uma linha: -------------------.
No ficheiro de log também aparece quando o utilizador faz ctrl+c durante a execução.
Não encontrámos limitações na nossa implementação.