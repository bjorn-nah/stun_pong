#include "huc.h"
#include "sprite.h"

#define DIR_LEFT 1
#define DIR_RIGHT 2

#define DECAL 8
#define SLOW 4

#define POUSSE 2

#incspr(raquette,"raquette.pcx",0,0,2,6);
#incpal(raquettepal,"raquette.pcx");


#incspr(balle,"balle.pcx",0,0,1,1);
#incpal(ballepal,"balle.pcx");

#incchr(scene_chr,"stun_pong.pcx");
#incpal(scene_pal,"stun_pong.pcx");
#incbat(scene_bat,"stun_pong.pcx",0x1000,32,28);

int tics_p1, frame_p1, state_p1, tics_p2, frame_p2, state_p2, counter;
int j1, j2, p1_x, p1_y, p1_v, p2_x, p2_y, p2_v, balle_x, balle_y, balle_vect_x, balle_vect_y;
int point_p1, point_p2, play_state, max;
char dir_p1, dir_p2, dir_balle;

main()
{
	
	spr_make(0,16,96,0x5000,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,0,1);
	set_sprpal(0,raquettepal,1);
	spr_make(1,224,96,0x5000,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,1,1);
	set_sprpal(1,raquettepal,1);
	
	load_vram(0x5000,raquette,0x600);

	spr_make(2,104,120,0x5800,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,2,1);
	set_sprpal(2,ballepal,1);
	load_vram(0x5800,balle,0x100);

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
	balle_vect_x = 1;
	balle_vect_y = 1;
	dir_balle = DIR_RIGHT;

	point_p1 = 0;
	point_p2 = 0;
	play_state = 0;
	max = 15;


	set_font_color(1, 0);
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
		physique();
		
		
		put_number(point_p1, 2 , 6, 0);
		put_number(point_p2, 2 , 24 , 0);
		
		/*
		put_number(balle_vect_y, 4 , 5, 8);
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
		if (frame_p1 == 0) spr_pattern(0x5080);
		if (frame_p1 == 1) spr_pattern(0x5100);
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
	/*
	if(balle_vect_x < 0) dir_balle = DIR_LEFT;
	else dir_balle = DIR_RIGHT;
	*/
	
	p1_y -= p1_v;
	if (p1_y < 176) p1_v--;
	if (p1_y > 176) p1_y = 176;
	if (p1_y < 16)
	{
		p1_v = 0;
		p1_y = 16;
	}
	spr_put(0,p1_x,p1_y, dir_p1);

	p2_y -= p2_v;
	if (p2_y < 176) p2_v--;
	if (p2_y > 176) p2_y = 176;
	if (p2_y < 16)
	{
		p2_v = 0;
		p2_y = 16;
	}
	spr_put(1,p2_x,p2_y, dir_p2); 
	
	if(balle_x<1) 
	{
		balle_vect_x = -balle_vect_x;
		dir_balle = DIR_RIGHT;
		point_p2++;
	}
	if(balle_x>239)
	{
		balle_vect_x = -balle_vect_x;
		dir_balle = DIR_LEFT;
		point_p1++;
	}
	
	if(balle_y<16 && balle_vect_y < 0) balle_vect_y = -balle_vect_y;
	if(balle_y>192&& balle_vect_y > 0) balle_vect_y = -balle_vect_y;
	
	
	if(
		balle_x<30 && balle_x>20 &&
		dir_balle == DIR_LEFT && balle_vect_x<0 &&	
		(balle_y  - p1_y)>-16 && (balle_y - p1_y)<48
		)
	{
		balle_vect_x = -balle_vect_x;
		dir_balle = DIR_RIGHT;
		balle_vect_y = (balle_y + 8 - p1_y)/8 - 4;
		/*
		put_number(balle_y, 4 , 5, 9);
		put_number(p1_y, 4 , 5, 10);
		*/
	}
	
	if(
		balle_x<220 && balle_x>210 &&
		dir_balle == DIR_RIGHT && balle_vect_x>0 &&	
		(balle_y  - p2_y)>-16 && (balle_y - p2_y)<48
		)
	{
		balle_vect_x = -balle_vect_x;
		dir_balle = DIR_LEFT;
		balle_vect_y = (balle_y + 8 - p2_y)/8 -4;
	}
	

	
	balle_x += balle_vect_x;
	if(counter%2==0) balle_y += balle_vect_y;
	spr_put(2,balle_x,balle_y, DIR_RIGHT); 


	
}

