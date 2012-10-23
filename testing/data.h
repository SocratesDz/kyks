// Estructuras para el juego 
#include <cmath>

struct Player
{
 	ALLEGRO_BITMAP *image;
	float x;
	float y;
	float moveSpeed;
	float degrees;
	int clip;
	bool alive;
	float xmouse;
	float ymouse;
};

struct Bala
{
	ALLEGRO_BITMAP *image;
	float x;
	float y;
	float dx;
	float dy;
	bool shot;
	void update();
};
	
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

struct Enemigo
	{
		ALLEGRO_BITMAP *image;
        float x;
        float y;
        float velocidad_x;
        float velocidad_y;
        //float degrees;
};

//~ struct Weapon
	//~ {
		//~ struct Bala bullets[player.clip];
		//~ void recargar();
//~ };
	
//~ void Weapon::recargar()
//~ {
	//~ for(int i = 0; i < player.clip; i++)
	//~ {
			//~ bullets[i] = bala;
	//~ }
//~ }
