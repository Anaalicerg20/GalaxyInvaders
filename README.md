# GalaxyInvaders
Uma paródia do jogo space invaders, inpirada no filme do Guardiões da Galáxia 

Introdução

O trabalho prático de Programação e desenvolvimento de software 1, teve como objetivo o desenvolvimento do jogo, “space invaders”, na linguagem C, com o auxílio da biblioteca ALLEGRO. 
No jogo original, há vários aliens que tentam invadir a Terra. Nesse contexto, o objetivo do jogo é conseguir matar todos esses aliens, impedindo-os de atingir a terra ou aproximar-se muito da nave. 
Nessa minha paródia do space invaders, o alien está representado pelo Thanos e as naves pelo Rocket e pelo Groot, todos esses personagens do Guardiões da Galáxia, filme, no qual eu baseei o meu jogo, visto que pela imagem de fundo é perceptível a tentativa do Thanos(vilão) invadir a galáxia protegida por Rocket e Groot, tudo isso ao som da música de abertura do filme.
Nesse sentido, os jogadores controlam duas naves (Rocket e Groot). O jogador 1, controla o Rocket(nave1) e o movimenta para direita e esquerda, ao clicar, respectivamente, nas teclas D e A, e atira ao clicar na tecla de espaço. Enquanto o jogador 2, controla o Groot (nave 2) e o movimenta para direita com a seta pra direita, para a esquerda com a seta pra esquerda e atira ao clicar no enter. 
Quando o míssil disparado pelas naves atinge um dos aliens(Thanos) ele morre. O jogo acaba ou quando os aliens chegam na galáxia (representada pelo clarão na imagem), ou quando eles se aproximam muito de uma das nave, ou quando todos os aliens morrem. Nesse último caso, o jogador que mais atingiu aliens vence a partida, ou os dois vencem, no caso de empate. 
	A pontuação é acrescida 1 a cada alien morto e o recorde atualizado de acordo com o maior número de aliens atingido em todas as rodadas. 
	Obs: para compilar o jogo basta digitar .\make e para rodar .\tp


Implementação


Inicializando o jogo e declarando funções úteis

Linha 1 - 45: inclusão das bibliotecas e constantes globais 

Linha 51 - 68: estrutura das naves(posição, velocidade, orientação), dos aliens(posição e velocidade) e do tiro(posição, velocidade, atividade).

//funções de inicialização das estruturas do jogo
Linha 75 - 85: inicializando as naves: local na tela, estado=parada, o quanto ela se movimenta a cada clique. 
Linha 88-102 : inicializando o alien: criando uma matriz 4x5(representa todos os aliens), indicando o local no espaço deles, a forma como eles se locomovem: 1 px na horizontal e a altura de um alien na vertical.
Linha 104- 110: inicializando o tiro, atribuindo valor para sua posição original, a velocidade do tiro e o marca como ativo para que ele seja desenhado e atualizado no looping.

Linha 114 - 122: função que carrega o recorde do jogo, ou seja, abre e lê o arquivo que contém o maior recorde de todas as partidas já jogadas. 
Linha 124 - 129: função que salva o recorde: permite a alteração do recorde caso a pontuação da partida atual seja maior. 

//funções que printam as imagens e texto na tela 
Linha 135 - 143: desenhando o cenário do jogo: determinando a imagem de fundo, printando as pontuações e o recorde na tela.
Linha 145 - 149: desenhando a nave: determinando a posição das naves em relação a altura da “grama”(galáxia), e posicionando as imagens do Rocket e do Groot
Linha 151 - 159: desenhando o alien: carrega uma iamgem do Thanos para cada um dos aliens da matriz
Linha 161 - 165: desenhando o tiro em formato de círculo, com raio igual a 5 e cor branca;

//funções de update
Linha 181- 178: atualizando a posição da nave: movendo a nave para esquerda ou direita quando um desses parâmetros é 1 e limitando tal movimento ao limite da tela. 
Linha 175 - 214: atualizando a posição dos aliens: permitindo o deslocamento horizontal dos aliens e  invertendo o movimento de todos quando um bate no final da tela.
Linha  216 - 223: ativando o tiro, caso tecle espaço ou enter, fazendo com que ele suba até o topo da tela e o desative.

