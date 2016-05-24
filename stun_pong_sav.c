#include "huc.h"
#include "sprite.h"

#define DIR_LEFT 1
#define DIR_RIGHT 2

#define DECAL 8
#define SLOW 4

#define POUSSE 5

#incspr(raquette,"raquette.pcx",0,0,1,6);
#incpal(raquettepal,"raquette.pcx");


#incspr(balle,"balle.pcx",0,0,1,1);
#incpal(ballepal,"balle.pcx");

#incchr(scene_chr,"stun_pong.pcx");
#incpal(scene_pal,"stun_pong.pcx");
#incbat(scene_bat,"stun_pong.pcx",0x1000,32,28);

int tics_p1, frame_p1, state_p1, tics_p2, frame_p2, state_p2, counter;
int j1, j2, p1_x, p1_y, p1_v, p2_x, p2_y, p2_v, balle_x, balle_y, balle_vect_x, balle_vect_y;
int point_p1, point_p2, play_state, max;
char dir_p1, dir_p2;

main()
{
	
	spr_make(0,16,96,0x5000,FLIP_X_MASK|SIZE_MAS,FLIP_X|SZ_16x32,0,1);
	spr_make(1,224,96,0x5000,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,1,1);
	set_sprpal(0,raquettepal,1);
	load_vram(0x5000,raquette,0x600);

	spr_make(2,104,120,0x5800,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,2,1);
	set_sprpal(2,ballepal,1);
	load_vram(0x5800,ball,0x100);

	load_background(scene_chr,scene_pal,scene_bat,32,28);

	counter = 0;
	tics_p1 = 0;
	frame_p1 = 0;
	state_p1 = 0;
	tics_p2 = 0;
	frame_p2 = 0;
	state_p2 = 0;

	p1_x = 16;
	p1_y = 96;
	p1_v = 0;
	dir_p1 = DIR_RIGHT;
	
	p2_x = 224;
	p2_y = 96;
	p2_v = 0;
	dir_p2 = DIR_RIGHT;
	

	balle_x = 104;
	balle_y = 120;
	balle_vect_x = 0;
	balle_vect_y = 0;

	point_p1 = 0;
	point_p2 = 0;
	play_state = 0;
	max = 15;


	set_font_color(13, 0);
	load_default_font();

	put_number(point_p1, 2 , 6, 0);
	put_number(point_p2, 2 , 24 , 0);

	for(;;)
	{
		vsync();
		srand(counter);		/* re-randomise le random! */
		j1 = joy(0);
		j2 = joy(1);
		
		player_machine();

		
		/*
		put_number(counter, 5 , 5, 0);
		put_number(point_alice, 5 , 5, 1);
		put_number(point_lapin, 5 , 5, 2);
		put_number(ball_vect_x, 5 , 5, 3);
		put_number(ball_vect_y, 5 , 5, 4);
		put_number(play_state, 5 , 5, 5);
		*/
		counter++;
		satb_update();
	}
}

player_machine()
{
/*
	0 - Repos
	1 - J'suis une fusé!
*/

	spr_set(0);
	if ( state_p1 == 0) player_1_0();
	if ( state_p1 == 1) player_1_1();

	
	spr_set(1);
	if ( state_p2 == 0) player_2_0();
	if ( state_p2 == 1) player_2_1();
	
}

/* joueur 1 */
player_1_0()
{
	if ( tics_p1 == 0 ) spr_pattern(0x5000);
	tics_p1 = 1;
	if (j1 & JOY_A)
	{
		tics_p1 = 0;
		frame_p1 = 0;
		state_p1 = 1;
		spr_pattern(0x5100);
	}
}

player_1_1()
{
	tics_p1++;
	p1_v+= POUSSE;
	if (tics_p1 > 3)
	{
		tics_p1 = 0;
		frame_p1++;
		if (frame_p1 > 1) frame_p1 = 0;
		if (frame_p1 == 0) spr_pattern(0x5100);
		if (frame_p1 == 1) spr_pattern(0x5200);
	}

	if (j1 & JOY_A)
	{
		state_p1 = 1;
	}
	else
	{
		tics_p1 = 0;
		frame_p1 = 0;
		state_p1 = 0;
		spr_pattern(0x5000);	
	}
		
}

/* joueur 2 */
player_2_0()
{
	if ( tics_p2 == 0 ) spr_pattern(0x5000);
	tics_p2 = 1;
	if (j2 & JOY_A)
	{
		tics_p2 = 0;
		frame_p2 = 0;
		state_p2 = 1;
		spr_pattern(0x5100);
	}
}

player_2_1()
{
	tics_p2++;
	p2_v+= POUSSE;
	if (tics_p2 > 3)
	{
		tics_p2 = 0;
		frame_p2++;
		if (frame_p2 > 1) frame_p2 = 0;
		if (frame_p2 == 0) spr_pattern(0x5100);
		if (frame_p2 == 1) spr_pattern(0x5200);
	}

	if (j2 & JOY_A)
	{
		state_p2 = 1;
	}
	else
	{
		tics_p2 = 0;
		frame_p2 = 0;
		state_p2 = 0;
		state_p2 = 0;
		spr_pattern(0x5000);	
	}
		
}

physique()
{
	/* a coller en fonction? */
	p1_y -= p1_v
	if (p1_y < 176) p1_v--;
	if (p1_y > 176) p1_y = 176;
	spr_put(0,p1_x,p1_y, dir_p1);

	p2_y -= p2_v;
	if (p2_y < 121) p2_v--;
	if (p2_y > 121) p2_y = 176;
	spr_put(0,p2_x,p2_y, dir_p2);

}

