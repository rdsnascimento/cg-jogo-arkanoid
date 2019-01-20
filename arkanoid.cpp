#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//definicao do tamanho da janela
#define TAMJANELA 680
#define POSJANELA 10

//quantidade de blocos no jogo
#define NUMBLOCOS 24

//struct (um tipo de dados q agrupam variaveis)
struct Bloco{
	float x;
	float y;
	float comp;
	float altura;
	bool ativo;
};
Bloco blocos[NUMBLOCOS]; //vetor de blocos

//variaveis nave
float naveX = 300, naveY = 40, naveComprimento = 80, naveAltura = 30;

//variavel bola
float bolaX = 325, bolaY = 70, bolaCA = 30;

//direcao (velocidade)
float direcaoX = 0.7, direcaoY = 0.7;

//logica do jogo
int pontuacao = 0;
int vidas = 3;
int nivel = 1;
char texto[30]; 
int blocosDestruidos=0;
int cor = 0;

//flags
bool gameover = false;
bool comecar = false;
bool pause = false;

//escopo das funcoes criadas
bool colisao(float bolaX, float bolaY, float bolaComp, float bolaAltura, float objetoX, float objetoY, float objetoComp, float objetoAltura); //verifica se houve colisao
void desenhaBola(void); //desenha a bola
void desenhaBlocos(); //desenha os blocos
void desenhaNave(); //desenha a nave
void escreveTexto(int posx, int posy, char ns[20]);
void escreveGameOver(int posx, int posy, char ns[20]);
void desenhaPlacar();
void desenha(void);
void proximoNivel();
void movimentaBola(int passo);
void iniciaBlocos();

void Inicializa(void){
	glClearColor(0, 0, 0, 0); //cor do fundo 
	glColor3f(1,1,0); 
}


void Mouse(int botao, int estado, int xmouse, int ymouse) {
	if (botao == GLUT_LEFT_BUTTON){ //botao esquerdo muda blocos de cor
		if (estado == GLUT_DOWN){
			cor++;
			if(cor==4){
				cor=0;
			}
		}
	}
	/*
	if (botao == GLUT_RIGHT_BUTTON){
		if (estado == GLUT_DOWN){
			direcaoX -= 0.35;
			direcaoY -= 0.35;
		}
	}*/		
	if (botao == GLUT_RIGHT_BUTTON){ //botao direito pausa o jogo
		if (estado == GLUT_DOWN){
			pause=!pause;
		}
	}
	
	glutPostRedisplay();
}

//inicia os blocos
void iniciaBlocos(){
	
	//o primeiro bloco
	int i=0;
	int x=25; //coordenada x
	int y = 500; //coordenada y
	
	//logica para os demais blocos
	for(i=0; i<NUMBLOCOS; i++){
		
		if(x > 600){ //para nao extrapolar os limites
			x = 25;
			y += 40;
		}
		
		blocos[i].x = x; //onde comeca x
		blocos[i].y = y; //onde comeca y
		blocos[i].comp = 70; //comprimento
		blocos[i].altura = 25; //altura
		blocos[i].ativo = true; //bloco ativo
		
		x+=80;
	}
}

//funcao desenha na tela
void desenha(void){
	glClear(GL_COLOR_BUFFER_BIT);	
	desenhaPlacar();
	desenhaNave();
	desenhaBlocos();
	desenhaBola();
	
	glPopMatrix();
	glutSwapBuffers();
}

//desenha a bola na tela
void desenhaBola(void) {
	glColor3f(0.5,1,0.5); //cor da bola
	
	glBegin(GL_QUADS); //desenha a bola na tela
		glVertex2f(bolaX, bolaY); //ponto 1
		glVertex2f(bolaX+bolaCA, bolaY); //ponto 2
		glVertex2f(bolaX+bolaCA, bolaY+bolaCA); //ponto 3
		glVertex2f(bolaX, bolaY+bolaCA); //ponto 4
	glEnd();
}

//desenha os blocos na tela
void desenhaBlocos(){ 
	if(cor==0){
		glColor3f(0.3,0.4,0.8);
	} else if(cor==1){
		glColor3f(0.7,0.3,0.3);
	} else if(cor==2){
		glColor3f(0.6,0.7,0.4);
	} else if(cor==3){
		glColor3f(0.6,0.8,0.8);
	} 
	
	glBegin(GL_QUADS);
	for(int n=0; n < NUMBLOCOS; n++){
		if(blocos[n].ativo == true){ //se o bloco ativo ele exibe
			glVertex2f(blocos[n].x, blocos[n].y);
			glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y);
			glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].altura);
			glVertex2f(blocos[n].x, blocos[n].y + blocos[n].altura);
		}
	}
	glEnd();
}

