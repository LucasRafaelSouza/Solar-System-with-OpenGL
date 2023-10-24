#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
//variaveis globais utilizadas durante o codigo
int gapx = 20;
int gapy = 15;
int nProjeteis = 0;
int nProjeteisIni = 0;
int Rodando = 1;
int iniDerro = 0, contador =0, InimigoXexplo, InimigoYexplo, sinalizador = 0,contador2=0, contador1=0,contador3=0;
GLuint texturaPersonagem, texturaNaveDireita, texturaNaveEsquerda, texturaFundo1, texturaFundo2, TexturaExplosao, TexturaFinal;
//define os tipos inimigo, usuario e projetil
struct Inimigo
  {
      int x;
      int y;
      int compx;
      int compy;
      int vX=2;
      int vY=10;
      int desenha = 1;

  };
struct Usuario
  {
      int x = 370;
      int y = 550;
      int compx = 60;
      int compy = 40;
      int vX = 20;
      int vida = 100;
  };Usuario personagem;
struct Projetil
  {
      int x;
      int y;
      int compy = 20;
      int vY = 75;
      int desenha;
  };
Projetil *projetil;
Projetil *projetilInimigo;
Inimigo inimigos[4][11];

//cabecalho tiroinimigo
void criaTiroInimigo();
//cabecalho inicializa
void inicializa();
//funcao que carrega a textura
GLuint carregaTextura(char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro carregando a textura: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

void atualizaCena(int valorQualquer) {
    if(Rodando == 1 & personagem.vida != 0){
    for(int j =0; j<4;j++){
        for(int i =0; i<11; i++){
            inimigos[j][i].x += inimigos[j][i].vX;//velocidade
        }
    }

    //define os inimigos para serem comparados nos sitemas de colisao de tela
    int posicaoMaisDirX=0,posicaoMaisDirY = 0;
    for(int j =0; j<4;j++){
        for(int i =10; i>=0; i--){
            if((inimigos[j][i].desenha == 1)&&(i>posicaoMaisDirX)){
                posicaoMaisDirX = i;
                posicaoMaisDirY = j;
            }
        }

    }
    int posicaoMaisEsqX=10,posicaoMaisEsqY = 0;
    for(int j =0; j<4;j++){
        for(int i =0; i<11; i++){
            if((inimigos[j][i].desenha == 1)&&(i<posicaoMaisEsqX)){
                posicaoMaisEsqX = i;
                posicaoMaisEsqY = j;
            }
        }

    }
    //colisao a direita
    if(inimigos[posicaoMaisDirY][posicaoMaisDirX].x+inimigos[posicaoMaisDirY][posicaoMaisDirX].compx > 800){
        int distancia = (inimigos[posicaoMaisDirY][posicaoMaisDirX].x+inimigos[posicaoMaisDirY][posicaoMaisDirX].compx)-800;
        for(int j =0; j<4;j++){
            for(int i =0; i<11; i++){
                inimigos[j][i].x -= distancia;
            }
        }

    }
    else if(inimigos[posicaoMaisDirY][posicaoMaisDirX].x+inimigos[posicaoMaisDirY][posicaoMaisDirX].compx == 800){
        for(int j =0; j<4;j++){
            for(int i =0; i<11; i++){
                inimigos[j][i].vX = -inimigos[j][i].vX;
                inimigos[j][i].y+= inimigos[j][i].vY;
            }
        }

    }
    //colisao a esquerda da tela
    else if(inimigos[posicaoMaisEsqY][posicaoMaisEsqX].x<0){
        int distancia = (inimigos[posicaoMaisEsqY][posicaoMaisEsqX].x)*-1;
        for(int j =0; j<4;j++){
            for(int i =0; i<11; i++){
                inimigos[j][i].x += distancia;
            }
        }
    }
    else if(inimigos[posicaoMaisEsqY][posicaoMaisEsqX].x==0){
        for(int j =0; j<4;j++){
            for(int i =0; i<11; i++){
                inimigos[j][i].vX = -inimigos[j][i].vX;
                inimigos[j][i].y+= inimigos[j][i].vY;
            }
        }
    //SISTEMA DE COLISAO DOS PROJETEIS
    //velocidade do projetil
    }
    for(int i =0; i<nProjeteis;i++){

            projetil[i].y -= 40;
     }
     //Colisao dos projeteis aliados com os inimigos
    // if(((projetil[h].x>= inimigos[j][i].x)&&(projetil[h].x<= inimigos[j][i].x+inimigos[j][i].compx))&&((projetil[h].y-20<=inimigos[j][i].y+inimigos[j][i].compy)&&(inimigos[j][i].y<projetil[h].y-20))
    for(int j =0; j<4;j++){ //colisao
        for(int i =0; i<11; i++){
           for(int h =0; h<nProjeteis;h++){
               //verifica se o tiro esta ativo
               if(projetil[h].desenha == 1 && inimigos[j][i].desenha == 1){
               if(((projetil[h].x>= inimigos[j][i].x)&&(projetil[h].x<= inimigos[j][i].x+inimigos[j][i].compx))&&(((projetil[h].y-20<=inimigos[j][i].y+inimigos[j][i].compy)&&(inimigos[j][i].y<=projetil[h].y-20))||((projetil[h].y<=inimigos[j][i].y+inimigos[j][i].compy)&&(inimigos[j][i].y<=projetil[h].y)))){

                    InimigoXexplo = i;
                    InimigoYexplo = j;
                    sinalizador = 1;
                    contador2 = contador1;
                     projetil[h].desenha = 0;
                     inimigos[j][i].desenha= 0;

                     //inicializa();
                     break;



               }
            }
           }

        }
    }
    //cria tiro inimigo
    int numero = (rand() % 101);
    if(numero % 2 == 0){
       criaTiroInimigo();

    }
    //velocidade tiro inimigo
    for(int i =0; i<nProjeteisIni;i++){

            projetilInimigo[i].y += 30;
     }
     //colisao com o personagem
      for(int h =0; h<nProjeteisIni;h++){
            if(projetilInimigo[h].desenha == 1){
            if(((projetilInimigo[h].x>= personagem.x)&&(projetilInimigo[h].x<= personagem.x+personagem.compx))&&(((projetilInimigo[h].y+20<=personagem.y+personagem.compy)&&(personagem.y<=projetilInimigo[h].y+20))||((projetilInimigo[h].y<=personagem.y+personagem.compy)&&(personagem.y<=projetilInimigo[h].y)))){
                personagem.vida = personagem.vida - 10;
                break;

            }
        }
     }
     //colisao com a tela de baixo, fim de jogo
     for(int j =0; j<4;j++){
        for(int i =0; i<11; i++){
            if(inimigos[j][i].y+inimigos[j][i].compy >= 600){
                personagem.vida = 0;
                //Rodando = 0;

            }
        }
    }

    //confere se o tiro aliado saiu da tela
    for(int i =0; i<nProjeteis; i++){
        if(projetil[i].y <= 0){
            projetil[i].desenha = 0;
        }

     }
     //confere se o tiro inimigo saiu da tela
    for(int i =0; i<nProjeteisIni;i++){
            if(projetilInimigo[i].y > 600){
                projetilInimigo[i].desenha = 0;
            }


     }
     //confere quantos inimigos o usuario derrotou
    iniDerro = 0;
    for(int j =0; j<4;j++){
        for(int i =0; i<11; i++){
            if(inimigos[j][i].desenha == 0){
               iniDerro++;

            }
        }
    }
    //verifica se derrotou todos os inimigos
    if(iniDerro == 44){
         personagem.vida = 0;


    }
    //anula os tiros quando o jogo termina
    if(personagem.vida == 0){

        for(int i =0; i<nProjeteis;i++){
                projetil[i].desenha = 0;
     }
      for(int i =0; i<nProjeteisIni;i++){
            projetilInimigo[i].desenha = 0;
    }
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TexturaFinal);
    glBegin(GL_QUADS);//GL_QUADS
        glTexCoord2f(0,1);  glVertex2i(personagem.x,personagem.y);
        glTexCoord2f(0,0);  glVertex2i(personagem.x,personagem.y+personagem.compy);
        glTexCoord2f(1,0);  glVertex2i(personagem.x+personagem.compx,personagem.y+personagem.compy);
        glTexCoord2f(1,1);  glVertex2i(personagem.x+personagem.compx,personagem.y);
     glEnd();
     glutSwapBuffers();
     glDisable(GL_TEXTURE_2D);
     Sleep(50);

    }



    }
    //fim
    if(contador1 == contador2+1){

        sinalizador = 0;
    }


    glutPostRedisplay();
    glutTimerFunc(33, atualizaCena, 0);

}
//função que cria o tiro para os inimigos
void criaTiroInimigo(){
    if((Rodando == 1) & (personagem.vida != 0)){
        int num = (rand() % 12);
        int num2 = (rand() % 4);

        int cont = 0;
        for (int i =num2+1; i<4;i++){

            if(inimigos[i][num].desenha == 0){
                cont += 1;
            }
        }

         //criar um sistema de varias linhas para definir quando um tiro pode ou nao sair do inimigo
        if((inimigos[num2][num].desenha == 1)&&(cont == 4-(num2+1))){
        nProjeteisIni += 1;
        projetilInimigo = ( Projetil *) realloc(projetilInimigo,nProjeteisIni * sizeof( Projetil));

        //while(inimigos[3][num].desenha == 0){
            //int num = (rand() % 12);
        //}

        if( nProjeteisIni >= 1){
            projetilInimigo[nProjeteisIni-1].x = inimigos[num2][num].x+(inimigos[num2][num].compx/2);
            projetilInimigo[nProjeteisIni-1].y = inimigos[num2][num].y+20;
            projetilInimigo[nProjeteisIni-1].desenha = 1;

        }

        }
    }

}
//funcao que cria os inimigos
void criaInimigos(){
    for(int j = 0; j<4; j++){
        for(int i = 0; i<11;i++){
                if((i == 0)){
                    inimigos[j][i].x = gapx+50;
                    inimigos[j][i].compx = 40;
                   }
                else{
                    inimigos[j][i].x = inimigos[j][i-1].x + 60;
                    inimigos[j][i].compx = 40;
                }

                if(j == 0){
                        inimigos[j][i].y = 10;
                        inimigos[j][i].compy = 30;
                }
                else{
                    inimigos[j][i].y= inimigos[j-1][i].y + 45;
                    inimigos[j][i].compy = 30;
                }


        }

    }
}

