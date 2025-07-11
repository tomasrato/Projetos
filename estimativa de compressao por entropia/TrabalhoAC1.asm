.data
PEDIR_NOME: 		.string "Introduza o nome de um ficheiro: "	#String para pedir o nome do ficheiro
MENSAGEM_ERRO1:		.string "Nao foi possivel abrir o ficheiro.\n"	#String de erro ao abrir o ficheiro
MENSAGEM_ERRO2:		.string "Ficheiro vazio.\n"			#String de erro de ficheiro vazio
APRESENTAR_TOTAL:	.string "O ficheiro tem "			#String 1 para apresentar o resultado
BYTES:			.string " bytes.\n"				#String que apresenta a unidade do numero apresentado.
APRESENTAR_ENTROPIA:	.string "Pode ser comprimido para "		#String 2 para apresentar resultado
FICHEIRO: 		.space 256					#Reserva o espaco para a string do nome do ficheiro
LER:			.space 1					#Reserva o espaco para guardar o caracter lido
			.align 2					#Alinhamos os bytes para nao resultar em erro de leitura das frequencias
FREQA_CARACTERES:	.space 512					#Reserva o espaco para guardar as frequencias dos caracteres lidos

.text
.globl main
#############################################################################
# Funcao: main
# Descricao:
#  Funcao principal do programa. Pede ao utilizador o nome de um ficheiro,
#  le o seu conteudo byte a byte, calcula a frequencia de cada caractere,
#  estima a entropia do conteudo e imprime o total de bytes e o valor
#  estimado de compressao sem perda.
# Argumentos:
#  Nenhum (interacao por input/output)
# Retorna:
#  Nao retorna – termina com syscall 10
# Side effects:
#  Imprime no ecra, le input do utilizador, le ficheiro
#############################################################################
main:
	li a7, 4							#syscall 4 para fazer a impressao de uma string (PrintString)
	la a0, PEDIR_NOME						#carrega o endereco da string em a0 para fazer a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 8							#syscall 8 para ler uma string (ReadString)
	la a0, FICHEIRO							#destino da string lida
	li a1, 256							#tamanho maximo da string
	ecall								#chama o sistema para executar a instrucao
	
	jal limpar_nova_linha						#ao ler a string com o ReadString, le ate o pressionar enter ("\n") o que gera
									#um erro ao abrir o ficheiro, por isso, precisamos retirar essa "newline"
	
	jal ler_caracteres						#lemos o total de caracteres usando a funcao ler_caracter
	
	mv s0, a0							#guardamos o valor dos caracteres contados em s0
	
	bltz s0, erro_abrir_ficheiro					#se houve algum erro a abrir o ficheir, s0 = -1 entao o programa termina
	
	beqz s0, erro_ficheiro_vazio 					#se o ficheiro esta vazio entao, s0 = 0, logo o programa termina
	
	li a7, 4							#syscall 4 para imprimir uma string (PrintString)
	la a0, APRESENTAR_TOTAL						#carrega o endereco da string em a0 para fazer a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 1							#syscall 1 para imprimir um inteiro (PrintInt)
	mv a0, s0							#carrega o inteiro em a0 para fazer a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 4							#syscall 4 para imprimir uma string (PrintString)
	la a0, BYTES							#carrega o endereco da string para a impressao
	ecall								#chama o sistema para executar a instrucao
	
	jal calcular_entropia						#calcular o valor estimado da compressao do ficheiro
	mv t0, a0							#guardamos o valor estimado
	
	li a7, 4							#syscall 4 para imprimir uuam string (PrintString)
	la a0, APRESENTAR_ENTROPIA					#carrega o endereco da string para a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 1							#syscall 1 para imprimir um inteiro (PrintInt)
	
	beqz t0, arredondar						#se a entropia der 0, tem de se adicionar 1 pois e necessario pelo menos 1 byte
	andi t1, t0, 7							#fazemos um andi com 111 (7 em binario) para verificar se e necessario arrendondamento
	bnez t1, arredondar 						#se der 0, significa que e necessario arredondar
	
	j imprimir							#caso contrario vai imprimir
	