//desenha a nave
void desenhaNave(){ 
	glColor3f(0.8, 0, 0);
	//inicia desenho nave
	glBegin(GL_QUADS); //desenha nave
		glVertex2f(naveX, naveY); //ponto 1
		glVertex2f(naveX + naveComprimento, naveY); //ponto 2
		glVertex2f(naveX + naveComprimento, naveY + naveAltura); //ponto 3
		glVertex2f(naveX, naveY + naveAltura); //ponto 4
	glEnd();
}

void desenhaPlacar(){
	//desenha o fundo azul
	glColor3f(0.5, 0.5, 1.0);
	glBegin(GL_POLYGON);
		glVertex2f(680, 0);
		glVertex2f(950, 0);
		glVertex2f(950, 800);
		glVertex2f(680, 800);
	glEnd();
  
	//Game Over
	glColor3f(0.8,0,0);
	char textoTmp[30];
	if(gameover==true){
		strcpy(texto, "GAME OVER");
		escreveGameOver(720, 450, texto);
	}
	
	//Exibe o nivel
	glColor3f(0,0,0);
	strcpy(texto, "NIVEL ");
	sprintf (textoTmp, "%d", nivel);
	strcat(texto,textoTmp);
	escreveTexto(720, 400, texto);
  
	//exibe a pontuacao
	glColor3f(0,0,0);
	strcpy(texto, "PONTUACAO ");
	sprintf (textoTmp, "%d", pontuacao);
	strcat(texto,textoTmp);
	escreveTexto(720, 350, texto);
  
	//exibe a quantidade de vidas
	strcpy(texto, "VIDAS ");
	sprintf (textoTmp, "%d", vidas);
	strcat(texto,textoTmp);
	escreveTexto(720, 300, texto);
}

//escreve os textos
void escreveTexto(int posx, int posy, char ns[20]){ 
	glRasterPos2i (posx, posy);
	char *p= (char*) ns;
	while (*p!= '\0'){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
	}
}

//escreve game over
void escreveGameOver(int posx, int posy, char ns[20]){
	glRasterPos2i (posx, posy);
	char *p= (char*) ns;
	while (*p!= '\0'){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p++);
	}
}

//teclado movimento para esquerda e para direita
void teclado(int tecla, int xmouse, int ymouse){
	switch (tecla) {
		case GLUT_KEY_LEFT: //movimenta para esquerda
			if(naveX > 0){
				if(!(bolaY<70&&bolaY>=10)){ //se a bola nao esta na altura da nave
					naveX-=10;
				} else if((naveX-naveComprimento+15)>(bolaX-bolaCA)){ //se a nave nao estah encostando na bola
					naveX-=10;
				} else {
					naveX-=1;
				}
				
				if(!comecar){ //se o jogo nao comecou entao a bola eh movimentada junto
					bolaX-=10;
				}
			}
			break;
		case GLUT_KEY_RIGHT: //movimenta para direita
			if(naveX < 600){
				if(!(bolaY<70&&bolaY>=10)){ //se a bola nao esta na altura da nave
					naveX+=10;
				} else if ((naveX+naveComprimento+bolaCA+4.5)<=(bolaX+bolaCA)){ //se a nave nao esta encostando na bola
					naveX+=10;
				} else {
					naveX+=1;
				}
				
				if(!comecar){ //se o jogo nao comecou entao a bola eh movimentada junto
					bolaX+=10;
				}
			}
			break;
	}
	glutPostRedisplay();
}

void teclado2(unsigned char tecla, int xmouse, int ymouse){
	switch (tecla) {
		case 27: exit(0); // Tecla [Esc] sair do jogo
		case 'a': //comeca pela esquerda
			if(comecar == false){ //se ainda nao comecou define as variaveis de incremento para esquerda
				direcaoX = -0.7; //velocidade
				direcaoY = 0.7; //velocidade
			}
			if(gameover == true){
				gameover=false;
				pontuacao=0;
				vidas=3;
				nivel=1;
			}
			comecar=true;
			pause=false;
			break;
		case 's': //comecar pela direita
			if(comecar == false){
				direcaoX = 0.7; //velocidade
				direcaoY = 0.7; //velocidade
			}
			if(gameover == true){
				gameover=false;
				pontuacao=0;
				vidas=3;
				nivel=1;
			}
			comecar=true;
			pause=false;
			break;
		case 'q': //pausa o jogo
			pause=!(pause);
			break;
	}
	glutPostRedisplay();
}

//logica de colisao
bool colisao(float bolaX, float bolaY, float bolaComp, float bolaAltura, float objetoX, float objetoY, float objetoComp, float objetoAltura){
	if(bolaY+bolaAltura < objetoY) return false; //se bola esta mais abaixo da altura objeto
	else if(bolaY > objetoY+objetoAltura) return false; //se bola esta mais acima do objeto
	else if(bolaX+bolaComp < objetoX) return  false; //se a bola esta mais a esquerda do objeto
	else if(bolaX > objetoX + objetoComp) return false; //se a bola esta mais a direita do objeto
	return true; //houve colisao
}

