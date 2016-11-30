#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "synchro.h"
#include "stream_common.h"
#include "oggstream.h"

int main(int argc, char *argv[]) {
	
    int res;
    void * status;

    // Init Mutex
	pthread_mutex_init(&mutex_video, NULL);
	pthread_mutex_init(&mutex_screen, NULL);
	pthread_mutex_init(&mutex_texture, NULL);
	pthread_mutex_init(&mutex_cons, NULL);

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE\n", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);
    
    // start the two stream readers

	printf("Prêt à lancer les threads\n");
	
	pthread_create(&audio_id, NULL, vorbisStreamReader, (void *) argv[1]);
	pthread_create(&video_id, NULL, theoraStreamReader, (void *) argv[1]);
	
    // wait audio thread
	pthread_join(audio_id, &status);
	if(status == 0) printf("Thread %lx completed!\n", audio_id);
    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(video_id);
    pthread_cancel(draw2sdl_id);

    // attendre les 2 threads videos
    pthread_join(video_id, &status);
	if(status == 0) printf("Thread %lx completed!\n", video_id);
	pthread_join(draw2sdl_id, &status);
	if(status == 0) printf("Thread %lx completed!\n", draw2sdl_id);
	
    exit(EXIT_SUCCESS);    
}