arredondar:
		addi t0, t0, 8						#arredondamos, adicionando 8
	
imprimir:
		srli a0, t0, 3						#carregamos o valor guardado da estimativa em bytes (dividir por 8)
		ecall							#chama o sistema para executar a instrucao
	
	li a7, 4							#syscall 4 para imprimir uma string (PrintString)
	la a0, BYTES							#carrega o endereco da string para a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 10							#syscall 10 para terminar o programa (Exit)
	ecall								#chama o sistema para executar a instrucao

				#########
				#FUNCOES#
				#########

############################################################################
# Funcao: limpar_nova_linha
# Descricao:
#  Esta funcao remove o caractere de nova linha ('\n', ASCII 10)
#  da string lida do utilizador, substituindo-o por '\0'.
#  É usada apos o ReadString, que inclui o '\n' no final da string.
# Argumentos:
#  A string encontra-se armazenada a partir do endereco FICHEIRO
# Retorna:
#  Nenhum valor retornado diretamente (efeito colateral: modifica a string)
############################################################################
limpar_nova_linha:
	la t0, FICHEIRO							#t0 aponta para o inicio da string
	li t1, 10							#ASCII do newline ("\n")

verificar_enter:
		lb t2, 0(t0)						#carregamos o primeiro caracter da string
		beqz t2, fim_verificar_enter				#se for o caracter nulo ("\0")
		addi t0, t0, 1						#avanca a posicao do ponteiro
		bne t2, t1, verificar_enter				#se nao for o newline, volta ao loop
	
fim_verificar_enter:
	sb zero, -1(t0)							#substituimos o \n por \0 que esta na posicao t0-1 pois a gente avancou uma posicao desde que o encontramos
									
	ret								#retornamos a onde estavamos
	

#######################################################################################
# Funcao: ler_caracteres
# Descricao:
#  Abre o ficheiro indicado pelo utilizador, le os caracteres um a um
#  e regista a frequencia absoluta de cada um num array.
#  Apenas sao contabilizados os caracteres pertencentes a tabela ASCII (0 a 127).
# Argumentos:
#  A string com o nome do ficheiro encontra-se no endereco FICHEIRO
# Efeitos:
#  Atualiza o array FREQA_CARACTERES com as frequencias absolutas de cada caractere.
# Retorna:
#  a0 – numero total de caracteres lidos (validos), ou -1 se falha ao abrir o ficheiro
#######################################################################################
ler_caracteres:
	li t0, -1							#criamos a variavel que vai contar os caracteres lidos
	li t2, 128							#variavel que vai controlar os valores para apenas permitir valores da tabela ASCII
	la t3, FREQA_CARACTERES						#coloca o endereco da base do array em t3
	
	li a7, 1024							#syscall 1024 para abrir um ficheiro (Open)
	la a0, FICHEIRO							#abrir o ficheiro com o nome indicado na string
	li a1, 0							#abrir em modo leitura
	ecall								#chama o sistema para executar a instrucao
	
	bltz a0, fim_loop_ler						#se o ficheiro nao conseguiu ser aberto vai para o fim e retorna -1
	
	mv s0, a0							#guarda o file descriptor que permite fazer a leitura e fechar o ficheiro
	
loop_ler:	
		addi t0, t0, 1						#adiciona 1 caracter lido ao total
		li a7, 63						#syscall 63 para ler num ficheiro (Read)
		la a1, LER						#endereco onde guardar
		mv a0, s0						#carrega o file descriptor 
		li a2, 1						#permite ler 1 byte a partir do "cursor"
		ecall							#chama o sistema para executar a instrucao
		beqz a0, fim_loop_ler					#verifica se algum caracter foi lido
		
		lbu t1, 0(a1)						#carregamos o caracter lido
		bge t1, t2, loop_ler					#ignora se houver um byte superior a 128 (nao pertence a tabela ASCII)
		
		slli t1, t1, 2						#multiplica-se o indice por 4
		add t4, t3, t1						#coloca t4 no endereco do indice correspondente
		lw t5, 0(t4)						#le a contagem atual ja realizada
		addi t5, t5, 1						#adiciona 1 a contagem ja feita
		sw t5, 0(t4)						#guarda a contagem atual no array
	
		j loop_ler						#volta a repetir o processo para o proximo caracter
				
