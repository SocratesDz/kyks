/*
2 - Creando una ventana (la forma correcta)
Basado en los tutoriales de Coding Made Easy.

Para el grupo KYKS

Nota: para compilar debe usarse la opción "-lallegro -lallegro_dialog" en el compilador.
Para más información lea el manual de Allegro 5, o pregunte en los issues
*/

#include <allegro5/allegro.h>	// Incluye la librería de Allegro
#include <allegro5/allegro_native_dialog.h>	// Incluye la librería de Allegro para crear diálogos

int main()
{

	// Creando un puntero que va a almacenar la ventana.
	// ALLEGRO_DISPLAY es un tipo de dato (obviamente).
	ALLEGRO_DISPLAY *display;

	// Inicializando Allegro
	if(!al_init())	// Si la inicialización falla entonces...
	{	
		// Muestra una ventana de error
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// Creando una ventana y poniéndole tamaño.
	display = al_create_display(800, 600);

	if(!display)	// Si no se pudo crear la ventana...
	{
		// Muestra una ventana de error
		al_show_native_message_box(display, "Sample Title", "Display Settings", "Display Window was not created successfully", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	// Esto destruye la ventana, es bueno hacerlo ya que esto limpia la memoria
	al_destroy_display(display);

	return 0;
}