// Estructuras para el juego 
#include <cmath>

struct Player
{
 	ALLEGRO_BITMAP *image;
	float x;
	float y;
	float w;
	float h;
	float moveSpeed;
	float degrees;
	int clip;
	bool alive;
	float xmouse;
	float ymouse;
	float vida;
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
}bala;
	
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
		ALLEGRO_BITMAP *death;
        float x;
        float y;
        float velocidad_x;
        float velocidad_y;
        //float degrees;
        float w;
        float h;
        float vida;
        bool alive;
        float fuerza;
        
};

struct Weapon
	{
		struct Bala bullets[6];
		void recargar();
};
	
void Weapon::recargar()
{
	for(int i = 0; i < 6; i++)
	{
			bullets[i] = bala;
	}
}