//movimento da bola
void movimentaBola(int passo) {
	
	if((comecar==true)&&(pause==false)){ //se o jogo nao esta pausado e esta rodando
		bolaX = bolaX + direcaoX; //eixo x da bola eh incrementado com a direcao
		
		//verifica se a bola colidiu com os blocos
		for(int n=0; n < NUMBLOCOS; n++){ //pra cada movimento eh verificado se nao houve colisao com um bloco
			if(blocos[n].ativo == true){
				if(colisao(bolaX, bolaY, bolaCA, bolaCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].altura) == true){ //se houve colisao no eixo x
					direcaoX =- direcaoX;
					blocos[n].ativo=false;
					pontuacao = pontuacao + 1000;
					blocosDestruidos++;
					if(blocosDestruidos == NUMBLOCOS){
						for(int i=0; i<100000000;i++); //utilizado para delay
						proximoNivel();
					}
					break;
				}
			}
		}
		
		bolaY = bolaY + direcaoY; //eixo y da bola eh incrementado com a direcao
		for(int n=0; n < NUMBLOCOS; n++){
			if(blocos[n].ativo == true){
				if(colisao(bolaX, bolaY, bolaCA, bolaCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].altura) == true){
					direcaoY =- direcaoY;
					blocos[n].ativo=false;
					pontuacao = pontuacao + 1000;
					blocosDestruidos++;
					if(blocosDestruidos == NUMBLOCOS){
						for(int i=0; i<100000000;i++);
						proximoNivel();
					}
					break;
				}
			}
		}
		
		//logica bola
		if ((bolaY <= 0)||(bolaX+(bolaCA/2)< 0) || (bolaY+bolaCA-20 > TAMJANELA)){ //se o jogador morreu
			//volta a nave ao ponto inicial
			comecar=false;
			pause=true;
			naveX = 300;
			bolaX = 325;
			bolaY = 70;
			
			//decrementa a pontuacao e a quantidade vidas
			pontuacao = pontuacao - 5000;
			vidas--;
			
			if(vidas==0){ //se vida eh igual a zero
				for(int n=0; n < NUMBLOCOS; n++){ //ativa novamente os blocos
					blocos[n].ativo = true;
				}
				blocosDestruidos=0;
				gameover=true; //da game over
			}
			for(int i=0; i<30000000;i++); //usado para delay
			
		} else if(bolaX < 0){ //se bola bateu no lado esquerdo da tela, inverte a direcao em x
			direcaoX =- direcaoX;
		} else if (bolaX + bolaCA > TAMJANELA) { //se bola bateu no lado direito da tela, inverte direcao em x
			direcaoX =- direcaoX;
			if(bolaY < 0){
				direcaoY=-direcaoY;
			}
		} else if (bolaY + bolaCA > TAMJANELA){ //se bola no limite superior inverte a direcao em y
			direcaoY =- direcaoY;
		}
		
		//quando a bola encosta na nave
		if(colisao(bolaX, bolaY, bolaCA, bolaCA, naveX, naveY, naveComprimento, naveAltura) == true){	
			direcaoY = -direcaoY;
			//aqui eh pra tornar mais dinamica a bola
			direcaoX = ((bolaX + bolaCA / 2) - naveX - 40) / 100;
		}
		
	}
	glutPostRedisplay();
	glutTimerFunc(1,movimentaBola,1);
}

//passa para o proximo nivel
void proximoNivel(){
	nivel++;
	vidas++;
	
	comecar=false; //nao permite que a proxima fase comece automaticamente
	pause=true; 
	
	//retorna para as coodernadas
	naveX = 300;
	bolaX = 325;
	bolaY = 70;
	direcaoX = 0.7; //velocidade
	direcaoY = 0.7; //velocidade
	
	for(int n=0; n < NUMBLOCOS; n++){ //ativa todos os blocos novamente
		blocos[n].ativo = true; 
	}
	blocosDestruidos=0; //contador de blocos destruidos
	
	cor++;
	if(cor==4){
		cor=0;
	}
}

void alteraTamanhoJanela(GLsizei w, GLsizei h) {
	if(h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h){
		gluOrtho2D(0, TAMJANELA, 0, TAMJANELA*h/w);
	}else{
		gluOrtho2D(0, TAMJANELA*w/h, 0, TAMJANELA);
	}
} 

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(950, TAMJANELA);
	glutInitWindowPosition(10,10);
	glutCreateWindow("Trabalho Final - Rafael Nascimento");
	iniciaBlocos(); //inicializa os blocos
	glutDisplayFunc(desenha);
	glutReshapeFunc(alteraTamanhoJanela);
	glutTimerFunc(1,movimentaBola,1);
	glutSpecialFunc(teclado);
	glutKeyboardFunc(teclado2);
	glutMouseFunc(Mouse);
	Inicializa();
	glutMainLoop();
	return 0;
}
