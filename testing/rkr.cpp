#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>

#include "data.h"


#define ScreenWidth 640
#define ScreenHeight 480


bool Collision(float x, float y, float w, float h, float ex, float ey, float ew, float eh);
bool PixelCol(ALLEGRO_BITMAP *player, ALLEGRO_BITMAP *enemy, float x, float y, float w, float h, float ex, float ey, float ew, float eh);

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

	// Pone la posición en la que debe salir l`a ventana
	//al_set_window_position(display, 0, 30);

	// Pone el título de la ventana
	al_set_window_title(display, "Killer Bunny");

	if(!display)	// Si no se pudo crear la ventana, entonces pone un mensaje de error
	{
		al_show_native_message_box(NULL, "Error", NULL, "No se pudo crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	// -----------------------------------------------------------------

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_KEYBOARD_STATE keyState;

	// Utilizado para debugging
	ALLEGRO_FONT *font = al_load_font("sprites/DroidSans.ttf", 10, 0);

	
	ALLEGRO_BITMAP *fondo1 = al_load_bitmap("sprites/fondo1.png");
	
	
	bool done = false;

	// ---------Estructuras del juego-----------------------------------

	struct Player player;
	player.image = al_load_bitmap("sprites/player.png");
	al_convert_mask_to_alpha(player.image, al_map_rgb(255,255,255));
	player.x = ScreenWidth / 2;
	player.y = ScreenHeight / 2;
	player.w = al_get_bitmap_width(player.image);
	player.h = al_get_bitmap_height(player.image);
	player.moveSpeed = 3;
	player.degrees = -ALLEGRO_PI/2;
   	player.alive = true;
   	player.clip = 6;
   	player.vida = 100;

	
	struct Bala bala;
	
	//~ void Bala::update()
	//~ {
		//~ bala.dx = cosf(player.xmouse - player.x);
		//~ bala.dy = senf(player.ymouse - player.y);
		//~ 
		//~ if(bala.shot){
			//~ 
			//~ bala.x += bala.dx;
			//~ bala.y += bala.dy;
			//~ }
		//~ 
	//~ }
	
	bala.image = al_load_bitmap("sprites/bullet.png");
	bala.x = player.x+50;
	bala.y = player.y+25;
	bala.shot = false;
	
	
	struct Enemigo robot;
	robot.image = al_load_bitmap("sprites/Robot_sprites.png");
	robot.death = al_load_bitmap("sprites/explosiondelrobot.png");
	al_convert_mask_to_alpha(robot.death, al_map_rgb(255, 255, 255));
	//al_convert_mask_to_alpha(robot.image, al_map_rgb(255,255,255));
	robot.x = 50;
	robot.y = 50;
	robot.w = al_get_bitmap_width(robot.image);
	robot.h = al_get_bitmap_height(robot.image);
	robot.velocidad_x = 0.23;
	robot.velocidad_y = 0.23;
	robot.fuerza= 0.5;
	robot.vida=50;
	
	//~ void Weapon::recargar()
	//~ {
		//~ for(int i = 0; i < 6; i++)
		//~ {
			//~ bullets[i] = bala;
		//~ }
	//~ }
	
	// -----------------------------------------------------------------

	// Esta variable guardará los eventos del mouse
	ALLEGRO_MOUSE_STATE mouseState;
	
	// Registro varias fuentes de eventos
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	// Inicializo el temporizador principal
	al_start_timer(timer);

	while(!done)
	{
		// La variable de los eventos
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev); // Y aquí espero por los eventos


		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			// Dos funciones para pasar eventos del mouse y del teclado
			al_get_keyboard_state(&keyState);
			al_get_mouse_state(&mouseState);
			
			// Esto detecta la posición del mouse y lo guarda a un par de variables
			player.xmouse = al_get_mouse_state_axis(&mouseState, 0);
			player.ymouse = al_get_mouse_state_axis(&mouseState, 1);

			// Si presiono Esc entonces me saca del juego
			if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
			{
				done = true;
			}
			// Si presiono A entonces el valor x se reduce, osea, se va a la izquierda
			if(al_key_down(&keyState, ALLEGRO_KEY_A))
			{
				player.x -= player.moveSpeed;
			}
			// Si... meh, ya sabes lo que sigue
			if(al_key_down(&keyState, ALLEGRO_KEY_D))
			{
				player.x += player.moveSpeed;
			}
			// ...
			if(al_key_down(&keyState, ALLEGRO_KEY_W))
			{
				player.y -= player.moveSpeed;
			}
			// ...
			if(al_key_down(&keyState, ALLEGRO_KEY_S))
			{
				player.y += player.moveSpeed;
			}
			// Mata al robot
			if(al_key_down(&keyState, ALLEGRO_KEY_K))
			{
                robot.vida -= 10;
            }
					
		}
		
		// Esto permite que el jugador se mueva con el mouse
		player.degrees = atan2((player.ymouse-player.y),(player.xmouse-player.x));
					
		// La Inteligencia Artificial del enemigo
		if(robot.alive && player.alive){
			if(robot.x < player.x) robot.x += robot.velocidad_x;
			if(robot.x > player.x) robot.x -= robot.velocidad_x;
			if(robot.y > player.y) robot.y -= robot.velocidad_y;
			if(robot.y < player.y) robot.y += robot.velocidad_y;
		}
		// Uso de las funciones para las colisiones
		//if(Collision(player.x, player.y, 50, 50, robot.x, robot.y, 34, 34)) player.alive = false;
		if(PixelCol(player.image, robot.image, player.x-(player.w/2), player.y-(player.h/2), player.w, player.h, robot.x, robot.y, robot.w/7, robot.h)) player.vida -= robot.fuerza;

		if(player.vida==0) player.alive = false;
		
		if(robot.vida<=0){
			robot.vida = 0;
			robot.alive = false;
		}

		al_clear_to_color(al_map_rgb(255, 255, 255));	// Se pinta todo a negro
		al_draw_scaled_bitmap(fondo1,0, 0, 256, 256, 0, 0, ScreenWidth, ScreenHeight, 0);	// Se dibuja el fondo
		if(player.alive){	// Si el jugador está vivo
			al_draw_rotated_bitmap(player.image, 25, 25, player.x, player.y, player.degrees, 0); // Dibujo el jugador
			al_draw_rotated_bitmap(bala.image, 0, 0, player.x+5, player.y+5, player.degrees, 0); // Dibujo la bala (esto hay que quitarlo)
		}
		if(robot.alive){
			al_draw_bitmap_region(robot.image, 0, 0, 60, 52, robot.x, robot.y, 0); // Dibujo el robot
			}
		else
		{
			robot.w = al_get_bitmap_width(robot.death);
			robot.h = al_get_bitmap_width(robot.death);
			al_draw_bitmap_region(robot.death, 0, 0, robot.w/4, robot.h, robot.x, robot.y, 0);
        }
        
        // Esto es para el debugging
        
        // Dibujo rectángulos para las colisiones
        al_draw_rectangle(player.x-(player.w/2), player.y-(player.h/2), (player.x+player.w)-(player.w/2), (player.y+player.h)-(player.h/2), al_map_rgb(0, 255, 0), 1.0);
        al_draw_rectangle(robot.x, robot.y, robot.x+(robot.w/7), robot.y+robot.h, al_map_rgb(0, 255, 0), 1.0);
        
        // Escribo en una esquina de la pantalla, información respecto al personaje
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 2, ALLEGRO_ALIGN_RIGHT, "Player x, y : %.1f %.1f", player.x, player.y);
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 12, ALLEGRO_ALIGN_RIGHT, "Rotation (rad): %.5f", player.degrees);
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 22, ALLEGRO_ALIGN_RIGHT, "Rotation (degrees): %.2f", (player.degrees*180)/ALLEGRO_PI);
		
		// Status bar
		al_draw_filled_rectangle(0,0,player.vida*2,15, al_map_rgb(0,255,0));
		
		// Actualizo la pantalla (flip)
		al_flip_display();
		
		
	}
	
	//-----After party (hay que limpiar)--------------------------------
	
	// A destruirlo todo!! BAM BAM BAM, KABOOM!!
	al_destroy_font(font);
	al_destroy_bitmap(fondo1);
	al_destroy_bitmap(robot.image);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(player.image);
	al_destroy_timer(timer);

	return 0;
}

