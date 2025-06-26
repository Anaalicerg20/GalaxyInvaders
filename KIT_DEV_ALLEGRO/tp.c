#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//constantes globais

//tamanho da tela
const int SCREEN_W = 960; 
const int SCREEN_H = 600; 

//altura da galáxia (grama)
const int GRASS_H = 80;

//medidas da nave
const int NAVE_W = 60; //largura
const int NAVE_H = 30; //altura

//medidas alien
const int ALIEN_W = 40;
const int ALIEN_H = 45;

//matriz de aliens
const int NUM_LINHAS = 4;
const int NUM_COLUNAS = 5;
const int ESPACO = 30;

//quantidade de vezes por segundo que o jogo atualiza a tela
const float FPS = 250; 

//letra: pontucao e recorde
ALLEGRO_FONT *font = NULL;
int pontuacao1 = 0, pontuacao2 = 0;
int recorde = 0;

//inicializando um ponteiro para uma imagem
ALLEGRO_BITMAP *img_fundo = NULL;
ALLEGRO_BITMAP *img_nave1 = NULL;
ALLEGRO_BITMAP *img_nave2 = NULL;
ALLEGRO_BITMAP *img_alien = NULL;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

//Criando

//estrutura nave
typedef struct Nave{
	float x; //posição horizontal
	float vel;
	int dir, esq; //indica se a nave se movimenta pra direita ou esquerda (0,1)
} Nave;

//estrutura alien
typedef struct Alien{
	float x, y; //posição horizontal e vertical na tela
	float x_vel, y_vel; //velocidade horizontal e vertical
} Alien;

//estrutura tiro 
typedef struct Tiro {
    float x, y;
    float vel;
    int ativo; //tiro esta visivel, movimentando?
} Tiro;

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

//inicializar

//criar nave
void initNave(Nave *nave, int jogador){
	if (jogador == 1) {
        nave->x = SCREEN_W / 4;  // Nave 1 começa à esquerda
    } else {
        nave->x = 3 * SCREEN_W / 4;  // Nave 2 começa à direita
    }
	nave-> vel = 3; //nave move 3 pixels de cada vez
	//inicializa a nave parada 
	nave -> dir = 0; 
	nave -> esq = 0;

}

//criar a matriz de aliens 4 x 5
void initAlien(Alien alien[NUM_LINHAS][NUM_COLUNAS]){
	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){

			alien[i][j].x = j * (ALIEN_W + ESPACO); //um ao lado do outro
			alien[i][j].y = i * (ALIEN_H + ESPACO); //um embaixo do outro

			alien[i][j].x_vel = 1; //move 1 px por seg
	 		alien[i][j].y_vel = ALIEN_H; //move a altura de um alien para baixo
		}
	}
}

//iniciar tiro no momento do disparo 
void init_Tiro(Tiro *tiro, float x, float y){
	tiro->x = x;
	tiro->y = y;
    tiro->vel = 4.0;
    tiro->ativo = 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

//funções para recorde 
int carregar_recorde(const char *nome){
	FILE *arq = fopen(nome, "r");

	int recorde = 0;
	fscanf(arq, "%d", &recorde);
	fclose(arq);

	return recorde; 
}

void salvar_recorde(const char *nome, int recorde){
	FILE *arq = fopen(nome, "w");

	fprintf(arq, "%d\n", recorde);
	fclose(arq);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

//desenhar

void draw_scenario(int pontuacao1, int pontuacao2, int recorde){

	al_draw_bitmap(img_fundo, 0, 0, 40);

	//desenha a pontuação
	char texto[100];
	 snprintf(texto, sizeof(texto), "Jogador 1: %d    Jogador 2: %d    Recorde: %d", pontuacao1, pontuacao2, recorde);
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, texto);
}

void draw_nave(Nave nave, int jogador){
	float y_base = SCREEN_H - 1.5*GRASS_H;
	ALLEGRO_BITMAP *img = (jogador == 1) ? img_nave1 : img_nave2; // se for o jogador1 usa a img_nave1, senão usa img_nav2
	al_draw_bitmap(img, nave.x - NAVE_W/2, y_base, 0); //posiçaõ da nave
}

void draw_alien(Alien alien[NUM_LINHAS][NUM_COLUNAS]){
	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){
			al_draw_bitmap(img_alien, alien[i][j].x, alien[i][j].y, 0);
		}
	}
}