void teclaPressionada(unsigned char key, int x, int y) {
    switch(key) {
    case 27:
        exit(0);
        break;
    case 100:
        //anda para a direita
        if(Rodando == 1 & personagem.vida != 0){
        if( (personagem.x+personagem.compx + personagem.vX) > 800){
            personagem.x =800-personagem.compx ;
        }
        else if(personagem.x == 800-personagem.compx ){
            personagem.x = 800-personagem.compx ;
        }
        else{
            personagem.x += personagem.vX;
        }
        //glutPostRedisplay();
        }
        break;

    case 97:
        //anda para a esquerda
        if(Rodando == 1 & personagem.vida != 0){
        if (personagem.x + (-personagem.vX) < 0){
            personagem.x -= personagem.x;
        }
        else if (personagem.x == 0){
            personagem.x = 0;
        }
        else{
         personagem.x -= personagem.vX;
        }
        //glutPostRedisplay();
        }
        break;


    case 32:
        //atira projetil aliado
        if(Rodando == 1 && personagem.vida != 0){
        nProjeteis += 1;
        projetil = ( Projetil *) realloc(projetil,nProjeteis * sizeof( Projetil));

        //if( nProjeteis >= 1){
            //for(int i = nProjeteis-1; i<nProjeteis; i++){
                projetil[nProjeteis-1].x = personagem.x+(personagem.compx/2);

                projetil[nProjeteis-1].y = personagem.y+20;

                projetil[nProjeteis-1].desenha = 1;
            //}
        //}
        }
        break;

    case 112:
        //pausa
        if(Rodando == 1){
            Rodando = 0;
        }
        else{
            Rodando = 1;
        }
        break;
    case 114:
        //reinicia
        criaInimigos();
        for(int j =0; j<4;j++){
            for(int i =0; i<11; i++){
                inimigos[j][i].vX = abs(inimigos[j][i].vX) ;
                inimigos[j][i].desenha = 1;
            }
        }
        for(int i =0; i<nProjeteisIni;i++){
            projetilInimigo[i].desenha = 0;
        }
        for(int i =0; i<nProjeteis;i++){
            projetil[i].desenha = 0;
     }

        personagem.x =370;
        personagem.y = 550;
        personagem.vida = 100;
        glutPostRedisplay();
        break;
    default:
        break;


    }
}
//funcao que escreve a mensagem
void escreve(void* fonte, char* texto, float x, float y) {
  glRasterPos2f(x, y);

  for (int i = 0; i < strlen(texto); i++) {
     glutBitmapCharacter(fonte, texto[i]);
  }
}
void desenhaMinhaCena() {
    contador++;contador1++;//conta quantas vezes a função foi chamada
    if((contador> 101)&(Rodando == 1) & (personagem.vida != 0))//&&
        contador = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    //desenha plano de fundo
    glColor3f(1,1,1);
    //textura de fundo
    glEnable(GL_TEXTURE_2D);
    if(contador<=50){
       glBindTexture(GL_TEXTURE_2D, texturaFundo1);
    }
    else{
        glBindTexture(GL_TEXTURE_2D, texturaFundo2);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex2i(0,0);
    glTexCoord2f(0,0); glVertex2i(0,600);
    glTexCoord2f(1,0); glVertex2i(800,600);
    glTexCoord2f(1,1); glVertex2i(800,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //desenha inimigos
    glEnable(GL_TEXTURE_2D);
    if(inimigos[0][0].vX>0)
        glBindTexture(GL_TEXTURE_2D, texturaNaveDireita);
    else
        glBindTexture(GL_TEXTURE_2D, texturaNaveEsquerda);
    for(int j = 0;j<4;j++){
        for(int i = 0; i<11;i++){
        if(inimigos[j][i].desenha == 1){
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
            glTexCoord2f(0,1); glVertex2i(inimigos[j][i].x,inimigos[j][i].y);
            glTexCoord2f(0,0); glVertex2i(inimigos[j][i].x,inimigos[j][i].y+inimigos[j][i].compy);
            glTexCoord2f(1,0); glVertex2i(inimigos[j][i].x+inimigos[j][i].compx,inimigos[j][i].y+inimigos[j][i].compy);
            glTexCoord2f(1,1); glVertex2i(inimigos[j][i].x+inimigos[j][i].compx,inimigos[j][i].y);
        glEnd();
        //glFlush();
        }
    }
    }
    glDisable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    //


    //desenha personagem do usuario
    if(personagem.vida != 0){
    //texturaPersonagem = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/nave1Definitiva-3.png");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaPersonagem);
    glBegin(GL_QUADS);//GL_QUADS
        glTexCoord2f(0,1);  glVertex2i(personagem.x,personagem.y);
        glTexCoord2f(0,0);  glVertex2i(personagem.x,personagem.y+personagem.compy);
        glTexCoord2f(1,0);  glVertex2i(personagem.x+personagem.compx,personagem.y+personagem.compy);
        glTexCoord2f(1,1);  glVertex2i(personagem.x+personagem.compx,personagem.y);
     glEnd();
     glDisable(GL_TEXTURE_2D);
    }

     //desenhar projetil do usuario
      glColor3f(0.98431372549,0.60784313725,0.39607843137);
     for(int i =0; i<nProjeteis;i++){
        if(projetil[i].desenha == 1){
        glLineWidth(3);
        glBegin(GL_LINES);
            glVertex2i(projetil[i].x,projetil[i].y);
            glVertex2i(projetil[i].x,projetil[i].y-(20));
        glEnd();
       }
     }
     //desenha projetil inimigo
     glColor3f(0.99215686274,0.94901960784,0.39215686274);
     for(int i =0; i<nProjeteisIni;i++){
        if(projetilInimigo[i].desenha == 1){
        glLineWidth(3);
        glBegin(GL_LINES);
            glVertex2i(projetilInimigo[i].x,projetilInimigo[i].y);
            glVertex2i(projetilInimigo[i].x,projetilInimigo[i].y+(20));
        glEnd();
       }
     }
     //desenha a barra de vida
    glColor3f(0.3,0.3,0.3);
     glBegin(GL_QUADS);
       glVertex2i(15,525);
       glVertex2i(15,585);
       glVertex2i(125,585);
       glVertex2i(125,525);
    glEnd();
     glColor3f(0,1,0);
     glBegin(GL_QUADS);
       glVertex2i(20,530);
       glVertex2i(20,580);
       glVertex2i(20+personagem.vida,580);
       glVertex2i(20+personagem.vida,530);
    glEnd();
    //explosao da nave inimiga
    if(sinalizador == 1){
        glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, TexturaExplosao);
                    glColor3f(1,1,1);
                    glBegin(GL_QUADS);
                        glTexCoord2f(0,1); glVertex2i(inimigos[InimigoYexplo][InimigoXexplo].x,inimigos[InimigoYexplo][InimigoXexplo].y);
                        glTexCoord2f(0,0); glVertex2i(inimigos[InimigoYexplo][InimigoXexplo].x,inimigos[InimigoYexplo][InimigoXexplo].y+inimigos[InimigoYexplo][InimigoXexplo].compy);
                        glTexCoord2f(1,0); glVertex2i(inimigos[InimigoYexplo][InimigoXexplo].x+inimigos[InimigoYexplo][InimigoXexplo].compx,inimigos[InimigoYexplo][InimigoXexplo].y+inimigos[InimigoYexplo][InimigoXexplo].compy);
                        glTexCoord2f(1,1); glVertex2i(inimigos[InimigoYexplo][InimigoXexplo].x+inimigos[InimigoYexplo][InimigoXexplo].compx,inimigos[InimigoYexplo][InimigoXexplo].y);
                    glEnd();
    }
    //escreve mensagem
    if(iniDerro == 44){
    glColor3f (0.97647058823,0.9294117647,0.76470588235);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, "Vencedor!", 20, 560);
    }
    else if(personagem.vida ==0){
        glColor3f (0.97647058823,0.9294117647,0.76470588235);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, "Perdedor!", 20, 560);
    }
    //escreve mensagem pausado
    if((Rodando == 0)&(personagem.vida != 0)){
        glColor3f (0.0,0.0,0.0);
    escreve(GLUT_BITMAP_TIMES_ROMAN_24, "Pausado", 20, 560);
    }

    //final
    glutSwapBuffers();




}
//inicializa as texturas
void inicializa() {

    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    texturaPersonagem = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/nave1Definitiva-3.png");
    texturaFundo1 = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/TeladeFundo1.png");
    texturaFundo2 = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/TeladeFundo2.png");
    texturaNaveDireita = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/Nave2Direita.png");
    texturaNaveEsquerda = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/Nave2Esquerda.png");
    TexturaExplosao = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/Explosion2Azul.png");
    TexturaFinal = carregaTextura("C:/Users/WazPC/OneDrive/Documentos/jogo2/bin/Debug/Explosion.png");

}
//redimensiona a tela
void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 800, 600, 0, -1, 1);


   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    criaInimigos();
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 50);


    glutCreateWindow("Galaxian");


    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    atualizaCena(0);



    inicializa();

    glutMainLoop();
    return 0;
}