fim_loop_ler:	
	li a7, 57							#syscall 57 para fechar um ficheiro (Close)
	mv a0, s0							#carrega o file descriptor em a0
	ecall								#chama o sistema para executar a instrucao
	
	mv a0, t0							#carrega o total de caracteres lidos para a0 para poder ser retornado
	
	ret								#retorna a0 com o numero total de caracteres lidos


#########################################################################################
# Funcao: log_base2
# Descricao:
#  Calcula o logaritmo inteiro de base 2 de um numero positivo.
#  Corresponde ao numero de vezes que o valor pode ser dividido por 2 ate ser igual a 1.
# Argumentos:
#  a0 – numero inteiro positivo do qual se pretende calcular log2(n)
# Retorna:
#  a0 – logaritmo inteiro de base 2 de n
#########################################################################################
log_base2:
	li a2, 0							#variavel que vai contar quantas vezes o numero e divisivel por 2 enquanto >1
	li a3, 1							#variavel usada para comparar com a0							
	beq a3, a0, retornar_log					#se ja for 1, retorna 0
	
calcular_log:
		srli a0, a0, 1						#divide a0 por 2
		addi a2, a2, 1						#adiciona 1 ao contador
		bne a3, a0, calcular_log				#quando a0=1, retorna a
	
retornar_log:
	mv a0, a2							#copia a contagem para a0
	ret								#retorna a contagem que e o log do numero na base 2 arredondado


#########################################################################################
# Funcao: calcular_entropia
# Descricao:
#  Calcula a entropia aproximada de um ficheiro, com base nas
#  frequencias absolutas dos caracteres lidos.
#  Utiliza a formula: H(X) ? ? [p(x) * log2(total/p(x))] com p(x) = ocorrencias[x]
#  Implementa multiplicacoes como somas sucessivas para evitar usar instrucoes mul.
# Argumentos:
#  s0 – total de caracteres lidos (total)
#  Array FREQA_CARACTERES contem as frequencias absolutas dos caracteres ASCII
# Retorna:
#  a0 – valor aproximado da entropia total (em bits)
# Registos usados:
#  s1 – entropia acumulada
#  t0 – indice (0 a 127)
#  t1 – base do array
#  t2 – limite (128)
#  t3 – log2(total)
#  t4 – endereco do array no indice atual
#  t5 – frequencia absoluta no indice atual
#  t6 – log2(total/p(x)) = log2(total) - log2(p(x))
# Side effects:
#  Preserva ra atraves da stack para chamadas a funcoes auxiliares
#########################################################################################
calcular_entropia:
    	li s1, 0                					#valor da entropia acumulada
    	li t0, 0                					#valor do indice acumulado 
    	la t1, FREQA_CARACTERES       					#aponta para o inicio do array que guarda as frequencias absolutas
    	li t2, 128              					#valor limite do loop
	
	addi sp, sp, -8							#decrementamos do stackpointer para guardar o ra na stack
	sw s1, 0(sp)							#guardamos s1 pois funcoes utilizadas dentro desta funcao utilizam esse registo 
	sw ra, 4(sp)							#guardamos o endereco do return da funcao calcular_entropia
	
   	mv a0, s0							#copia o valor do total de caracteres para a0
    	jal log_base2							#calcula o logaritmo de base 2 desse valor
   	mv t3, a0							#coloca o resultado em t3
	