void draw_tiro(Tiro tiro) {
    if (tiro.ativo) {
        al_draw_filled_circle(tiro.x, tiro.y, 5, al_map_rgb(255, 255, 255)); 
    }
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

//atualizar 

void update_nave(Nave *nave){
	//nave se move dentro do limite da tela 
	if(nave-> dir && nave->x + nave->vel <= SCREEN_W){
		nave-> x += nave->vel;
	}
	if(nave->esq && nave->x - nave->vel >= 0){
		nave->x -= nave->vel;
	}
}

void update_alien(Alien alien[NUM_LINHAS][NUM_COLUNAS]){
	int i;
	int inverte = 0;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){
			if (alien[i][j].x < 0 && alien[i][j].y < 0) continue; // Ignora destruídos

			//se um alien chegar no limite da tela
			if(alien[i][j].x + ALIEN_W + alien[i][j].x_vel > SCREEN_W || alien[i][j].x + alien[i][j].x_vel < 0){
				inverte = 1;
				break;
			}
		if(inverte == 1)
			break;
		}
	}

	//inverta a direção de todos 
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){

			if (alien[i][j].x < 0 && alien[i][j].y < 0) continue;//pula os aliens ja destruios

			if(inverte == 1){
				alien[i][j].y += alien[i][j].y_vel;
				alien[i][j].x_vel *= -1;
				}
			alien[i][j].x += alien[i][j].x_vel;
		}
	}

}

void update_tiro(Tiro *tiro){
	if(tiro->ativo){ // se o tiro foi dispardo
		tiro->y -= tiro->vel; //tiro sobe na tela
		 if (tiro->y < 0){ //ao chegar no topo da tela 
			tiro->ativo = 0; //desativa o tiro
		 }
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//colisões

//se um alien colide, o jogo acaba
int colisao_alien_solo(Alien alien[NUM_LINHAS][NUM_COLUNAS]){
	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){

			if (alien[i][j].x < 0 && alien[i][j].y < 0) continue;
			//parte inferior do alien > parte superior da grama (colidiu)
			if(alien[i][j].y + ALIEN_H > SCREEN_H - GRASS_H){
				return 1;	
			}
		}
	}
	return 0;
}

//colisao de um alien com a nave
int colisao_alien_nave(Alien alien[NUM_LINHAS][NUM_COLUNAS], Nave nave){
	float y_base = SCREEN_H - GRASS_H; //usado para desenhar a nave
		
	//limites da nave: aproximando ela de um retangulo
	float nave_left = nave.x - NAVE_W/2;
	float nave_right = nave.x + NAVE_W/2;
	float nave_top = y_base - NAVE_H;
	float nave_bottom = y_base;


	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){

			if (alien[i][j].x < 0 && alien[i][j].y < 0) continue;
			//limites do alien
			float alien_left = alien[i][j].x;
			float alien_right = alien[i][j].x + ALIEN_W;
    		float alien_top = alien[i][j].y;
    		float alien_bottom = alien[i][j].y + ALIEN_H;
		
			// Verifica interseção entre os retângulos
			if((alien_right >= nave_left && alien_left <= nave_right) && (alien_bottom >= nave_top && alien_top <= nave_bottom)){
				return 1;
			}
		}
	}
	return 0;
}

//verifica colisao do tiro com o alien
void verifica_colisao_tiro_alien(Tiro *tiro, Alien alien[NUM_LINHAS][NUM_COLUNAS], int *pontuacao, int *recorde){
	if(!tiro->ativo) //o tiro deve estar ativo para q a colisao ocorra 
		return; 

	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){

			if (alien[i][j].x < 0 && alien[i][j].y < 0) //alien ja destruido?
                continue;

			//limites do alien
			float alien_left = alien[i][j].x;
			float alien_right = alien[i][j].x + ALIEN_W;
    		float alien_top = alien[i][j].y;
    		float alien_bottom = alien[i][j].y + ALIEN_H;

			//se o tiro estiver dentro dos limites do retangulo
			if(tiro->x >= alien_left && tiro->x <= alien_right && tiro->y >= alien_top && tiro-> y <= alien_bottom){ //obs: y = 0 esta no topo da tela
				tiro->ativo = 0; //desativar o tiro
				//move os aliens pra fora da tela (desaparece)
				alien[i][j].x = -1000;
				alien[i][j].y = -1000;

				(*pontuacao)++;
				if (*pontuacao > *recorde) {
   					 *recorde = *pontuacao;
				}

				return;
			}

		}
	}
}

