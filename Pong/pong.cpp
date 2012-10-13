/*

Pong

Un juego que simula una partida de ping pong

Autor: Sócrates Díaz
Fecha: 21/09/2012

Nota: Para compilar se debe utilizar como opción en el compilador
	 -lallegro -lallegro_dialog -lallegro_primitives
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
	
	al_set_new_display_flags(ALLEGRO_WINDOWED); // Pone la ventana en modo Windowed
	ALLEGRO_DISPLAY *display = al_create_display(ScreenWidth, ScreenHeight);

	// Pone la posición en la que debe salir la ventana
	al_set_window_position(display, 0, 0);

	// Pone el título de la ventana
	al_set_window_title(display, "Pong");

	if(!display)	// Si no se pudo crear la ventana, entonces pone un mensaje de error
	{
		al_show_native_message_box(NULL, "Error", NULL, "No se pudo crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	/************************** Creamos algunas variables para el juego ************/

	al_install_keyboard();	// Inicializa el teclado
	al_init_primitives_addon();	// Inicializa el addon de las formas primitivas

	// Creo una variable que almacene el color blanco
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

	// No me gusta realizar los siguientes pasos, me confunden
	
	// Crea una variable para almacenar el estado de las teclas
	ALLEGRO_KEYBOARD_STATE keyState;

	// Crea un temporizador que va a 60 FPS
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);

	// Crea una variable que almacena la lista de eventos
	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();

	// Registra la fuente de donde va a sacar los eventos de...
	al_register_event_source(eventQueue, al_get_keyboard_event_source());	// El teclado
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));	// El temporizador
	al_register_event_source(eventQueue, al_get_display_event_source(display));	// La pantalla
 
 	// De aquí en adelante no me confunde :)

 	// Crea una variable para el loop principal
	bool done = false;
	
	// Defino una estructura (próximamente será una clase)
	struct Bola
	{
		float x;
		float y;
		float radius;
		float moveSpeedX;
		float moveSpeedY;
		ALLEGRO_COLOR color;
	}bola;

	bola.x = ScreenWidth/2; // La ponemos a mitad de la pantalla
	bola.y = ScreenHeight/2;
	bola.radius = 5.0;	// La bola tiene un radio de 5 píxeles
	bola.color = white;	// Es color blanca
	bola.moveSpeedX = 5.0;	// Y se mueve a 5 píxeles por segundo
	bola.moveSpeedY = 5.0;

	// Hacemos lo anterior para hacer el jugador
	struct Palito
	{
		// Un rectángulo se dibuja

		float x1;	// desde el punto x1, y1 (esquina superior izquierda)
		float y1;	
		float x2;	// hasta el punto x2, y2 (esquina inferior derecha)
		float y2;
		float moveSpeedY; // defino la velocidad en el eje y
		ALLEGRO_COLOR color;
	}palito;

	palito.x1 = 15;
	palito.y1 = palito.x1;
	palito.x2 = 25;
	palito.y2 = 55;
	palito.moveSpeedY = 5.0;
	palito.color = white;


	/************************** Principio del juego *******************************/

	al_start_timer(timer);	// Pone el temporizador a correr

	while(!done)
	{
		// Aquí me vuelvo a confundir

		ALLEGRO_EVENT events;	// Crea una variable de eventos

		// Espera por un evento para mandarlo a la variable de eventos
		al_wait_for_event(eventQueue, &events);

		// Hasta aquí me confundo, pero después de esta línea no

		// Si el tipo de eventos es del temporizador de eventos
		// Lo que está dentro de este temporizador ocurre en tiempo real
		if(events.type == ALLEGRO_EVENT_TIMER)
		{
			// En esta condición es donde ocurre la acción

			// Obtiene el estado del teclado
			al_get_keyboard_state(&keyState);

			// Si se presiona la tecla ESCAPE
			if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
				done = true;	// entonces el programa se cierra

			// Si se presiona la tecla abajo
			if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
			{
				// Si la posición 'y' del jugador es menor que el ancho de la pantalla
				if(palito.y2 <= ScreenHeight)
				{
					// Entonces se mueve hacia abajo
					palito.y1 += palito.moveSpeedY;
					palito.y2 += palito.moveSpeedY;	
				}
					
			}

			// Si se presiona la tecla arriba
			if(al_key_down(&keyState, ALLEGRO_KEY_UP))
			{	
				// Si la posición 'y' del jugador es mayor que 0 (la parte superior de la pantalla)
				if(palito.y1 >= 0)
				{
					// Entonces se mueve hacia arriba
					palito.y1 -= palito.moveSpeedY;
					palito.y2 -= palito.moveSpeedY;	
				}
				
			}

			// La bola se mueve 'n' píxeles por segundo en el eje 'x' y 'y'
			bola.x += bola.moveSpeedX;
			bola.y += bola.moveSpeedY;

			// Si la bola toca los bordes de la pantalla, entonces su velocidad se vuelve negativa
			// por lo que su dirección se vuelve contraria
			if(bola.x == ScreenWidth || bola.x == 0) bola.moveSpeedX *= -1;
			if(bola.y == ScreenHeight || bola.y == 0) bola.moveSpeedY *= -1;
			
			// Vamos a detectar colisiones
			if(bola.y > palito.y1 && bola.y < palito.y2)
			{
				if(bola.x < palito.x1+palito.x2)
					bola.moveSpeedX *= -1;
			}
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));	// Pinto de negro la pantalla

		// Dibujo un círculo (quería un cuadrado pero no me funcionaba)
		al_draw_filled_circle(bola.x, bola.y, bola.radius, bola.color);

		// Dibujo un rectángulo (espero que esta vez funcione)
		al_draw_filled_rectangle(palito.x1, palito.y1, palito.x2, palito.y2, palito.color);

		// Copio todo lo dibujado a la pantalla
		al_flip_display();
	}

	// Recojo la basura
	al_destroy_display(display);
	al_destroy_event_queue(eventQueue);
	al_destroy_timer(timer);

	return 0;
}