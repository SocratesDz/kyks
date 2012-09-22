/*

Pong

Un juego que simula una partida de ping pong

Autor: Sócrates Díaz
Fecha: 21/09/2012
*/


/*********************** Incluyendo Allegro **************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/*********************** Inicializando Allegro y creando la pantalla ***********/


#define ScreenWidth 640
#define ScreenHeight 480

int main()
{
	ALLEGRO_DISPLAY *display;

	const float FPS = 60.0;

	if(!al_init())
	{
		al_show_native_message_box(NULL, "Fatal Error", NULL, "No se pudo inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	display = al_create_display(ScreenWidth, ScreenHeight);

	if(!display)
	{
		al_show_native_message_box(NULL, "Error", NULL, "No se pudo crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_set_window_position(display, 0, 0);

	/************************** Creamos algunas variables para el juego ************/

	al_install_keyboard();
	al_init_primitives_addon();

	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

	//bool done = false;
	
	struct Bola
	{
		float x;
		float y;
		float w;
		float h;
		ALLEGRO_COLOR color;
	}bola1;

	bola1.x = bola1.y = bola1.w = bola1.h = 10.0;
	bola1.color = white;


	/************************** Principio del juego *******************************/

	//while(!done)
	//{
		//al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(10, 10, 10, 10, al_map_rgb(255, 255, 255));
		al_flip_display();
		al_rest(5.0);
		//done = true;
	//}

	al_destroy_display(display);

	return 0;
}