//verifica se os aliens foram todos destruidos 
int todos_aliens_destruidos(Alien alien[NUM_LINHAS][NUM_COLUNAS]){
	int i;
	for(i = 0; i < NUM_LINHAS; i++){
		int j;
		for(j = 0; j < NUM_COLUNAS; j++){
			if(!(alien[i][j].x < 0 && alien[i][j].y < 0)){ //ainda esta na tela
				return 0;
			}
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(){

    //typedef struct: tela 
    ALLEGRO_DISPLAY *display = NULL;
	//fila de eventos
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	//espaço de tempo
	ALLEGRO_TIMER *timer = NULL;

    //inicializa o Allegro
	if(!al_init()) {  
		fprintf(stderr, "failed to initialize allegro!\n"); //caso não abra o alegro
		return -1; 
	}

	//fakha imagem
	if (!al_init_image_addon()) {
    fprintf(stderr, "failed to initialize image addon!\n");
    return -1;
}

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H); //parametro altura e largura da  tela
	if(!display) {
		fprintf(stderr, "failed to create display!\n"); //erro
		return -1;
	}

	//inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	

	//inicializa a fonte
	al_init_font_addon();

	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS); //em cada segundo o relogio contade 1 ate 100
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//instala a fonte
	font = al_create_builtin_font();
	if (!font) {
    	fprintf(stderr, "failed to load built-in font!\n");
    	return -1;
	}

	//inicializa o recorde e pontuação
	recorde = carregar_recorde("recorde.txt");
	pontuacao1 = 0;
	pontuacao2 = 0;

	//adiciona imagens no fundo, nas naves e nos aliens
	img_fundo = al_load_bitmap("fundo.jpg");
	img_nave1 = al_load_bitmap("nave1.png");
	img_nave2 = al_load_bitmap("nave2.png");
	img_alien = al_load_bitmap("alien.png");

	//inicializando a música
	if (!img_fundo || !img_nave1 || !img_nave2 || !img_alien) {
    	fprintf(stderr, "failed to load one or more images!\n");
    	return -1;
	}
	if (!al_install_audio()) {
    	fprintf(stderr, "failed to initialize audio!\n");
    	return -1;
	}
	if (!al_init_acodec_addon()) {
    	fprintf(stderr, "failed to initialize audio codecs!\n");
    	return -1;
	}
	// Reserve samples (buffer) para tocar áudio
	if (!al_reserve_samples(1)) {
    	fprintf(stderr, "failed to reserve samples!\n");
    	return -1;
	}

	ALLEGRO_SAMPLE *musica = al_load_sample("musica.ogg");
	if (!musica) {
    	fprintf(stderr, "failed to load audio file!\n");
    	return -1;
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  	
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

	Nave nave1, nave2;
	initNave(&nave1, 1);
	initNave(&nave2, 2);


	Alien alien[NUM_LINHAS][NUM_COLUNAS];
	initAlien(alien);

	Tiro tiro1, tiro2;

	tiro1.ativo = 0;
	tiro2.ativo = 0; //inicialmente o tiro esta inativo

	ALLEGRO_SAMPLE_ID musica_id; //armazena identificador da música

	//inicia o temporizador
	al_start_timer(timer);

	//tocando a música(volume, posição, velocidade)
	al_play_sample(musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musica_id);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

	int playing = 1;
	ALLEGRO_EVENT ev;
	while(playing){
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER){
			//desenhando/atualizando

			update_nave(&nave1);
			update_nave(&nave2);
			update_alien(alien);
			update_tiro(&tiro1);
			update_tiro(&tiro2);

			verifica_colisao_tiro_alien(&tiro1, alien, &pontuacao1, &recorde);
			verifica_colisao_tiro_alien(&tiro2, alien, &pontuacao2, &recorde);
			
			draw_scenario(pontuacao1, pontuacao2, recorde);
			draw_tiro(tiro1);
			draw_tiro(tiro2);
			draw_nave(nave1, 1);
			draw_nave(nave2, 2);
			draw_alien(alien);

			//playing inverso do retorno
			//se houve colisão: return 1, playing = 0, fim de jogo
			if(colisao_alien_solo(alien) || colisao_alien_nave(alien, nave1) || colisao_alien_nave(alien, nave2)){
				playing = 0;
			}else if(todos_aliens_destruidos(alien)){
				playing = 0;
			}
			
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();

			//printa no terminal o tempo que está passando
			if(al_get_timer_count(timer) %(int)FPS == 0)
				printf("\n%d segundos se passaram ...", (int)(al_get_timer_count(timer)/FPS));
			
		}

		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}

		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) { 
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_A:
					nave1.esq = 1;
					break;

				case ALLEGRO_KEY_D:
					nave1.dir = 1;
					break;

				case ALLEGRO_KEY_SPACE:
        			if (!tiro1.ativo) {
            			init_Tiro(&tiro1, nave1.x, SCREEN_H - (GRASS_H/2 + NAVE_H));
        			}
       				 break;

				case ALLEGRO_KEY_LEFT:
        			nave2.esq = 1;
        			break;

    			case ALLEGRO_KEY_RIGHT:
        			nave2.dir = 1;
        			break;

				case ALLEGRO_KEY_ENTER:  // Exemplo: jogador 2 atira com Enter
        			if (!tiro2.ativo) {
            			init_Tiro(&tiro2, nave2.x, SCREEN_H - (GRASS_H/2 + NAVE_H));
        			}
        			break;
			}
		}

		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) { 
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_A:
					nave1.esq = 0;
					break;

				case ALLEGRO_KEY_D:
					nave1.dir = 0;
					break;
				 case ALLEGRO_KEY_LEFT:
        			nave2.esq = 0;
        			break;

    			case ALLEGRO_KEY_RIGHT:
        			nave2.dir = 0;
        			break;
				}
		}
	}

	if(colisao_alien_solo(alien) || colisao_alien_nave(alien, nave1) || colisao_alien_nave(alien, nave2)) {
    	// Game Over
    	al_clear_to_color(al_map_rgb(0, 0, 0)); //fundo preto
    	al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "GAME OVER!");

    	char texto_pontuacao1[64];
    	sprintf(texto_pontuacao1, "Pontuacao Jogador 1: %d", pontuacao1);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 - 20, ALLEGRO_ALIGN_CENTER, texto_pontuacao1);
   		char texto_pontuacao2[64];
    	sprintf(texto_pontuacao2, "Pontuacao Jogador 2: %d", pontuacao2);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, texto_pontuacao2);

    	char texto_recorde[64];
    	sprintf(texto_recorde, "Recorde Geral: %d", recorde);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, texto_recorde);

    	al_flip_display();
    	al_rest(3.0); // espera 3 segundos antes de fechar

	} else if(todos_aliens_destruidos(alien)) {
    	// Você venceu
    	al_clear_to_color(al_map_rgb(0, 0, 0));
    
    	if (pontuacao1 > pontuacao2) {
        	al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "JOGADOR 1 VENCEU!");
    	} else if (pontuacao2 > pontuacao1) {
        	al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "JOGADOR 2 VENCEU!");
    	} else {
        	al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_W/2, SCREEN_H/2 - 60, ALLEGRO_ALIGN_CENTER, "EMPATE!");
    	}

    	char texto_pontuacao1[64];
    	sprintf(texto_pontuacao1, "Pontuacao Jogador 1: %d", pontuacao1);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 - 20, ALLEGRO_ALIGN_CENTER, texto_pontuacao1);

    	char texto_pontuacao2[64];
    	sprintf(texto_pontuacao2, "Pontuacao Jogador 2: %d", pontuacao2);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 10, ALLEGRO_ALIGN_CENTER, texto_pontuacao2);

    	char texto_recorde[64];
    	sprintf(texto_recorde, "Recorde Geral: %d", recorde);
    	al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, texto_recorde);

    	al_flip_display();
    	al_rest(3.0); // espera 3 segundos antes de fechar
	}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
	salvar_recorde("recorde.txt", recorde);

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);

	al_destroy_bitmap(img_fundo);
	al_destroy_bitmap(img_nave1);
	al_destroy_bitmap(img_nave2);
	al_destroy_bitmap(img_alien);
	al_destroy_sample(musica);
    return 0;
}
