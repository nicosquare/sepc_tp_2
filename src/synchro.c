#include "synchro.h"
#include "ensitheora.h"


bool fini;

/* les variables pour la synchro, ici */
pthread_t audio_id, video_id, draw2sdl_id;
pthread_mutex_t mutex_video, mutex_screen, mutex_texture, mutex_cons;

// Init CondVar
pthread_cond_t cond_screen = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_texture = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_cons = PTHREAD_COND_INITIALIZER; 
pthread_cond_t cond_prod = PTHREAD_COND_INITIALIZER; 

// Variables 
int texture_prete, nb_textures;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {

	pthread_mutex_lock(&mutex_screen);

	windowsx = buffer[0].width;
    windowsy = buffer[0].height;

    pthread_cond_signal(&cond_screen);

    pthread_mutex_unlock(&mutex_screen);
}

void attendreTailleFenetre() {

	pthread_mutex_lock(&mutex_screen);

	while(windowsy == 0 || windowsx == 0){
    	pthread_cond_wait(&cond_screen, &mutex_screen);
    }

    pthread_mutex_unlock(&mutex_screen);

}

void signalerFenetreEtTexturePrete() {

	pthread_mutex_lock(&mutex_texture);

	texture_prete = 1;

	pthread_cond_signal(&cond_texture);

    pthread_mutex_unlock(&mutex_texture);

}

void attendreFenetreTexture() {

	pthread_mutex_lock(&mutex_texture);

	while(texture_prete == 0){
    	pthread_cond_wait(&cond_texture, &mutex_texture);
    }

    pthread_mutex_unlock(&mutex_texture);

}

void debutConsommerTexture() {

	pthread_mutex_lock(&mutex_cons);

	while(nb_textures == 0){
    	pthread_cond_wait(&cond_cons, &mutex_cons);
    }

    pthread_mutex_unlock(&mutex_cons);

}

void finConsommerTexture() {

	pthread_mutex_lock(&mutex_cons);

	nb_textures--;

	pthread_cond_signal(&cond_prod);

    pthread_mutex_unlock(&mutex_cons);

}


void debutDeposerTexture() {

	pthread_mutex_lock(&mutex_cons);

	while(nb_textures == NBTEX){
    	pthread_cond_wait(&cond_prod, &mutex_cons);
    }

    pthread_mutex_unlock(&mutex_cons);

}

void finDeposerTexture() {

	pthread_mutex_lock(&mutex_cons);

	nb_textures++;

	pthread_cond_signal(&cond_cons);

    pthread_mutex_unlock(&mutex_cons);

}
