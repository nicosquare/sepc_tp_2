#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdbool.h>
#include <pthread.h>
#include "ensitheora.h"

extern bool fini;


/* Les extern des variables pour la synchro ici */
extern pthread_t audio_id, video_id, draw2sdl_id;
extern pthread_mutex_t mutex_video, mutex_screen, mutex_texture, mutex_cons;
extern pthread_cond_t cond_screen, cond_texture, cond_cons, cond_prod;

extern int texture_prete, nb_textures;

/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
