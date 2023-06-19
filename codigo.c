/*Trabalo 3 Computação gráfica
Nomes:  Gustavo Campos Pádua        20203001143
        Herinque Frade Vieira       20203001321
        Marcelo Morais Barbosa      20203014713
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#define DIVISOES_ESFERA 20
#define RAIO_ESFERA 25
#define AUMENTO_WIREFRAME 1.001



typedef struct esfera{
    GLfloat raio, raioDoSistema;
    GLuint idTextura;
    GLfloat angle, angularVeloc;
	GLfloat selfAngle, selfAngularVeloc;
    GLfloat matAmb[4];
    GLfloat matDif[4];
    GLfloat matSpec[4];
    GLfloat matEmi[4];
    GLint matShininess;
}esfera;

esfera sol;
esfera mercurio;
esfera venus;
esfera terra;
esfera lua;
esfera marte;
esfera jupiter;
esfera jupiterLua1;
esfera jupiterLua2;
esfera jupiterLua3;
esfera saturno;
esfera uranus;
esfera uranusLua1;
esfera uranusLua2;
esfera uranusLua3;
esfera netuno;
esfera netunoLua1;
esfera netunoLua2;
esfera netunoLua3;
esfera skybox;

typedef struct camera{
    GLdouble eyeX, eyeY, eyeZ;
	GLdouble centerX, centerY, centerZ;
	GLdouble upX, upY, upZ;
}camera;
camera pov;

float light0[3][4] = {
				{-0.1f, -0.1f, -0.1f, -1.f}, // ambient
				{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
				{ 1.f,  1.f,  1.f, 1.f } // specular
	};


GLuint carregaTextura(const char* arquivo) 
{
    GLuint idTextura = SOIL_load_OGL_texture
	(
		arquivo,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
    return idTextura;
}


// Desenha uma esfera na origem, com certo raio e subdivisões
// latitudinais e longitudinais
//
// Não podemos usar glutSolidSphere, porque ela não chama
// glTexCoord para os vértices. Logo, se você quer texturizar
// uma esfera, não pode usar glutSolidSphere
void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}

void desenhaPlaneta(esfera planeta)
{

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

        //glColor3f(1, 1, 1);
        glMaterialfv(GL_FRONT,GL_EMISSION, planeta.matEmi);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, planeta.idTextura);

        solidSphere((int)planeta.raio, DIVISOES_ESFERA, DIVISOES_ESFERA);
        
        
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
	return;
}

void desenhaCena(void)
{
    float lightposition[4] = {-terra.raioDoSistema, 0, 0, 0};
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(pov.eyeX, pov.eyeY, pov.eyeZ,
              pov.centerX, pov.centerY, pov.centerZ,
	          pov.upX, pov.upY, pov.upZ);

    // glLoadIdentity();

    glPushMatrix();

    desenhaPlaneta(skybox);
    //Roraciona o sol
    glRotatef(sol.selfAngle, 0, 0, 1);
	desenhaPlaneta(sol);
    glRotatef(-sol.selfAngle, 0, 0, 1);

    //desenha mercurio
    glRotatef(mercurio.angle, 0, 0, 1);
    glTranslatef(mercurio.raioDoSistema, 0, 0);
    glRotatef(mercurio.selfAngle, 0, 0, 1);
	desenhaPlaneta(mercurio);
    glRotatef(-mercurio.selfAngle, 0, 0, 1);
    glTranslatef(-mercurio.raioDoSistema, 0, 0);
    glRotatef(-mercurio.angle, 0, 0, 1);

    //desenha venus
    glRotatef(venus.angle, 0, 0, 1);
    glTranslatef(venus.raioDoSistema, 0, 0);
    glRotatef(venus.selfAngle, 0, 0, 1);
	desenhaPlaneta(venus);
    glRotatef(-venus.selfAngle, 0, 0, 1);
    glTranslatef(-venus.raioDoSistema, 0, 0);
    glRotatef(-venus.angle, 0, 0, 1);

    //Posição da terra
    glPushMatrix();
    glRotatef(terra.angle, 0, 0, 1);
    glRotatef(terra.angle, lightposition[0], 1, 1);
    glTranslatef(-terra.raioDoSistema, 0, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
    glTranslatef(terra.raioDoSistema, 0, 0);
    glRotatef(-terra.angle, lightposition[0], 1, 1);
    glTranslatef(terra.raioDoSistema, 0, 0);
    glRotatef(terra.selfAngle, 0, 0, 1);
	desenhaPlaneta(terra);
    glRotatef(-terra.selfAngle, 0, 0, 1);
    //Lua da terra
    glRotatef(lua.angle, 0, 0, 1);
    glTranslatef(lua.raioDoSistema, 0, 0);
    glRotatef(lua.selfAngle, 0, 0, 1);
    desenhaPlaneta(lua);
    glRotatef(-lua.selfAngle, 0, 0, 1);
    glTranslatef(-lua.raioDoSistema, 0, 0);
    glRotatef(-lua.angle, 0, 0, 1);
    glRotatef(-terra.angle, 0, 0, 1);
    glPopMatrix();
    
    //desenha marte
    glRotatef(marte.angle, 0, 0, 1);
    glTranslatef(marte.raioDoSistema, 0, 0);
    glRotatef(marte.selfAngle, 0, 0, 1);
	desenhaPlaneta(marte);
    glRotatef(-marte.selfAngle, 0, 0, 1);
    glTranslatef(-marte.raioDoSistema, 0, 0);
    glRotatef(-marte.angle, 0, 0, 1);

    //desenha Jupiter
    glPushMatrix();
    glRotatef(jupiter.angle, 0, 0, 1);
    glTranslatef(jupiter.raioDoSistema, 0, 0);
    glRotatef(jupiter.selfAngle, 0, 0, 1);
	desenhaPlaneta(jupiter);
    glRotatef(-jupiter.selfAngle, 0, 0, 1);

    glRotatef(jupiterLua1.angle, 0, 0, 1);
    glTranslatef(jupiterLua1.raioDoSistema, 0, 0);
    glRotatef(jupiterLua1.selfAngle, 0, 0, 1);
	desenhaPlaneta(jupiterLua1);
    glRotatef(-jupiterLua1.selfAngle, 0, 0, 1);
    glTranslatef(-jupiterLua1.raioDoSistema, 0, 0);
    glRotatef(-jupiterLua1.angle, 0, 0, 1);

    glRotatef(jupiterLua2.angle, 0, 0, 1);
    glTranslatef(jupiterLua2.raioDoSistema, 0, 0);
    glRotatef(jupiterLua2.selfAngle, 0, 0, 1);
	desenhaPlaneta(jupiterLua2);
    glRotatef(-jupiterLua2.selfAngle, 0, 0, 1);
    glTranslatef(-jupiterLua2.raioDoSistema, 0, 0);
    glRotatef(-jupiterLua2.angle, 0, 0, 1);

    glRotatef(jupiterLua3.angle, 0, 0, 1);
    glTranslatef(jupiterLua3.raioDoSistema, 0, 0);
    glRotatef(jupiterLua3.selfAngle, 0, 0, 1);
	desenhaPlaneta(jupiterLua3);
    glRotatef(-jupiterLua3.selfAngle, 0, 0, 1);
    glTranslatef(-jupiterLua3.raioDoSistema, 0, 0);
    glRotatef(-jupiterLua3.angle, 0, 0, 1);

    glTranslatef(-jupiter.raioDoSistema, 0, 0);
    glRotatef(-jupiter.angle, 0, 0, 1);
    glPopMatrix();

    //desenha saturno
    glRotatef(saturno.angle, 0, 0, 1);
    glTranslatef(saturno.raioDoSistema, 0, 0);
    glRotatef(saturno.selfAngle, 0, 0, 1);
	desenhaPlaneta(saturno);
    glRotatef(-saturno.selfAngle, 0, 0, 1);
    glTranslatef(-saturno.raioDoSistema, 0, 0);
    glRotatef(-saturno.angle, 0, 0, 1);


    //urano
    glPushMatrix();
    glRotatef(uranus.angle, 0, 0, 1);
    glTranslatef(uranus.raioDoSistema, 0, 0);
    glRotatef(uranus.selfAngle, 0, 0, 1);
	desenhaPlaneta(uranus);
    glRotatef(-uranus.selfAngle, 0, 0, 1);

    glRotatef(uranusLua1.angle, 0, 0, 1);
    glTranslatef(uranusLua1.raioDoSistema, 0, 0);
    glRotatef(uranusLua1.selfAngle, 0, 0, 1);
	desenhaPlaneta(uranusLua1);
    glRotatef(-uranusLua1.selfAngle, 0, 0, 1);
    glTranslatef(-uranusLua1.raioDoSistema, 0, 0);
    glRotatef(-uranusLua1.angle, 0, 0, 1);

    glRotatef(uranusLua2.angle, 0, 0, 1);
    glTranslatef(uranusLua2.raioDoSistema, 0, 0);
    glRotatef(uranusLua2.selfAngle, 0, 0, 1);
	desenhaPlaneta(uranusLua2);
    glRotatef(-uranusLua2.selfAngle, 0, 0, 1);
    glTranslatef(-uranusLua2.raioDoSistema, 0, 0);
    glRotatef(-uranusLua2.angle, 0, 0, 1);

    glRotatef(uranusLua3.angle, 0, 0, 1);
    glTranslatef(uranusLua3.raioDoSistema, 0, 0);
    glRotatef(uranusLua3.selfAngle, 0, 0, 1);
	desenhaPlaneta(uranusLua3);
    glRotatef(-uranusLua3.selfAngle, 0, 0, 1);
    glTranslatef(-uranusLua3.raioDoSistema, 0, 0);
    glRotatef(-uranusLua3.angle, 0, 0, 1);

    glTranslatef(-uranus.raioDoSistema, 0, 0);
    glRotatef(-uranus.angle, 0, 0, 1);
    glPopMatrix();

    //netuno
    glPushMatrix();
    glRotatef(netuno.angle, 0, 0, 1);
    glTranslatef(netuno.raioDoSistema, 0, 0);
    glRotatef(netuno.selfAngle, 0, 0, 1);
	desenhaPlaneta(netuno);
    glRotatef(-netuno.selfAngle, 0, 0, 1);

    glRotatef(netunoLua1.angle, 0, 0, 1);
    glTranslatef(netunoLua1.raioDoSistema, 0, 0);
    glRotatef(netunoLua1.selfAngle, 0, 0, 1);
	desenhaPlaneta(netunoLua1);
    glRotatef(-netunoLua1.selfAngle, 0, 0, 1);
    glTranslatef(-netunoLua1.raioDoSistema, 0, 0);
    glRotatef(-netunoLua1.angle, 0, 0, 1);

    glRotatef(netunoLua2.angle, 0, 0, 1);
    glTranslatef(netunoLua2.raioDoSistema, 0, 0);
    glRotatef(netunoLua2.selfAngle, 0, 0, 1);
	desenhaPlaneta(netunoLua2);
    glRotatef(-netunoLua2.selfAngle, 0, 0, 1);
    glTranslatef(-netunoLua2.raioDoSistema, 0, 0);
    glRotatef(-netunoLua2.angle, 0, 0, 1);

    glRotatef(netunoLua3.angle, 0, 0, 1);
    glTranslatef(netunoLua3.raioDoSistema, 0, 0);
    glRotatef(netunoLua3.selfAngle, 0, 0, 1);
	desenhaPlaneta(netunoLua3);
    glRotatef(-netunoLua3.selfAngle, 0, 0, 1);
    glTranslatef(-netunoLua3.raioDoSistema, 0, 0);
    glRotatef(-netunoLua3.angle, 0, 0, 1);

    glTranslatef(-netuno.raioDoSistema, 0, 0);
    glRotatef(-netuno.angle, 0, 0, 1);
    glPopMatrix();
   
    glPopMatrix();

    
	
    glFlush();
}

void inicializa(void)
{
    // Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.0);

 // ativa o depth buffer
    glEnable(GL_DEPTH_TEST);

	glFrustum(-100, 100,  // l, r
          	  -100, 100,  // b, t
          	  -100, 100); // n, f


    // habilita mesclagem de cores, para termos suporte a texturas (semi-)transparentes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    

    glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);


	pov.eyeX=0;
	pov.eyeY=100;
	pov.eyeZ=0;
	pov.upX=0;
	pov.upY=0;
	pov.upZ=1;


    // carrega uma textura

	sol.raio=15;
	sol.selfAngularVeloc=0.5;
	sol.idTextura=carregaTextura("sol.png");
    sol.matEmi[0]=1;
    sol.matEmi[1]=1;
    sol.matEmi[2]=1;
    sol.matEmi[3]=1;

    mercurio.idTextura = carregaTextura("mercurio.png");
    mercurio.raioDoSistema=30;
    mercurio.raio=1.4;
    mercurio.selfAngularVeloc=15;
    mercurio.angularVeloc=3.0;

    venus.raioDoSistema=40;
    venus.raio=1.9;
    venus.selfAngularVeloc=12;
    venus.angularVeloc=2;
    venus.idTextura=carregaTextura("venus.png");


	terra.raio=2;
    terra.raioDoSistema=50;
    terra.selfAngularVeloc=10;
	terra.angularVeloc=1.0;
	terra.idTextura=carregaTextura("terra.png");

    lua.raio=1;
    lua.raioDoSistema=5;
    lua.selfAngularVeloc=5;
    lua.angularVeloc=1.0;
    lua.idTextura=carregaTextura("lua.png");
    lua.matEmi[0]=0.01;
    lua.matEmi[1]=0.01;
    lua.matEmi[2]=0.01;
    lua.matEmi[3]=0.01;

    marte.idTextura=carregaTextura("marte.png");
    marte.raio=1.5;
    marte.raioDoSistema=60;
    marte.angularVeloc=0.8;
    marte.selfAngularVeloc=1.0;

    jupiter.idTextura=carregaTextura("jupiter.png");
    jupiter.raio=4;
    jupiter.raioDoSistema=70;
    jupiter.angularVeloc=0.6;
    jupiter.selfAngularVeloc=1.0;

    jupiterLua1.idTextura=carregaTextura("lua1.png");
    jupiterLua1.raio=1;
    jupiterLua1.raioDoSistema=6;
    jupiterLua1.angularVeloc=2;
    jupiterLua1.selfAngularVeloc=5;

    jupiterLua2.idTextura=carregaTextura("lua2.png");
    jupiterLua2.raio=1;
    jupiterLua2.raioDoSistema=7;
    jupiterLua2.angularVeloc=1;
    jupiterLua2.selfAngularVeloc=5;

    jupiterLua3.idTextura=carregaTextura("lua3.png");
    jupiterLua3.raio=1;
    jupiterLua3.raioDoSistema=8;
    jupiterLua3.angularVeloc=0.5;
    jupiterLua3.selfAngularVeloc=5;

    saturno.idTextura=carregaTextura("saturno.png");
    saturno.raio=5;
    saturno.raioDoSistema=80;
    saturno.angularVeloc=0.4;
    saturno.selfAngularVeloc=1.0;

    uranus.idTextura=carregaTextura("urano.png");
    uranus.raio=4;
    uranus.raioDoSistema=95;
    uranus.angularVeloc=0.2;
    uranus.selfAngularVeloc=1.0;

    uranusLua1.idTextura=carregaTextura("lua4.png");
    uranusLua1.raio=1;
    uranusLua1.raioDoSistema=6;
    uranusLua1.angularVeloc=2;
    uranusLua1.selfAngularVeloc=1.0;

    uranusLua2.idTextura=carregaTextura("lua2.png");
    uranusLua2.raio=1;
    uranusLua2.raioDoSistema=7;
    uranusLua2.angularVeloc=1;
    uranusLua2.selfAngularVeloc=1.0;

    uranusLua3.idTextura=carregaTextura("lua3.png");
    uranusLua3.raio=1;
    uranusLua3.raioDoSistema=8;
    uranusLua3.angularVeloc=0.4;
    uranusLua3.selfAngularVeloc=1.0;

    netuno.idTextura=carregaTextura("netuno.png");
    netuno.raio=5;
    netuno.raioDoSistema=105;
    netuno.angularVeloc=0.1;
    netuno.selfAngularVeloc=1.0;

    netunoLua1.idTextura=carregaTextura("lua1.png");
    netunoLua1.raio=1;
    netunoLua1.raioDoSistema=6;
    uranusLua1.angularVeloc=2;
    uranusLua1.selfAngularVeloc=1.0;

    netunoLua2.idTextura=carregaTextura("lua2.png");
    netunoLua2.raio=1;
    netunoLua2.raioDoSistema=7;
    netunoLua2.angularVeloc=1;
    netunoLua2.selfAngularVeloc=1.0;

    netunoLua3.idTextura=carregaTextura("lua3.png");
    netunoLua3.raio=1;
    netunoLua3.raioDoSistema=8;
    netunoLua3.angularVeloc=0.4;
    netunoLua3.selfAngularVeloc=1.0;

    skybox.raio=800;
    skybox.idTextura=carregaTextura("skyBox.jpg");
    skybox.matEmi[0]=0.1;
    skybox.matEmi[1]=0.1;
    skybox.matEmi[2]=0.1;
    skybox.matEmi[3]=0.1;
}

void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, ((float)w)/h, 1, 1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void teclado(unsigned char key, int x, int y)
{
   switch(key)
   {
      // Tecla ESC
		case 27:
          exit(0);
		break;
		case 'z':
        case 'Z':
		light0[0][0]+=0.1;
		light0[0][1]+=0.1;
		light0[0][2]+=0.1;
		light0[0][3]+=0.1;
		break;
		case 'x':
        case 'X':
		light0[0][0]-=0.1;
		light0[0][1]-=0.1;
		light0[0][2]-=0.1;
		light0[0][3]-=0.1;
		break;
		case 'c':
        case 'C':
		light0[1][0]+=0.1;
		light0[1][1]+=0.1;
		light0[1][2]+=0.1;
		light0[1][3]+=0.1;
		break;
		case 'v':
        case 'V':
		light0[1][0]-=0.1;
		light0[1][1]-=0.1;
		light0[1][2]-=0.1;
		light0[1][3]-=0.1;
		break;
		case 'b':
        case 'B':
		light0[2][0]+=0.1;
		light0[2][1]+=0.1;
		light0[2][2]+=0.1;
		light0[2][3]+=0.1;
		break;
		case 'n':
        case 'N':
		light0[2][0]-=0.1;
		light0[2][1]-=0.1;
		light0[2][2]-=0.1;
		light0[2][3]-=0.1;
		break;
		default:
		break;
   }
    glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);

   glutPostRedisplay();
}

void atualiza(int periodo) {

    sol.selfAngle += sol.selfAngularVeloc;
    if(sol.selfAngle>=360) {sol.selfAngle=0;}
    terra.angle += terra.angularVeloc;
    if(terra.angle>=360) {terra.angle=0;}
    terra.selfAngle+=terra.selfAngularVeloc;
    if(terra.selfAngle>=360) {terra.selfAngle=0;}
    lua.selfAngle+=lua.selfAngularVeloc;
    if(lua.selfAngle>=360) {lua.selfAngle=0;}
    lua.angle+=lua.angularVeloc;
    if(lua.angle>=360) {lua.angle=0;}
    mercurio.selfAngle += mercurio.selfAngularVeloc;
    if(mercurio.selfAngle>=360) {mercurio.selfAngle=0;}
    mercurio.angle += mercurio.angularVeloc;
    if(mercurio.angle>=360) {mercurio.angle=0;}
    venus.selfAngle += venus.selfAngularVeloc;
    if(venus.selfAngle>=360) {venus.selfAngle=0;}
    venus.angle += venus.angularVeloc;
    if(venus.angle>=360) {venus.angle=0;}
    marte.selfAngle += marte.selfAngularVeloc;
    if(marte.selfAngle>=360) {marte.selfAngle=0;}
    marte.angle += marte.angularVeloc;
    if(marte.angle>=360) {marte.angle=0;}
    jupiter.selfAngle += jupiter.selfAngularVeloc;
    if(jupiter.selfAngle>=360) {jupiter.selfAngle=0;}
    jupiter.angle += jupiter.angularVeloc;
    if(jupiter.angle>=360) {jupiter.angle=0;}
    jupiterLua1.selfAngle += jupiterLua1.selfAngularVeloc;
    if(jupiterLua1.selfAngle>=360) {jupiterLua1.selfAngle=0;}
    jupiterLua1.angle += jupiterLua1.angularVeloc;
    if(jupiterLua1.angle>=360) {jupiterLua1.angle=0;}
    jupiterLua2.selfAngle += jupiterLua2.selfAngularVeloc;
    if(jupiterLua2.selfAngle>=360) {jupiterLua2.selfAngle=0;}
    jupiterLua2.angle += jupiterLua2.angularVeloc;
    if(jupiterLua2.angle>=360) {jupiterLua2.angle=0;}
    jupiterLua3.selfAngle += jupiterLua3.selfAngularVeloc;
    if(jupiterLua3.selfAngle>=360) {jupiterLua3.selfAngle=0;}
    jupiterLua3.angle += jupiterLua3.angularVeloc;
    if(jupiterLua3.angle>=360) {jupiterLua3.angle=0;}
    saturno.selfAngle += saturno.selfAngularVeloc;
    if(saturno.selfAngle>=360) {saturno.selfAngle=0;}
    saturno.angle += saturno.angularVeloc;
    if(saturno.angle>=360) {saturno.angle=0;}
    uranus.selfAngle += uranus.selfAngularVeloc;
    if(uranus.selfAngle>=360) {uranus.selfAngle=0;}
    uranus.angle += uranus.angularVeloc;
    if(uranus.angle>=360) {uranus.angle=0;}
    uranusLua1.selfAngle += uranusLua1.selfAngularVeloc;
    if(uranusLua1.selfAngle>=360) {uranusLua1.selfAngle=0;}
    uranusLua1.angle += uranusLua1.angularVeloc;
    if(uranusLua1.angle>=360) {uranusLua1.angle=0;}
    uranusLua2.selfAngle += uranusLua2.selfAngularVeloc;
    if(uranusLua2.selfAngle>=360) {uranusLua2.selfAngle=0;}
    uranusLua2.angle += uranusLua2.angularVeloc;
    if(uranusLua2.angle>=360) {uranusLua2.angle=0;}
    uranusLua3.selfAngle += uranusLua3.selfAngularVeloc;
    if(uranusLua3.selfAngle>=360) {uranusLua3.selfAngle=0;}
    uranusLua3.angle += uranusLua3.angularVeloc;
    if(uranusLua3.angle>=360) {uranusLua3.angle=0;}
    netuno.selfAngle += netuno.selfAngularVeloc;
    if(netuno.selfAngle>=360) {netuno.selfAngle=0;}
    netuno.angle += netuno.angularVeloc;
    if(netuno.angle>=360) {netuno.angle=0;}
    netunoLua1.selfAngle += netunoLua1.selfAngularVeloc;
    if(netunoLua1.selfAngle>=360) {netunoLua1.selfAngle=0;}
    netunoLua1.angle += netunoLua1.angularVeloc;
    if(netunoLua1.angle>=360) {netunoLua1.angle=0;}
    netunoLua2.selfAngle += netunoLua2.selfAngularVeloc;
    if(netunoLua2.selfAngle>=360) {netunoLua2.selfAngle=0;}
    netunoLua2.angle += netunoLua2.angularVeloc;
    if(netunoLua2.angle>=360) {netunoLua2.angle=0;}
    netunoLua3.selfAngle += netunoLua3.selfAngularVeloc;
    if(netunoLua3.selfAngle>=360) {netunoLua3.selfAngle=0;}
    netunoLua3.angle += netunoLua3.angularVeloc;
    if(netunoLua3.angle>=360) {netunoLua3.angle=0;}


    glutPostRedisplay();
    glutTimerFunc(periodo, atualiza, periodo);
}

void imprimeInstrucoes() {
    printf("Instruções:\n");
    printf("\t'Z' aumenta e 'X' diminui a intensidade de luz uniforme.\n");
    printf("\t'C' aumenta e 'V' diminui a intensidade de luz difusa.\n");
    printf("\t'B' aumenta e 'N' diminui a intensidade de luz especular.\n\n");
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Trabalho 3");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutTimerFunc(33, atualiza, 33);
    
	inicializa();
	imprimeInstrucoes();

    glutMainLoop();
    return 0;
}