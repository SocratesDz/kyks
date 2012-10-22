#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <stdlib.h>


#define ScreenWidth 640
#define ScreenHeight 480


bool Collision(float x, float y, float w, float h, float ex, float ey, float ew, float eh);
bool PixelCol(ALLEGRO_BITMAP player, ALLEGRO_BITMAP enemy, float x, float y, float w, float h, float ex, float ey, float ew, float eh);

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
	al_set_window_position(display, 0, 30);

	// Pone el título de la ventana
	al_set_window_title(display, "juego!");

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

	// -----------------------------------------------------------------

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	ALLEGRO_KEYBOARD_STATE keyState;

	// Utilizado para debugging
	ALLEGRO_FONT *font = al_load_font("sprites/DroidSans.ttf", 10, 0);

	
	ALLEGRO_BITMAP *fondo1 = al_load_bitmap("sprites/fondo1.png");
	
	
	bool done = false;

	// ---------Estructuras del juego-----------------------------------

	struct Player
	{
 		ALLEGRO_BITMAP *image;
		float x;
		float y;
		float moveSpeed;
		float degrees;
		bool alive;
		float xmouse;
		float ymouse;
	}player;
	
	player.image = al_load_bitmap("sprites/player.png");
	al_convert_mask_to_alpha(player.image, al_map_rgb(255,255,255));
	player.x = ScreenWidth / 2;
	player.y = ScreenHeight / 2;
	player.moveSpeed = 3;
	player.degrees = -ALLEGRO_PI/2;
   	player.alive = true;

	struct Bala
	{
	    ALLEGRO_BITMAP *image;
	    float x;
	    float y;

	}bala;
	
	
	bala.image = al_load_bitmap("sprites/bullet.png");
	bala.x = player.x+50;
	bala.y = player.y+25;
	
	struct Enemigo
	{
            ALLEGRO_BITMAP *image;
            float x;
            float y;
            float velocidad_x;
            float velocidad_y;
	}robot;

	robot.image = al_load_bitmap("sprites/enemigo.jpg");
	al_convert_mask_to_alpha(robot.image, al_map_rgb(255,255,255));
	robot.x = 50;
	robot.y = 50;
	robot.velocidad_x = 0.23;
	robot.velocidad_y = 0.23;
    
	// -----------------------------------------------------------------

	ALLEGRO_MOUSE_STATE mouseState;
	
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&keyState);
			al_get_mouse_state(&mouseState);
			
			player.xmouse = al_get_mouse_state_axis(&mouseState, 0);
			player.ymouse = al_get_mouse_state_axis(&mouseState, 1);

			if(al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
			{
				done = true;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_A))
			{
				player.x -= player.moveSpeed;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_D))
			{
				player.x += player.moveSpeed;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_W))
			{
				player.y -= player.moveSpeed;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_S))
			{
				player.y += player.moveSpeed;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_Q))
			{
				player.degrees-=0.1;
			}
			if(al_key_down(&keyState, ALLEGRO_KEY_E))
			{
				player.degrees+=0.1;
			}
					
		}
		
		player.degrees = atan2((player.ymouse-player.y),(player.xmouse-player.x));
				
		
 		if(robot.x < player.x) robot.x += robot.velocidad_x;
 		if(robot.x > player.x) robot.x -= robot.velocidad_x;
 		if(robot.y > player.y) robot.y -= robot.velocidad_y;
 		if(robot.y < player.y) robot.y += robot.velocidad_y;
		
		//if(Collision(player.x, player.y, 50, 50, robot.x, robot.y, 34, 34)) player.alive = false;
		if(PixelCol(player.image, robot.image, player.x, player.y, 50, 50, robot.x, robot.y, 34, 34)) player.alive = false;

		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(fondo1,0, 0, 256, 256, 0, 0, ScreenWidth, ScreenHeight, 0);
		if(player.alive){
			al_draw_rotated_bitmap(player.image, 25, 25, player.x, player.y, player.degrees, 0);
			al_draw_rotated_bitmap(bala.image, 0, 0, player.x+5, player.y+5, player.degrees, 0);
		}
		al_draw_bitmap(robot.image, robot.x, robot.y, 0);
        	
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 2, ALLEGRO_ALIGN_RIGHT, "Player x, y : %.1f %.1f", player.x, player.y);
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 12, ALLEGRO_ALIGN_RIGHT, "Rotation (rad): %.5f", player.degrees);
		al_draw_textf(font, al_map_rgb(255,255,255), ScreenWidth-10, 22, ALLEGRO_ALIGN_RIGHT, "Rotation (degrees): %.2f", (player.degrees*180)/ALLEGRO_PI);
		
		al_flip_display();
		
		
	}
	
	//-----After party (hay que limpiar)--------------------------------
	
	al_destroy_font(font);
	al_destroy_bitmap(fondo1);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(player.image);
	al_destroy_timer(timer);

	return 0;
}

// Funciones -----------------------------------------------------------

bool Collision(float x, float y, float w, float h, float ex, float ey, float ew, float eh)
{
     //~ if((x <= (ex+ew) && (y <= ey && y >= (ey+eh))) || ((x+w) >= ex && (y <= ey && y >= (ey + eh)))
	     //~ || (y <= (ey+eh) && (x >= ex && x <= (ex+ew))) || ((y+h) >= ey && (x >= ex && x <= (ex+ew))))
             //~ return true;
     //~ else
		//~ return false;
		
	if(x+w < ex || x > ex + ew || y + h < ey || y > ey+eh)
		return false;
	return true;
}

bool PixelCol(ALLEGRO_BITMAP *player, ALLEGRO_BITMAP *enemy, float x, float y, float w, float h, float ex, float ey, float ew, float eh)
{
	float top = fmax(y, ey);
	float bottom = fmin(y + h, ey + eh);
	float left = fmax(x, ex);
	float right = fmin(x + w, ex + ew);
	
	for(int i = top; i < bottom; i++)
	{
		for(int j = left; j < right; j++)
		{
			al_lock_bitmap(player, al_get_bitmap_format(player), ALLEGRO_LOCK_READONLY);
			al_lock_bitmap(enemy, al_get_bitmap_format(enemy), ALLEGRO_LOCK_READONLY);
			ALLEGRO_COLOR color = al_get_pixel(player, j - x, i - y);
			ALLEGRO_COLOR color2 = al_get_pixel(enemy, j - ex, i - ey);
			
			if(color.a != 0 && color2.a != 0)
				return true;
		}
	}
	return false;
}
