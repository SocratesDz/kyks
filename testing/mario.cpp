/*

	Juego de prueba

Aqui definiremos las bases de nuestro juego real.

Autores: Sócrates Díaz, Yordy Aquiles

*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>


#define AnchoPantalla 640
#define AltoPantalla 480

int main()

{

	// Inicializando 

	const float FPS = 60.0;
	const float gravedad = 5.0;

	if(!al_init())
	{
		al_show_native_message_box(NULL, "Fatal Error", NULL, "No se pudo inicializar Allegro", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	al_set_new_display_flags(ALLEGRO_WINDOWED); // Pone la ventana en modo Windowed
	ALLEGRO_DISPLAY *display = al_create_display(AnchoPantalla, AltoPantalla);

	// Pone la posición en la que debe salir la ventana
	al_set_window_position(display, 0, 0);

	// Pone el título de la ventana
	al_set_window_title(display, "Muévete!");

	if(!display)	// Si no se pudo crear la ventana, entonces pone un mensaje de error
	{
		al_show_native_message_box(NULL, "Error", NULL, "No se pudo crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	// Fin de la inicialización

	al_install_keyboard();
	al_init_image_addon();

	ALLEGRO_BITMAP *image = al_load_bitmap("sprites/mario.png");
	ALLEGRO_BITMAP *mario = al_create_sub_bitmap(image, 3, 603, 16, 21);
	ALLEGRO_BITMAP *fondo = al_load_bitmap("sprites/jordy.jpg");
	ALLEGRO_BITMAP *malo  = al_load_bitmap ("sprites/enemigo.jpg");
	ALLEGRO_KEYBOARD_STATE estadoTeclado;
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE *listaEventos = al_create_event_queue();

	al_register_event_source(listaEventos, al_get_keyboard_event_source());
	al_register_event_source(listaEventos, al_get_display_event_source(display));
	al_register_event_source(listaEventos, al_get_timer_event_source(timer));


	// Ahora si comenzamos con lo del juego

	bool finJuego = false;
	float velocidad = 8.0, rotacion = 360.0;
	float x = AnchoPantalla/2, y = AltoPantalla/2;
	int right = 0, distancia_arma = 10;


	ALLEGRO_BITMAP *arma = al_load_bitmap("sprites/submachine.png");
	ALLEGRO_BITMAP *submachine = al_create_sub_bitmap(arma, 4, 4, 49, 21);
	al_convert_mask_to_alpha(mario, al_map_rgb(0,0,0));


	al_start_timer(timer);

	while(!finJuego)
	{
		ALLEGRO_EVENT eventos;

		al_wait_for_event(listaEventos, &eventos);


		if(eventos.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&estadoTeclado);

			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_Q) || al_key_down(&estadoTeclado, ALLEGRO_KEY_ESCAPE))
				finJuego = true;

			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_LEFT))
			{
				right = 0;
				distancia_arma = 10;
				rotacion = 0.0;
				x -= velocidad;
			}		
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_RIGHT))
			{
				right = 1;
				distancia_arma = 0;
				rotacion = 0.0;
				x += velocidad;
			}
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_UP))
				{
					rotacion = -90.0;
					y -= velocidad;
				}
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_DOWN))
				{
					rotacion = 90.0;
					y += velocidad;
				}

		if(x <= 0) x = 0;
		if(x+32 >= AnchoPantalla) x = AnchoPantalla-32;
		if(y <= 0) y = 0;
		y += gravedad;
		if(y+42 >= AltoPantalla) y = AltoPantalla-42;

		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_scaled_bitmap(fondo, 0, 0, 300, 290, 0, 0, AnchoPantalla, AltoPantalla, 0);
		al_draw_scaled_bitmap(mario, 0, 0, 16, 21, x, y, 32, 42, right);
		al_draw_bitmap(submachine, x - distancia_arma, y+15, right);
		al_draw_bitmap(malo, 10, 30, 0);

		al_flip_display();
	}

	al_destroy_bitmap(image);
	al_destroy_bitmap(mario);
	al_destroy_bitmap(arma);
	al_destroy_bitmap(submachine);
	al_destroy_event_queue(listaEventos);
	al_destroy_display(display);
	al_destroy_timer(timer);

	return 0;
}