//funções de colisão
Linha 229 - 243: vendo se algum dos aliens colidiu com a galaxia.
Linha 245- 275: vendo se algum dos aliens colidiu com as naves ou chegou muito próximo delas(se aproxima a nave a um retângulo)
Linha 277 - 313: vendo se o tiro acertou algum dos aliens(se a posição do tiro está dentro dos limites do retângulo dos aliens): se sim, o tiro é desativado, o alien “deslocado” para fora da tela(torna sua posição negativa) e a pontuação do jogador que atirou printada na tela é acrescido 1. Além disso, é checado se a pontuação atual está maior que o recorde, caso sim, o valor do recorde é atualizado. 

Linha 315 - 328: vê se a posição de todos os aliens está negativa, o que significa que não há mais aliens no jogo(todos foram mortos).

Função main:

Linha 334 - 437 : inicialização do jogo usando ALLEGRO 
criação dos ponteiros para tela, fila de eventos e relógio
inicializando o allegro, criando a tela (altura e largura), iniciando as primitivas(formas como retângulo, triângulo), iniciando as fontes básicas.
criando um temporizador que controla o ritmo do jogo.
fazendo uma fila de eventos que conecta:mouse, tecla, timer.
instalando suporte de mouse e teclado.
inicializa o recorde: chama a função de carregar recorde.
adiciona imagens e inicia a música

Linha 442 - 449: registra os eventos da tela, teclado, mouse e tempo na fila de eventos.

Linha : declara as variáveis naves alien, tiro, timer e música, utilizando das structs formadas e inicializa as variáveis com as informações presentes nas funções initNave e initAlien, além de atribuir o valor inicial 0 para a atividade do tiro, ou seja, iniciando inativo. 

Linha 476 - : determina a variável playing = 1, e realiza o algoritmo referente ao funcionamento do jogo, enquanto essa variável ainda for 1, ou seja, enquanto o jogo ainda não acabou.

	Se o temporizador começou:
Linha 483 -  500:, chama todas as funções de desenho e printa o cenários, as naves, os aliens, a pontuação e o recorde na tela. Além disso, chama as funções de update nave, alien e tiro, para permitir que se altere as posições dos aliens, naves e a atividade dos tiros. Chama a função que verifica se um alien foi atingido.
Linha 503- 508 : checa se o jogo ainda está acontecendo, ou seja, confere se o alien colidiu com o solo ou com as naves ou se todos os aliens foram destruídos (chamando as funções responsáveis por isso) , caso isso seja verdade, ou seja, caso tais funções retorne 1, é necessário igualar a variável playing a 0, para parar a execução do jogo.
	Linha 511 - 516: atualiza a tela caso necessário e printa o tempo passando no     terminal.
	

Se fechar a tela
Linha 520 : jogo acabou: playing = 0;

Se pressionar uma tela
Linha 526 : mostra a tela pressionada no terminal
Linha 528 - 557: vê se a tecla pressionada foi A, D,espaço, seta para direita, para esquerda e/ou enter: atribui 1 a atividade dessa tecla, permitindo a locomoção das naves para esquerda, direita ou o disparo do tiro.
Linha 560 - 581: vê se a tecla foi liberada, caso sim, retorna a atividade da nave ou do tiro para 0.


Linha 583 - 601 : se o alien colidiu com o solo ou com a nave: faz um fundo preto na tela, e printa no centro um “GAME OVER”, as pontuações e o recorde dos jogadores por 3 segundos.
Linha 602 - 626 : se todos os aliens foram destruídos, faz um fundo preto e printa no centro tela o Jogador que venceu a partida, as pontuações e recorde do jogo.


Linha 630 - 642: finalizações: salva o recorde, destroi a tela, a fila de eventos, a fonte, as imagens e a musica que haviam sido iniciados. Retorna 0.

