/*
1 - Creando una ventana
Basado en los tutoriales de Coding Made Easy.

Para el grupo KYKS

*/

#include <allegro5/allegro.h>	// Incluye la librería de Allegro
#include <allegro5/allegro_native_dialog.h>	// Incluye la librería de Allegro para crear diálogos

int main(){

	// Creando un puntero que va a almacenar la ventana.
	// ALLEGRO_DISPLAY es un tipo de dato (obviamente).
	ALLEGRO_DISPLAY *display;

	// Inicializando Allegro
	if(!al_init())		// Si la inicialización falla entonces...
	{
		// Muestra una ventana de error
		al_show_native_message_box(NULL, NULL, NULL, "No se pudo inicializar Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	// Creando una ventana y poniéndole tamaño.
	display = al_create_display(800, 600);

	if(!display)	// Si no se pudo crear la ventana...
	{
		// Muestra una ventana de error
		al_show_native_message_box(NULL, NULL, NULL, "Could not create Allegro window", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	return 0;
}