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


/*********************** Inicializando Allegro y creando la pantalla ***********/


#define ScreenWidth 640
#define ScreenHeight 480

int main()
{

	const float FPS = 60.0;

	if(!al_init())
	{
		al_show_native_message_box(NULL, "Fatal Error", NULL, "No se pudo inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	ALLEGRO_DISPLAY *display = al_create_display(ScreenWidth, ScreenHeight);
	al_set_window_position(display, 0, 0);
	al_set_window_title(display, "Pong");

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
	
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_display_event_source(display));
 
	bool done = false;
	
	struct Bola
	{
		float x;
		float y;
		float radius;
		float moveSpeedX;
		float moveSpeedY;
		ALLEGRO_COLOR color;
	}bola;

	bola.x = ScreenWidth/2;
	bola.y = ScreenHeight/2;
	bola.radius = 5.0;
	bola.color = white;
	bola.moveSpeedX = 5.0;
	bola.moveSpeedY = 5.0;


	/************************** Principio del juego *******************************/

	al_start_timer(timer);

	while(!done)
	{

		ALLEGRO_EVENT events;
		al_wait_for_event(eventQueue, &events);

		if(events.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(events.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
					done = true;
			}

		}
		else if(events.type == ALLEGRO_EVENT_TIMER)
		{
			bola.x += bola.moveSpeedX;
			bola.y += bola.moveSpeedY;

			if(bola.x == ScreenWidth || bola.x == 0) bola.moveSpeedX *= -1;
			if(bola.y == ScreenHeight || bola.y == 0) bola.moveSpeedY *= -1;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_filled_circle(bola.x, bola.y, bola.radius, bola.color);
		al_flip_display();
	}

	al_destroy_display(display);
	al_destroy_event_queue(eventQueue);
	al_destroy_timer(timer);

	return 0;
}