ver_entropia_indice:
    	bge t0, t2, entropia_fim					#inicio do loop para nao ultrapassar o tamanho do array
    	slli t4, t0, 2							#multiplicamos o indice por 4
    	add t4, t4, t1          					#determina o endereco do indice do array e guarda em t4
    	
    	lw t5, 0(t4)           						#guarda em t5, o valor nessa posicao do array					

   	beqz t5, proximo_indice						#se o valor for zero, ignora e faz o proximo indice

    	mv a0, t5							#copia para a0 o valor nessa posicao do array
    	
    	jal log_base2							#calcula o logaritmo de base 2 desse valor
    	mv t6, a0               					#copia esse resultado para t6

    	sub t6, t3, t6          					#calcula o logaritmo de base 2 de total/p(x)
    	
    	mv a0, t6							#decrementamos do stackpointer para guardar o ra na stack
    	mv a1, t5             						#copia o numero de ocorrencias para a1 
    	
    	jal multiplicar_somas  						#calcula a multiplicacao entre o numero de ocorrencias e o ultimo logaritmo calculado

    	add s1, s1, a0							#soma a entropia acumulada

proximo_indice:
    	addi t0, t0, 1							#avanca para o proximo indice
    	blt t0, t2, ver_entropia_indice					#ve se o indice atual nao ultrapassa o limite
    	j entropia_fim							#ve a entropia do proximo indice

entropia_fim:
    mv a0, s1								#coloca a entropia acumulada em a0
    
    lw ra, 4(sp)							#carrega o ra guardado na stack
    lw s1, 0(sp)							#guardamos s1 pois funcoes utilizadas dentro desta funcao utilizam esse registo 
    addi sp, sp, 8							#incrementamos do stackpointer para guardar o ra na stack
    
    ret									#retornamos o valor de a0 a main 
	
	
###############################################################################
# Funcao: erro_abrir_ficheiro
# Descricao:
#  Imprime uma mensagem de erro informando que nao foi possivel
#  abrir o ficheiro indicado e termina imediatamente a execucao do programa.
# Argumentos:
#  Nenhum
# Retorna:
#  Nao retorna – termina o programa com syscall 10
# Side effects:
#  Imprime a mensagem definida em MENSAGEM_ERRO1
###############################################################################
erro_abrir_ficheiro:
	li a7, 4							#syscall 4 para fazer a impressao de uma string (PrintString)
	la a0, MENSAGEM_ERRO1						#carrega o endereco da string em a0 para fazer a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 10							#syscall 10 para terminar o programa (Exit)
	ecall								#chama o sistema para executar a instrucao
	
	
######################################################
# Funcao: erro_ficheiro_vazio
# Descricao:
#  Imprime uma mensagem de erro informando que o ficheiro
#  indicado esta vazio e termina a execucao do programa.
# Argumentos:
#  Nenhum
# Retorna:
#  Nao retorna – termina a execucao do programa com syscall 10
# Side effects:
#  Imprime a string definida em MENSAGEM_ERRO2
######################################################
erro_ficheiro_vazio:
	li a7, 4							#syscall 4 para fazer a impressao de uma string (PrintString)
	la a0, MENSAGEM_ERRO2						#carrega o endereco da string em a0 para fazer a impressao
	ecall								#chama o sistema para executar a instrucao
	
	li a7, 10							#syscall 10 para terminar o programa (Exit)
	ecall								#chama o sistema para executar a instrucao
	
	
#########################################################################
# Funcao: multiplicar_somas
# Descricao:
#  Calcula o produto entre dois inteiros positivos atraves de
#  somas sucessivas (simula a multiplicacao sem usar a instrucao mul).
# Argumentos:
#  a0 – primeiro operando (multiplicador)
#  a1 – segundo operando (multiplicando, numero de vezes a somar)
# Retorna:
#  a0 – resultado da multiplicacao: a0 × a1
# Side effects:
#  Nenhum (registos temporarios a5, a6 usados internamente)
#########################################################################
multiplicar_somas:
    li a5, 0            						#acumula o produto
    mv a6, a1           						#conta quantas vezes e multiplicado

    beqz a6, fim_multiplicar						#se for suposto multiplicar 0 vezes, retorna 0

multiplicar_loop:
    	add a5, a5, a0      						#soma, ao acumulador do produto, a1
    	addi a6, a6, -1							#diminui a quantidade de vezes que ainda e preciso fazer a soma
    	bnez a6, multiplicar_loop					#refaz se ainda faltar repetir

fim_multiplicar:
    mv a0, a5								#copia o acumulador para a0
    ret									#retorna o acumulador
