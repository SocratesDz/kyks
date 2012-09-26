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
	ALLEGRO_KEYBOARD_STATE estadoTeclado;
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_EVENT_QUEUE *listaEventos = al_create_event_queue();

	al_register_event_source(listaEventos, al_get_keyboard_event_source());
	al_register_event_source(listaEventos, al_get_display_event_source(display));
	al_register_event_source(listaEventos, al_get_timer_event_source(timer));


	// Ahora si comenzamos con lo del juego

	bool finJuego = false;
	float velocidad = 8.0;
	float x = AnchoPantalla/2, y = AltoPantalla/2;
	int right = 0;

	al_start_timer(timer);

	while(!finJuego)
	{
		ALLEGRO_EVENT eventos;

		al_wait_for_event(listaEventos, &eventos);

		// Idea de yordy

	 	/*switch  (eventos.type){

			case ALLEGRO_EVENT_TIMER: 
				al_get_keyboard_state(&estadoTeclado);
				switch 

		}*/
		// Fin de idea de yordy

		if(eventos.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&estadoTeclado);

			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_Q) || al_key_down(&estadoTeclado, ALLEGRO_KEY_ESCAPE))
				finJuego = true;

			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_LEFT))
			{
				right = 0;
				x -= velocidad;
			}
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_RIGHT))
			{
				right = 1;
				x += velocidad;
			}
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_UP))
				y -= velocidad;
			if(al_key_down(&estadoTeclado, ALLEGRO_KEY_DOWN))
				y += velocidad;

		if(x <= 0) x = 0;
		if(x+16 >= AnchoPantalla) x = AnchoPantalla-16;
		if(y <= 0) y = 0;
		y += gravedad;
		if(y+21 >= AltoPantalla) y = AltoPantalla-21;

		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap_region(image, 3, 603, 16, 21, x, y, right);
		al_flip_display();
	}

	al_destroy_bitmap(image);
	al_destroy_event_queue(listaEventos);
	al_destroy_display(display);
	al_destroy_timer(timer);

	return 0;
}