// Funciones -----------------------------------------------------------

// Colision de cuadrados
bool Collision(float x, float y, float w, float h, float ex, float ey, float ew, float eh)
{
	 // Todavía estoy pensando sobre por qué esto no funcionó
     //~ if((x <= (ex+ew) && (y <= ey && y >= (ey+eh))) || ((x+w) >= ex && (y <= ey && y >= (ey + eh)))
	     //~ || (y <= (ey+eh) && (x >= ex && x <= (ex+ew))) || ((y+h) >= ey && (x >= ex && x <= (ex+ew))))
             //~ return true;
     //~ else
		//~ return false;
		
	if(x+w < ex || x > ex + ew || y + h < ey || y > ey+eh)
		return false;
	return true;
}


// Colision de píxel perfecto
bool PixelCol(ALLEGRO_BITMAP *player, ALLEGRO_BITMAP *enemy, float x, float y, float w, float h, float ex, float ey, float ew, float eh)
{
	// El valor más alto entre la parte de arriba del jugador y del enemigo
	float top = fmax(y, ey);
	// El valor más bajo entre la parte de abajo del jugador y del enemigo
	float bottom = fmin(y + h, ey + eh);
	// El valor más alto entre la parte izquierda del jugador y del enemigo
	float left = fmax(x, ex);
	// El valor más bajo entre la parte derecha del jugador y del enemigo
	float right = fmin(x + w, ex + ew);
	
	// Hace un par de for's para revisar si hace colisión
	for(int i = top; i < bottom; i++)
	{
		for(int j = left; j < right; j++)
		{
			al_lock_bitmap(player, al_get_bitmap_format(player), ALLEGRO_LOCK_READONLY);
			al_lock_bitmap(enemy, al_get_bitmap_format(enemy), ALLEGRO_LOCK_READONLY);
			ALLEGRO_COLOR color = al_get_pixel(player, j - x, i - y);
			ALLEGRO_COLOR color2 = al_get_pixel(enemy, j - ex, i - ey);
			
			// Si el color alpha del jugador es diferente de 0 y el color alpha del enemigo es diferente de 0,
			// y están en la misma posición, entonces detecta una colisión
			if(color.a != 0 && color2.a != 0)
				return true;
		}
	}
	
	// Pero de lo contrario, devuelve false
	return false;
}
