/*
  circuslinux.c
  
  Circus Linux!
  
  A clone of the Atari 2600 game "Circus Atari."
  (Similar to "Breakout" and "Arkanoid.")

  Use a teeter-totter to bounce clowns up in the air to pop balloons.
  
  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/
  
  December 11, 1999 - April 28, 2001
*/


/* Constraints: */

#define NUM_TITLE_BALLOONS 16
#define STARTING_LIVES 5
#define MAX_RECTS 512
#define FLYING_START_Y 192
#define FLYING_START_YM -8
#define BOUNCER_TIME 6
#define GRAVITY 1
#define FLYING_SPLAT_TIME 50
#define NUM_ROWS 3
#define NUM_BARRIERS 3
#define FASTEST_YM_OFF_BALLOON 8
#define MAX_YM 32
#define LIMB_ANIMATION_TIME 8
#define FPS (1000 / 33)
#define SHOW_PLAYER_TIME 100


/* Definitions: */

#define LEFT  0
#define RIGHT 1

#define GONE 0
#define NORMAL 1
#define POPPING0 2
#define POPPING1 2


/* #includes: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifdef LINUX
#include <pwd.h>
#include <sys/types.h>
#include <ctype.h>
#endif


/* Image enumerations: */

enum {
  IMG_TITLE,
  IMG_TITLE_HIGHLIGHTS,
  IMG_LIGHT_ON,
  IMG_LIGHT_OFF,
  IMG_PROGRAMMER,
  IMG_GRAPHICS,
  IMG_MUSIC,
  
  IMG_BACKGROUND_0,
  IMG_BACKGROUND_1,
  
  IMG_BALLOON_RED_LEFT_0,
  IMG_BALLOON_RED_LEFT_1,
  IMG_BALLOON_RED_RIGHT_0,
  IMG_BALLOON_RED_RIGHT_1,
  IMG_BALLOON_RED_DIE_0,
  IMG_BALLOON_RED_DIE_1,

  IMG_BALLOON_ORANGE_LEFT_0,
  IMG_BALLOON_ORANGE_LEFT_1,
  IMG_BALLOON_ORANGE_RIGHT_0,
  IMG_BALLOON_ORANGE_RIGHT_1,
  IMG_BALLOON_ORANGE_DIE_0,
  IMG_BALLOON_ORANGE_DIE_1,

  IMG_BALLOON_YELLOW_LEFT_0,
  IMG_BALLOON_YELLOW_LEFT_1,
  IMG_BALLOON_YELLOW_RIGHT_0,
  IMG_BALLOON_YELLOW_RIGHT_1,
  IMG_BALLOON_YELLOW_DIE_0,
  IMG_BALLOON_YELLOW_DIE_1,

  IMG_BALLOON_GREEN_LEFT_0,
  IMG_BALLOON_GREEN_LEFT_1,
  IMG_BALLOON_GREEN_RIGHT_0,
  IMG_BALLOON_GREEN_RIGHT_1,
  IMG_BALLOON_GREEN_DIE_0,
  IMG_BALLOON_GREEN_DIE_1,

  IMG_BALLOON_CYAN_LEFT_0,
  IMG_BALLOON_CYAN_LEFT_1,
  IMG_BALLOON_CYAN_RIGHT_0,
  IMG_BALLOON_CYAN_RIGHT_1,
  IMG_BALLOON_CYAN_DIE_0,
  IMG_BALLOON_CYAN_DIE_1,

  IMG_BALLOON_BLUE_LEFT_0,
  IMG_BALLOON_BLUE_LEFT_1,
  IMG_BALLOON_BLUE_RIGHT_0,
  IMG_BALLOON_BLUE_RIGHT_1,
  IMG_BALLOON_BLUE_DIE_0,
  IMG_BALLOON_BLUE_DIE_1,

  IMG_BALLOON_PURPLE_LEFT_0,
  IMG_BALLOON_PURPLE_LEFT_1,
  IMG_BALLOON_PURPLE_RIGHT_0,
  IMG_BALLOON_PURPLE_RIGHT_1,
  IMG_BALLOON_PURPLE_DIE_0,
  IMG_BALLOON_PURPLE_DIE_1,

  IMG_BALLOON_WHITE_LEFT_0,
  IMG_BALLOON_WHITE_LEFT_1,
  IMG_BALLOON_WHITE_RIGHT_0,
  IMG_BALLOON_WHITE_RIGHT_1,
  IMG_BALLOON_WHITE_DIE_0,
  IMG_BALLOON_WHITE_DIE_1,

  IMG_CLOWN_BODY_LEFT,
  IMG_CLOWN_BODY_RIGHT,
  
  IMG_CLOWN_BODY_UPSIDE_DOWN,
  
  IMG_CLOWN_LEFT_ARM_0,
  IMG_CLOWN_LEFT_ARM_1,
  IMG_CLOWN_LEFT_ARM_2,

  IMG_CLOWN_RIGHT_ARM_0,
  IMG_CLOWN_RIGHT_ARM_1,
  IMG_CLOWN_RIGHT_ARM_2,

  IMG_CLOWN_LEFT_LEG_0,
  IMG_CLOWN_LEFT_LEG_1,
  
  IMG_CLOWN_LEFT_LEG_0_UPSIDE_DOWN,
  IMG_CLOWN_LEFT_LEG_1_UPSIDE_DOWN,

  IMG_CLOWN_RIGHT_LEG_0,
  IMG_CLOWN_RIGHT_LEG_1,
  
  IMG_CLOWN_RIGHT_LEG_0_UPSIDE_DOWN,
  IMG_CLOWN_RIGHT_LEG_1_UPSIDE_DOWN,
  
  IMG_TEETER_TOTTER_LEFT_0,
  IMG_TEETER_TOTTER_LEFT_1,
  IMG_TEETER_TOTTER_LEFT_2,
  IMG_TEETER_TOTTER_LEFT_3,

  IMG_TEETER_TOTTER_RIGHT_0,
  IMG_TEETER_TOTTER_RIGHT_1,
  IMG_TEETER_TOTTER_RIGHT_2,
  IMG_TEETER_TOTTER_RIGHT_3,
  
  IMG_BOUNCER_0,
  IMG_BOUNCER_1,
  IMG_BARRIER,
  
  IMG_TIMES,
  IMG_NUMBERS_0,
  IMG_NUMBERS_1,
  IMG_LETTERS,
  IMG_FUZZ,
  IMG_CLOWN_HEAD,
  IMG_CLOWN_HEAD_OH,
  IMG_SADCLOWN_0,
  IMG_SADCLOWN_1,
  IMG_SADCLOWN_2,
  IMG_ENTER_INITIALS,
  
  IMG_HIGHSCORE_TOP,
  IMG_HIGHSCORE_LEFT,
  
  IMG_SEAL_0,
  IMG_SEAL_1,
  
  IMG_BEACHBALL_0,
  IMG_BEACHBALL_1,
  IMG_BEACHBALL_2,
  
  IMG_BEAR_RIGHT_0,
  IMG_BEAR_RIGHT_1,
  
  IMG_BEAR_LEFT_0,
  IMG_BEAR_LEFT_1,
  
  NUM_IMAGES
};


/* Image fileames: */

const char * image_names[NUM_IMAGES] = {
  DATA_PREFIX "images/title/title.png",
  DATA_PREFIX "images/title/title-highlights.png",
  DATA_PREFIX "images/title/light-on.png",
  DATA_PREFIX "images/title/light-off.png",
  DATA_PREFIX "images/title/programming.png",
  DATA_PREFIX "images/title/graphics.png",
  DATA_PREFIX "images/title/music.png",
  
  DATA_PREFIX "images/backgrounds/background0.png",
  DATA_PREFIX "images/backgrounds/background1.png",
  
  DATA_PREFIX "images/balloons/red-left-0.png",
  DATA_PREFIX "images/balloons/red-left-1.png",
  DATA_PREFIX "images/balloons/red-right-0.png",
  DATA_PREFIX "images/balloons/red-right-1.png",
  DATA_PREFIX "images/balloons/red-die-0.png",
  DATA_PREFIX "images/balloons/red-die-1.png",

  DATA_PREFIX "images/balloons/orange-left-0.png",
  DATA_PREFIX "images/balloons/orange-left-1.png",
  DATA_PREFIX "images/balloons/orange-right-0.png",
  DATA_PREFIX "images/balloons/orange-right-1.png",
  DATA_PREFIX "images/balloons/orange-die-0.png",
  DATA_PREFIX "images/balloons/orange-die-1.png",

  DATA_PREFIX "images/balloons/yellow-left-0.png",
  DATA_PREFIX "images/balloons/yellow-left-1.png",
  DATA_PREFIX "images/balloons/yellow-right-0.png",
  DATA_PREFIX "images/balloons/yellow-right-1.png",
  DATA_PREFIX "images/balloons/yellow-die-0.png",
  DATA_PREFIX "images/balloons/yellow-die-1.png",

  DATA_PREFIX "images/balloons/green-left-0.png",
  DATA_PREFIX "images/balloons/green-left-1.png",
  DATA_PREFIX "images/balloons/green-right-0.png",
  DATA_PREFIX "images/balloons/green-right-1.png",
  DATA_PREFIX "images/balloons/green-die-0.png",
  DATA_PREFIX "images/balloons/green-die-1.png",

  DATA_PREFIX "images/balloons/cyan-left-0.png",
  DATA_PREFIX "images/balloons/cyan-left-1.png",
  DATA_PREFIX "images/balloons/cyan-right-0.png",
  DATA_PREFIX "images/balloons/cyan-right-1.png",
  DATA_PREFIX "images/balloons/cyan-die-0.png",
  DATA_PREFIX "images/balloons/cyan-die-1.png",

  DATA_PREFIX "images/balloons/blue-left-0.png",
  DATA_PREFIX "images/balloons/blue-left-1.png",
  DATA_PREFIX "images/balloons/blue-right-0.png",
  DATA_PREFIX "images/balloons/blue-right-1.png",
  DATA_PREFIX "images/balloons/blue-die-0.png",
  DATA_PREFIX "images/balloons/blue-die-1.png",

  DATA_PREFIX "images/balloons/purple-left-0.png",
  DATA_PREFIX "images/balloons/purple-left-1.png",
  DATA_PREFIX "images/balloons/purple-right-0.png",
  DATA_PREFIX "images/balloons/purple-right-1.png",
  DATA_PREFIX "images/balloons/purple-die-0.png",
  DATA_PREFIX "images/balloons/purple-die-1.png",

  DATA_PREFIX "images/balloons/white-left-0.png",
  DATA_PREFIX "images/balloons/white-left-1.png",
  DATA_PREFIX "images/balloons/white-right-0.png",
  DATA_PREFIX "images/balloons/white-right-1.png",
  DATA_PREFIX "images/balloons/white-die-0.png",
  DATA_PREFIX "images/balloons/white-die-1.png",
  
  DATA_PREFIX "images/clowns/body-left.png",
  DATA_PREFIX "images/clowns/body-right.png",

  DATA_PREFIX "images/clowns/body-upside-down.png",
  
  DATA_PREFIX "images/clowns/left-arm-0.png",
  DATA_PREFIX "images/clowns/left-arm-1.png",
  DATA_PREFIX "images/clowns/left-arm-2.png",
  
  DATA_PREFIX "images/clowns/right-arm-0.png",
  DATA_PREFIX "images/clowns/right-arm-1.png",
  DATA_PREFIX "images/clowns/right-arm-2.png",
  
  DATA_PREFIX "images/clowns/left-leg-0.png",
  DATA_PREFIX "images/clowns/left-leg-1.png",
  
  DATA_PREFIX "images/clowns/left-leg-0-upside-down.png",
  DATA_PREFIX "images/clowns/left-leg-1-upside-down.png",
  
  DATA_PREFIX "images/clowns/right-leg-0.png",
  DATA_PREFIX "images/clowns/right-leg-1.png",

  DATA_PREFIX "images/clowns/right-leg-0-upside-down.png",
  DATA_PREFIX "images/clowns/right-leg-1-upside-down.png",
  
  DATA_PREFIX "images/teeter-totter/left-0.png",
  DATA_PREFIX "images/teeter-totter/left-1.png",
  DATA_PREFIX "images/teeter-totter/left-2.png",
  DATA_PREFIX "images/teeter-totter/left-3.png",

  DATA_PREFIX "images/teeter-totter/right-0.png",
  DATA_PREFIX "images/teeter-totter/right-1.png",
  DATA_PREFIX "images/teeter-totter/right-2.png",
  DATA_PREFIX "images/teeter-totter/right-3.png",

  DATA_PREFIX "images/bouncers/bouncer-0.png",
  DATA_PREFIX "images/bouncers/bouncer-1.png",
  DATA_PREFIX "images/bouncers/barrier.png",
  
  DATA_PREFIX "images/status/times.png",
  DATA_PREFIX "images/status/numbers-0.png",
  DATA_PREFIX "images/status/numbers-1.png",
  DATA_PREFIX "images/status/letters.png",
  DATA_PREFIX "images/status/fuzz.png",
  DATA_PREFIX "images/status/clown-head.png",
  DATA_PREFIX "images/status/clown-head-oh.png",
  DATA_PREFIX "images/status/sadclown-0.png",
  DATA_PREFIX "images/status/sadclown-1.png",
  DATA_PREFIX "images/status/sadclown-2.png",
  DATA_PREFIX "images/status/enter-initials.png",
  
  DATA_PREFIX "images/highscore/top.png",
  DATA_PREFIX "images/highscore/left.png",

  DATA_PREFIX "images/acts/seal-0.png",
  DATA_PREFIX "images/acts/seal-1.png",
  
  DATA_PREFIX "images/acts/beachball-0.png",
  DATA_PREFIX "images/acts/beachball-1.png",
  DATA_PREFIX "images/acts/beachball-2.png",

  DATA_PREFIX "images/acts/bear-right-0.png",
  DATA_PREFIX "images/acts/bear-right-1.png",
  DATA_PREFIX "images/acts/bear-left-0.png",
  DATA_PREFIX "images/acts/bear-left-1.png"
};


/* Bits of background that change: */

#define NUM_BACKGROUND_CHANGES 4

int background_change_rects[NUM_BACKGROUND_CHANGES][4] = {
  {424, 0, 88, 127},
  {256, 150, 153, 87},
  {26, 288, 57, 63},
  {580, 295, 44, 55}
};


/* Acts: */

enum {
  ACT_SEAL,
  ACT_BEAR,
  NUM_ACTS
};


/* Sound enumerations: */

enum {
  SND_POP,
  SND_BOUNCE,
  SND_TEETER1,
  SND_TEETER2,
  SND_SPLAT,
  SND_APPLAUSE,
  SND_CHEERING,
  SND_HIGHSCORE,
  SND_KEYPRESS,
  NUM_SOUNDS
};


/* Sound filenames: */

const char * sound_names[NUM_SOUNDS] = {
  DATA_PREFIX "sounds/pop.wav",
  DATA_PREFIX "sounds/bounce.wav",
  DATA_PREFIX "sounds/teeter1.wav",
  DATA_PREFIX "sounds/teeter2.wav",
  DATA_PREFIX "sounds/splat.wav",
  DATA_PREFIX "sounds/applause.wav",
  DATA_PREFIX "sounds/cheering.wav",
  DATA_PREFIX "sounds/wahoo.wav",
  DATA_PREFIX "sounds/keypress.wav"
};


/* Music: */

#define MUS_TITLE DATA_PREFIX "music/finally.mod"
#define MUS_GAME DATA_PREFIX "music/klovninarki.mod"
#define MUS_GAMEOVER DATA_PREFIX "music/kaupunki.mod"
#define MUS_HISCORE DATA_PREFIX "music/hiscore.mod"
#define MUS_HISCORESCREEN DATA_PREFIX "music/hiscreen.mod"


/* Local function prototypes: */

void setup(void);
void intro(void);
int title(void);
int game(void);
void erase(int x, int y, int w, int h, int bkgd);
void draw(int x, int y, int pict);
void drawclown(int x, int y, int side,
	       int left_arm, int right_arm,
	       int left_leg, int right_leg);
void clearrects(void);
void addrect(int x, int y, int w, int h);
void newclown(void);
void playsound(int snd);
void resetballoons(int player, int row);
void drawballoon(int player, int x, int y, int off);
void update_background(int which);
void drawnumber(int x, int y, int v, int img);
void drawtext(int x, int y, char * str);
void drawfuzz(int x, int y, int w, int h);
void seticon(void);
void usage(int ret);
FILE * open_option_file(char * mode);
void addscore(int player, int inc);
int highscorescreen(void);
int pausescreen(void);
void getinitials(void);


/* Global variables: */

int use_sound, use_fullscreen, use_low, use_joystick, use_joy_analog,
  use_paddle,
  num_rects, num_players, coop, barriers, bouncy, clearall, sfx_vol, music_vol,
  has_highscore, highscore_index, show_highscores, highscore_effect;
SDL_Surface * screen;
SDL_Surface * images[NUM_IMAGES];
SDL_Rect rects[MAX_RECTS];
#ifdef JOY_YES
SDL_Joystick * js;
#endif
int flying_active, flying_splat, flying_dir,
  flying_x, flying_y, flying_xm, flying_ym,
  flying_left_arm, flying_right_arm, flying_left_leg, flying_right_leg;
Uint8 balloons[2][NUM_ROWS][20];
int balloon_colors[2][NUM_ROWS];
int score[2], lives[2];
int highscore[8];
char highscorer[8][4];
char username_initials[3];
int mouse_grabbed;


#ifndef NOSOUND
Mix_Chunk * sounds[NUM_SOUNDS];
Mix_Music * mus_title, * mus_game, * mus_gameover, * mus_hiscore,
  * mus_hiscreen;
#endif


/* --- MAIN --- */

int main(int argc, char * argv[])
{
  int done, i;
  FILE * fi;
  char temp[512];

  
  /* Set program defaults: */
  
  use_sound = 1;
  use_joystick = 1;
  use_paddle = 0;
  use_joy_analog = 1;
  use_fullscreen = 0;
  use_low = 0;


  /* Check for arguments: */
  
  for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "--fullscreen") == 0 ||
	  strcmp(argv[i], "-f") == 0)
	use_fullscreen = 1;
      else if (strcmp(argv[i], "--digital") == 0 ||
	  strcmp(argv[i], "-d") == 0)
	use_joy_analog = 0;
      else if (strcmp(argv[i], "--paddle") == 0 ||
          strcmp(argv[i], "-p") == 0)
        use_paddle = 1;
      else if (strcmp(argv[i], "--low") == 0 ||
	       strcmp(argv[i], "-l") == 0)
	use_low = 1;
      else if (strcmp(argv[i], "--disable-sound") == 0 ||
	       strcmp(argv[i], "--nosound") == 0 ||
	       strcmp(argv[i], "-q") == 0)
	use_sound = 0;
      else if (strcmp(argv[i], "--help") == 0 ||
	       strcmp(argv[i], "-h") == 0)
	{
	  printf("\nCircus Linux!\n"
                 "\n"
		 "Version " VERSION "\n"
		 "New Breed Software, 2000\n"
                 "\n"
		 "Programming: Bill Kendrick\n"
		 "Graphics: Manu Parssinen and Bill Kendrick\n"
		 "Music: Jarkko Rotsten\n"
		 "\n"
		 "Game controls:\n"
		 "  Mouse Movement   - Move teeter-totter\n"
		 "  Any Mouse Button - Launch new clown /\n"
		 "                     Flip teeter-totter\n"
		 "\n"
		 "Run with \"--usage\" for command-line options...\n"
		 "Run with \"--copying\" for copying information...\n"
		 "\n");
	  
	  exit(0);
	}
      else if (strcmp(argv[i], "--copying") == 0 ||
	       strcmp(argv[i], "-c") == 0)
	{
	  printf("\nCircus Linux! is free software; you can redistrubut it\n"
		 "and/or modify it under the terms of the GNU General Public\n"
                 "License as published by the Free Software Foundation;\n"
                 "either version 2 of the License, or (at your option) any\n"
                 "later version.\n"
                 "\n"
                 "This program is distributed in the hope that it will be\n"
                 "useful and entertaning, but WITHOUT ANY WARRANTY; without\n"
                 "even the implied warranty of MERCHANTABILITY or FITNESS\n"
                 "FOR A PARTICULAR PURPOSE.  See the GNU General Public\n"
                 "License for more details.\n"
                 "\n"
                 "You should have received a copy of the GNU General Public\n"
                 "License along with this program; if not, write to the Free\n"
                 "Software Foundation, Inc., 59 Temple Place, Suite 330,\n"
                 "Boston, MA  02111-1307  USA\n"
                 "\n");
	  
	  exit(0);
	}
      else if (strcmp(argv[i], "--version") == 0 ||
	       strcmp(argv[i], "-v") == 0)
	{
	  printf("Circus Linux! version " VERSION "\n");
	  
	  exit(0);
	}
      else if (strcmp(argv[i], "--usage") == 0 ||
	       strcmp(argv[i], "-u") == 0)
	usage(0);
      else
	usage(1);
    }
  
  
  /* Set option defaults: */
  
  sfx_vol = 3;
  music_vol = 3;
  
  for (i = 0; i < 8; i++)
    {
      highscore[i] = 100;
      strcpy(highscorer[i], "TUX");
    }
  
  
  /* Load options: */
  
  fi = open_option_file("r");
  
  if (fi != NULL)
    {
      do
	{
	  fgets(temp, sizeof(temp), fi);
	  
	  if (!feof(fi))
	    {
	      temp[strlen(temp) - 1] = '\0';
	      
	      
	      /* Parse each line: */
	      
	      if (strstr(temp, "highscore") == temp &&
		  temp[9] >= '0' && temp[9] <= '7' &&
		  temp[10] == '=')
		{
		  highscore[temp[9] - '0'] = atoi(temp + 11);
		}
	      else if (strstr(temp, "highscorer") == temp &&
		  temp[10] >= '0' && temp[10] <= '7' &&
		  temp[11] == '=')
		{
		  highscorer[temp[10] - '0'][0] = temp[12];
		  highscorer[temp[10] - '0'][1] = temp[13];
		  highscorer[temp[10] - '0'][2] = temp[14];
		}
	      else if (strstr(temp, "effects=") == temp)
		{
		  sfx_vol = atoi(temp + 8);
		  if (sfx_vol > 3 || sfx_vol < 0)
		    sfx_vol = 3;
		}
	      else if (strstr(temp, "music=") == temp)
		{
		  music_vol = atoi(temp + 6);
		  if (music_vol > 3 || music_vol < 0)
		    music_vol = 3;
		}
	    }
	}
      while (!feof(fi));
      
      fclose(fi);
    }
  
  
  /* Setup: */
  
  setup();
  getinitials();
  
  
  /* Set gameplay defaults: */

  num_players = 1;
  coop = 0;
  barriers = 0;
  bouncy = 0;
  clearall = 0;
  
  
  /* --- MAIN FUNCTION LOOP: --- */
  
  done = 0;
  intro();
  
  do
    {
      show_highscores = 0;
      done = title();
      
      if (!done)
	{
	  if (show_highscores == 0)
	    done = game();
	  else
	    done = highscorescreen();
	}
    }
  while (!done);
  
  
  /* Save options: */
  
  fi = open_option_file("w");
  if (fi != NULL)
    {
      /* Comment at the top (I wish _everyone_ did this!) */
      
      fprintf(fi, "# Circus Linux! options file\n\n");
      
      
      /* High scores: */
      
      fprintf(fi, "# Highscores:\n\n");
      
      for (i = 0; i < 8; i++)
	{
	  fprintf(fi, "highscore%d=%d\n", i, highscore[i]);
	  fprintf(fi, "highscorer%d=%s\n\n", i, highscorer[i]);
	}
      
      fprintf(fi, "\n");
      
      
      /* Volume settings: */
      
      fprintf(fi,
	      "# Set \"effects\" and \"music\" to a value between 0 and 3.\n"
	      "# Where \"0\" is silent and \"3\" is maximum volume (loud).\n"
	      "# Where \"effects\" sets sound effects volumes, and\n"
	      "# where \"music\" sets music volume.\n\n"
	      "# (Default: 3 for both)\n\n");
      
      fprintf(fi, "effects=%d\n", sfx_vol);
      fprintf(fi, "music=%d\n\n\n", music_vol);
      
      
      /* The end! */
      
      fprintf(fi, "# (File automatically created.)\n");
      
      fclose(fi);
    }
  
  
  /* Quit and exit: */
  
  SDL_Quit();
  
  return(0);
}


/* Game: */

int game(void)
{
  int i, x, y, done, quit, frame, any, some, player, background_frame,
    teeter_x, teeter_xm, teeter_xmm, old_teeter_x, teeter_roll,
    teeter_side, teeter_sound, fire,
    show_player, mouse_x, mouse_y, len, act, act_x, act_y, act_xm, act_ym;
  int bouncers[2], barrier_x[3];
  SDL_Event event;
  Uint32 last_time, now_time;
  SDLKey key;
  SDL_Rect dest;
  Uint8 * keystate;
  Sint16 axis;
  
  
  /* Hide mouse pointer: */
  
  SDL_ShowCursor(0);
  
  
  /* Init: */
  
  key = SDLK_UNKNOWN;
  
  frame = 0;
  
  has_highscore = -1;
  highscore_effect = 0;
  
  teeter_side = LEFT;
  teeter_x = 0;
  teeter_xm = 0;
  teeter_xmm = 0;
  teeter_roll = 0;
  teeter_sound = 0;
  mouse_x = 0;
  mouse_y = 0;
  mouse_grabbed = 0;
  
  bouncers[0] = 0;
  bouncers[1] = 0;
  
  for (player = 0; player < 2; player++)
    {
      lives[player] = STARTING_LIVES;
      score[player] = 0;
      
      for (y = 0; y < NUM_ROWS; y++)
	resetballoons(player, y);
      
      for (y = 0; y < NUM_ROWS; y++)
	balloon_colors[player][y] = y * 2;
    }
  
  act = (rand() % NUM_ACTS);
  player = 0;
  flying_active = 0;
  flying_splat = 0;
  show_player = SHOW_PLAYER_TIME;
  background_frame = 0;
  highscore_index = (barriers * 4 + bouncy * 2 + clearall);
  
  act_x = 0;
  act_y = 0;
  act_xm = 0;
  act_ym = 0;
  
  for (i = 0; i < NUM_BARRIERS; i++)
    barrier_x[i] = i * 128;
  
  done = 0;
  quit = 0;
  
  
  /* Draw entire background: */
  
  if (use_low == 0)
    SDL_BlitSurface(images[IMG_BACKGROUND_0], NULL, screen, NULL);
  else
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 50, 50, 80));
  
  SDL_Flip(screen);
  
  
  /* --- MAIN GAME LOOP --- */
  
  do
    {
      last_time = SDL_GetTicks();
      frame++;
      
      
      /* Clear rectangles: */
      
      if (use_low == 0 || !(frame % 2))
	clearrects();
      
      
      /* Animate background: */
      
      if ((frame % 5) == 0 && use_low == 0)
	{
	  background_frame = (background_frame + 1) % 2;
	  update_background(background_frame);
	}
      
      
      /* Erase teeter-totter: */
      
      erase(teeter_x, 444, 96, 36, IMG_BACKGROUND_0 + background_frame);
      addrect(teeter_x, 444, 96, 36);
      
      
      /* Erase flying clown: */
      
      if (flying_active || flying_splat)
	{
	  erase(flying_x, flying_y, 32, 32,
		IMG_BACKGROUND_0 + background_frame);
	  addrect(flying_x, flying_y, 32, 32);
	}
      
      
      /* Erase bouncers: */
      
      for (i = 0; i < 2; i++)
	{
	  erase(608 * i, 448, 32, 32, IMG_BACKGROUND_0 + background_frame);
	  addrect(608 * i, 448, 32, 32);
	}

      
      /* Erase act: */
      
      erase(152, 347, 48, 48, IMG_BACKGROUND_0 + background_frame);
      addrect(152, 347, 48, 48);
      
      
      /* Erase balloons: */
      
      for (y = 0; y < NUM_ROWS; y++)
	erase(0, (y * 32) + 32, 640, 32, IMG_BACKGROUND_0 + background_frame);
      
      addrect(0, 32, 640, NUM_ROWS * 32);
      
      
      /* Erase barriers: */
      
      if (barriers)
	{
	  erase(0, (NUM_ROWS * 32) + 32, 640, 32,
		IMG_BACKGROUND_0 + background_frame);
	}
      
      
      /* Erase lives status: */
      
      erase(512, 0, 128, 32, IMG_BACKGROUND_0 + background_frame);
      addrect(512, 0, 128, 32);


      /* Erase score status: */
      
      erase(0, 0, 192, 32, IMG_BACKGROUND_0 + background_frame);
      addrect(0, 0, 192, 32);
      
      
      /* Keep track of old teeter-totter position: */
      
      old_teeter_x = teeter_x;
      
      
      /* Move teeter totter (digital joystick code) */
      
      teeter_x = teeter_x + teeter_xm;
      teeter_xm = teeter_xm + teeter_xmm;
      
      if (teeter_xm > 32)
	teeter_xm = 32;
      else if (teeter_xm < -32)
	teeter_xm = -32;
      
      if (teeter_x < 32)
	teeter_x = 32;
      else if (teeter_x > 512)
	teeter_x = 512;
      
      
      /* Handle events: */
      
      fire = 0;
      
      while (SDL_PollEvent(&event) > 0)
	{
	  /* Handle digital controls: */
	  
	  keystate = SDL_GetKeyState(NULL);
	  
	  axis = 0;
#ifdef JOY_YES
	  if (!use_joy_analog)
	    axis = SDL_JoystickGetAxis(js, 0);
#endif
	  
	  if (keystate[SDLK_LEFT] || axis < -256)
	    {
	      teeter_xmm = -2;
	      if (teeter_xm > 0)
		teeter_xm = 0;
	    }
	  else if (keystate[SDLK_RIGHT] || axis > 256)
	    {
	      teeter_xmm = 2;
	      if (teeter_xm < 0)
		teeter_xm = 0;
	    }
	  else
	    {
	      teeter_xmm = 0;
	      teeter_xm = 0;
	    }
	  
	  
	  /* Handle incoming events: */
	  
	  if (event.type == SDL_QUIT)
	    {
	      /* Quit request: */
	      
	      quit = 1;
	    }
          else if (event.type == SDL_KEYDOWN)
            {
              /* A keypress! */
              
              key = event.key.keysym.sym;
              
              if (key == SDLK_ESCAPE)
                {
                  /* Escape: Quit the game and return to main menu: */
                  
                  done = 1;
                }
	      else if (key == SDLK_SPACE || key == SDLK_TAB ||
		       key == SDLK_p)
		{
		  /* SPACE, TAB or P: Pause! */
		  
		  done = pausescreen();
		  if (done == 2)
		    {
		      done = 0;
		      quit = 1;
		    }
		}
	      else if (key == SDLK_LALT || key == SDLK_RALT ||
		       key == SDLK_LSHIFT || key == SDLK_RSHIFT ||
		       key == SDLK_RETURN)
		{
		  /* Fire button! */
		  
		  fire = 1;
		}
	      else if (key == SDLK_g)
		{
		  /* Toggle mouse grabbing: */
		  
		  if (mouse_grabbed == 0)
		    {
		      SDL_WM_GrabInput(SDL_GRAB_ON);
		      mouse_grabbed = 1;
		    }
		  else
		    {
		      SDL_WM_GrabInput(SDL_GRAB_OFF);
		      mouse_grabbed = 0;
		    }
		}
	    }
	  else if (event.type == SDL_MOUSEMOTION)
	    {
	      teeter_x = event.motion.x - 48;
	      mouse_x = event.motion.x;
	      mouse_y = event.motion.y;
	      
	      if (teeter_x < 32)
		teeter_x = 32;
	      else if (teeter_x > 512)
		teeter_x = 512;
	    }
#ifdef JOY_YES
	  else if (event.type == SDL_JOYAXISMOTION)
	    {
	      /* Joystick motion: */
	      
	      if (use_joy_analog)
		{
		  /* Analog: */
 
		  if (event.jaxis.axis == 0)
		    {
                      if (!use_paddle)
                      {
                        /* Regular controller: */

  		        teeter_x = ((event.jaxis.value + 32768) / 68) - 48;
                      }
                      else
                      {
                        /* Paddle controller: */

                        teeter_x = (-event.jaxis.value / 68) + 32;
                      }
		      
		      if (teeter_x < 32)
			teeter_x = 32;
		      else if (teeter_x > 512)
			teeter_x = 512;
		    }
		}
	    }
#endif
	  else if (event.type == SDL_MOUSEBUTTONDOWN
#ifdef JOY_YES
		   || event.type == SDL_JOYBUTTONDOWN
#endif
		   )
	    {
	      fire = 1;
	    }
	}

      
      /* Handle fire button: */
      
      if (fire)
	{
	  /* Swap teeter-totter side: */
	  
	  if (flying_active)
	    teeter_side = 1 - teeter_side;
	  
	  
	  /* Activate a new clown: */
	  
	  if (flying_active == 0 && flying_splat == 0)
	    {
	      /* Add the new clown: */
	      
	      newclown();
	      
		  
	      /* Erase "Player X Ready" message, if any: */
	      
	      if (show_player > 0)
		show_player = 1;
	    }
	}
      
      
      /* Handle highscore effect: */
      
      if (highscore_effect > 0)
	highscore_effect--;


      /* Handle the barrel: */
      
      if (teeter_x > old_teeter_x)
	{
	  teeter_roll = teeter_roll + 1;
	  if (teeter_roll > 3)
	    teeter_roll = 0;
	}
      else if (teeter_x < old_teeter_x)
	{
	  teeter_roll = teeter_roll - 1;
	  if (teeter_roll < 0)
	    teeter_roll = 3;
	}
      
      
      /* Handle bouncers: */
      
      for (i = 0; i < 2; i++)
	{
	  if (bouncers[i] > 0)
	    bouncers[i]--;
	}
      
      
      /* Handle barriers: */
      
      if (barriers)
	{
	  for (i = 0; i < NUM_BARRIERS; i++)
	    {
	      /* Move and wrap-around: */
	      
	      barrier_x[i] = barrier_x[i] + 8;
	      
	      if (barrier_x[i] >= 640)
		barrier_x[i] = -64;
	    }
	}
      
      
      /* Handle balloons: */
      
      any = 0;
      
      for (y = 0; y < NUM_ROWS; y++)
	{
	  /* Handle popping: */
	  
	  some = 0;
	  
	  for (x = 0; x < 20; x++)
	    {
	      if (balloons[player][y][x] == POPPING0)
		balloons[player][y][x] = GONE;
	      else if (balloons[player][y][x] > POPPING0)
		balloons[player][y][x]--;
	      
	      if (balloons[player][y][x] == NORMAL)
		{
		  some = 1;
		  any = 1;
		}
	    }
	  
	  
	  /* All balloons popped?  Add more? */
	  
	  if (some == 0 && flying_y > (NUM_ROWS * 32) + 64 &&
	      clearall == 0)
	    {
	      resetballoons(player, y);
	      balloon_colors[player][y]++;
	      
	      if (balloon_colors[player][y] > 7)
		balloon_colors[player][y] = 0;
	      
	      if (y == 0)
		{
		  addscore(player, 1000);
		  lives[player]++;
		  playsound(SND_CHEERING);
		}
	      else if (y == 1)
		{
		  addscore(player, 250);
		  playsound(SND_APPLAUSE);
		}
	      else if (y == 2)
		{
		  addscore(player, 100);
		  playsound(SND_APPLAUSE);
		}
	    }
	  
	  
	  /* Move balloons: */
	  
	  if ((frame % 4) == 0)
	    {
	      if ((y % 2) == 0)
		{
		  /* Left: */
		  
		  i = balloons[player][y][0];
		  
		  for (x = 0; x < 19; x++)
		    balloons[player][y][x] = balloons[player][y][x + 1];
		  
		  balloons[player][y][19] = i;
		}
	      else
		{
		  /* Right: */
		  
		  i = balloons[player][y][19];
		  
		  for (x = 18; x >= 0; x--)
		    balloons[player][y][x + 1] = balloons[player][y][x];
		  
		  balloons[player][y][0] = i;
		}
	    }
	}


      /* Reset all balloons? */
      
      if (clearall && !any && flying_y > (NUM_ROWS * 32) + 64)
	{
	  for (y = 0; y < 3; y++)
	    {
	      resetballoons(player, y);
	      balloon_colors[player][y]++;
	      
	      if (balloon_colors[player][y] > 7)
		balloon_colors[player][y] = 0;
	      
	      if (y == 0)
		{
		  addscore(player, 1000);
		  lives[player]++;
		  playsound(SND_CHEERING);
		}
	      else if (y == 1)
		{
		  addscore(player, 250);
		  playsound(SND_APPLAUSE);
		}
	      else if (y == 2)
		{
		  addscore(player, 100);
		  playsound(SND_APPLAUSE);
		}
	    }
	}
      
      
      /* Handle flying clown: */
      
      if (flying_active)
	{
	  /* Move the clown: */
	  
	  flying_x = flying_x + flying_xm;
	  flying_y = flying_y + flying_ym;
	  
	  
	  /* Bounce off top: */
	  
	  if (flying_y < 32)
	    {
	      flying_y = 32;
	      flying_ym = 0;
	    }
	  
	  
	  /* Bounce off bouncers: */
	  
	  if (flying_y > 416 && (flying_x < 32 || flying_x > 576))
	    {
	      flying_y = 416;
	      flying_ym = -abs(flying_ym);
	      
	      
	      /* Make bouncer squish and launch clown towards center: */
	      
	      if (flying_x < 32)
		{
		  bouncers[0] = BOUNCER_TIME;
		  flying_xm = 8;
		}
	      else
		{
		  bouncers[1] = BOUNCER_TIME;
		  flying_xm = -8;
		}
	      
	      
	      /* Give a point for bouncing: */
	      
	      addscore(player, 1);
	      
	      
	      /* Play bouncer sound: */
	      
	      playsound(SND_BOUNCE);
	    }
	  
	  
	  /* Bounce off barriers: */
	  
	  if (barriers)
	    {
	      for (i = 0; i < NUM_BARRIERS; i++)
		{
		  if (flying_y >= (NUM_ROWS * 32) &&
		      flying_y <= (NUM_ROWS * 32) + 64 &&
		      flying_x >= barrier_x[i] - 32 &&
		      flying_x <= barrier_x[i] + 64)
		    {
		      if (flying_y <= (NUM_ROWS * 32) + 32)
			{
			  flying_y = NUM_ROWS * 32;
			  flying_ym = -abs(flying_ym);
			}
		      else
			{
			  flying_y = (NUM_ROWS * 32) + 64;
			  flying_ym = abs(flying_ym);
			}
		      
		      playsound(SND_BOUNCE);
		    }
		}
	    }
	  
	  
	  /* Bounce off teeter-totter or splat: */
	  
	  if (flying_y > 448)
	    {
	      flying_y = 448;
	      
	      
	      /* Did we hit the teeter-totter? */
	      
	      if ((teeter_side == RIGHT &&
		   (flying_x >= teeter_x && flying_x <= teeter_x + 96)) ||
		  (teeter_side == LEFT &&
		   (flying_x >= teeter_x - 32 && flying_x <= teeter_x + 64)))
		{
		  /* Yes!  Bounce other the clown: */
		  
		  flying_ym = -(abs(flying_x - (teeter_x + 48 - 16)) / 3) - 16;
		  flying_y = 432;
		  
		  
		  if (teeter_side == LEFT)
		    {
		      flying_x = teeter_x + 64;
		      flying_dir = LEFT;
		      teeter_side = RIGHT;
		    }
		  else
		    {
		      flying_x = teeter_x;
		      flying_dir = RIGHT;
		      teeter_side = LEFT;
		    }
		  
		  
		  /* Randomly pick a X direction: */
		  
		  if ((rand() % 2) == 0)
		    {
		      if (flying_xm != 0)
			flying_xm = 0;
		      else
			flying_xm = -4;
		    }
		  
		  
		  /* Randomly change X direction (sign): */
		  
		  if ((rand() % 2) == 0)
		    {
		      flying_xm = -flying_xm;
		    }
		  
		  
		  /* Give a point for bouncing: */
		  
		  addscore(player, 1);
		  
		  
		  /* Play teeter-totter bounce sound: */
		  
		  playsound(SND_TEETER1 + teeter_sound);
		  teeter_sound = !teeter_sound;
		}
	      else
		{
		  /* No!  Splat the flying clown! */
		  
		  flying_active = 0;
		  flying_splat = FLYING_SPLAT_TIME;
		  lives[player]--;
		  playsound(SND_SPLAT);
		}
	    }
	  
	  
	  /* Bounce off balloons: */
	  
	  x = (flying_x + 16) / 32;
	  y = (flying_y / 32) - 1;

	  if ((frame / 2) % 2)
	    {
	      if ((y % 2) == 0)
		x = (flying_x / 32);
	      else
		x = (flying_x / 32) + 1;
	    }
	  
	  
	  if (y >= 0 && y < NUM_ROWS)
	    {
	      if (balloons[player][y][x] == NORMAL)
		{
		  balloons[player][y][x] = POPPING1;
		  playsound(SND_POP);
		  
		  
		  addscore(player, y + 1);
		  
		  
		  /* Bounce horizontally: */
		  
		  if ((flying_x % 32) < 16)
		    flying_xm = -4;
		  else
		    flying_xm = 4;
		  
		  
		  /* Bounce vertically: */
		  
		  if (bouncy == 1)
		    {
		      flying_ym = -flying_ym;
		      
		      if (flying_ym > FASTEST_YM_OFF_BALLOON)
			flying_ym = FASTEST_YM_OFF_BALLOON;
		    }
		}
	    }
	  
	  
	  /* Bounce off sides: */
	  
	  if (flying_x < 0)
	    {
	      flying_x = 0;
	      flying_xm = abs(flying_xm);
	    }
	  else if (flying_x > 608)
	    {
	      flying_x = 608;
	      flying_xm = -abs(flying_xm);
	    }
	  
	  
	  /* Deal with gravity: */
	  
	  flying_ym = flying_ym + GRAVITY;
	  
	  if (flying_ym > MAX_YM)
	    flying_ym = MAX_YM;
	  if (flying_ym < -MAX_YM)
	    flying_ym = -MAX_YM;
	}
      
      
      /* Count splats down: */
      
      if (flying_splat)
	{
	  flying_splat--;
	  
	  
	  /* If out of clowns, show game over while clown is splat: */
	  
	  if (lives[player] == 0)
	    {
	      if (num_players == 1)
		{
		  drawfuzz(176, 224, 288, 32);
		  drawtext(176, 224, "GAME OVER");
		}
	      else if (num_players == 2)
		{
		  drawfuzz(176, 192, 288, 96);
		  drawtext(224, 192, "PLAYER");
		  drawnumber(304, 224, player + 1, IMG_NUMBERS_0 + player);
		  drawtext(176, 256, "GAME OVER");
		}
	    }
	  
	  
	  if (flying_splat == 0)
	    {
	      /* Switch players: */
	      
	      if (num_players == 2)
		{
		  /* Copy balloons if in coop mode: */
		  
		  if (coop)
		    {
		      for (y = 0; y < NUM_ROWS; y++)
			{
			  for (x = 0; x < 20; x++)
			    {
			      balloons[1 - player][y][x] =
				balloons[player][y][x];
			    }
			}
		    }
		  
		  
		  /* Swap player: */
		  
		  player = 1 - player;
		  
		  if (lives[player] == 0)
		    {
		      player = 1 - player;
		      erase(0, 0, 640, 480,
			    IMG_BACKGROUND_0 + background_frame);
		      SDL_Flip(screen);
		    }
		}
	      
	      
	      /* Erase "Game over" display: */
	      
	      if (num_players == 1)
		erase(176, 224, 288, 32, IMG_BACKGROUND_0 + background_frame);
	      else
		erase(176, 192, 288, 96, IMG_BACKGROUND_0 + background_frame);

	      
	      /* Show which player is playing now: */

	      show_player = SHOW_PLAYER_TIME;


	      /* No more lives? */
	      
	      if (lives[player] == 0)
		{
		  done = 1;
		}
	    }
	}
      
      
      /* Change limb positions: */
	  
      if (flying_active || flying_splat)
	{
	  if ((frame % LIMB_ANIMATION_TIME) == 0)
	    {
	      flying_left_arm = rand() % 3;
	      flying_right_arm = rand() % 3;
	      flying_left_leg = rand() % 2;
	      flying_right_leg = rand() % 2;
	    }
	}
      
      
      /* Draw act: */
      
      if (!use_low)
	{
	  if (act == ACT_SEAL)
	    {
	      erase(148, act_y, 32, 32, IMG_BACKGROUND_0 + background_frame);
	      addrect(148, act_y, 32, 32);
	      
	      act_y = act_y + act_ym;
	      act_ym++;
	      
	      if (act_y >= 315)
		{
		  act_y = 315;
		  act_ym = -10;
		}
	      
	      draw(148, act_y, IMG_BEACHBALL_0 + ((frame / 4) % 3));
	      addrect(148, act_y, 32, 32);
	      
	      draw(152, 347, IMG_SEAL_0 + ((frame / 4) % 2));
	      addrect(152, 315, 48, 48);
	    }
	  else if (act == ACT_BEAR)
	    {
	      erase(act_x, 340, 48, 96, IMG_BACKGROUND_0 + background_frame);
	      addrect(act_x, 340, 48, 96);
	      
	      act_x = act_x + act_xm;
	      if (act_x <= 64)
		{
		  act_x = 64;
		  act_xm = 4;
		}
	      else if (act_x >= 524)
		{
		  act_x = 524;
		  act_xm = -4;
		}
	      
	      if (act_xm > 0)
		draw(act_x, 340, IMG_BEAR_RIGHT_0 + ((frame / 4) % 2));
	      else
		draw(act_x, 340, IMG_BEAR_LEFT_0 + ((frame / 4) % 2));
	      addrect(act_x, 340, 48, 96);
	    }
	}
      
      
      /* Draw balloons: */
      
      for (y = 0; y < NUM_ROWS; y++)
	{
	  for (x = 0; x < 20; x++)
	    {
	      drawballoon(player, x, y, ((frame / 2) % 2));
	    }
	}
      
      addrect(0, 32, 640, NUM_ROWS * 32);
      
      
      /* Draw barriers: */
      
      if (barriers)
	{
	  for (i = 0; i < 3; i++)
	    draw(barrier_x[i], (NUM_ROWS * 32) + 32, IMG_BARRIER);
	  
	  addrect(0, (NUM_ROWS * 32) + 32, 640, 32);
	}
      
      
      /* Draw teeter-totter: */
      
      if (teeter_side == LEFT)
	draw(teeter_x, 448, IMG_TEETER_TOTTER_LEFT_0 + teeter_roll);
      else if (teeter_side == RIGHT)
	draw(teeter_x, 448, IMG_TEETER_TOTTER_RIGHT_0 + teeter_roll);
      
      
      /* Draw clown on teeter-totter: */
      
      drawclown(teeter_x + 64 - (teeter_side * 64), 444, teeter_side,
		1, 1, 1, 1);
      
      addrect(teeter_x, 444, 96, 36);
      
      
      /* Draw flying clown: */
      
      if (flying_active)
	{
	  drawclown(flying_x, flying_y, flying_dir,
		    flying_left_arm, flying_right_arm,
		    flying_left_leg, flying_right_leg);
	  
	  addrect(flying_x, flying_y, 32, 32);
	}


      /* Draw splat clown: */
      
      if (flying_splat)
	{
	  drawclown(flying_x, flying_y, 2,
		    -1, -1,
		    flying_left_leg + 2, flying_right_leg + 2);
	  
	  addrect(flying_x, flying_y, 32, 32);
	}
      
      
      /* Draw bouncers: */
      
      for (i = 0; i < 2; i++)
	{
	  if (bouncers[i] == 0)
	    draw(608 * i, 448, IMG_BOUNCER_0);
	  else
	    draw(608 * i, 448, IMG_BOUNCER_1);
	  
	  addrect(608 * i, 448, 32, 32);
	}
      
      
      /* Draw lives status: */
      
      drawfuzz(512, 0, 128, 32);
      
      if (mouse_x < 500 || mouse_x > 556 || mouse_y > 44)
	{
	  /* Not near head, show normal clown face: */
	  
	  draw(512, 0, IMG_CLOWN_HEAD);
	}
      else
	{
	  /* Near head!  Show excited clown face: */
	  
	  draw(512, 0, IMG_CLOWN_HEAD_OH);
	}
      
      draw(544, 0, IMG_TIMES);
      addrect(512, 0, 64, 32);
      
      drawnumber(576, 0, lives[player], IMG_NUMBERS_0 + player);
      
      
      /* Draw score status: */
      
      drawfuzz(0, 0, 192, 32);
      drawnumber(0, 0, score[player], IMG_NUMBERS_0 + player);
      
      
      /* Draw "Player X Ready" message: */
      
      if (show_player > 0 && lives[player] > 0)
	{
	  if (num_players == 1)
	    {
	      drawfuzz(240, 224, 160, 32);
	      drawtext(240, 224, "READY");
	    }
	  else if (num_players == 2)
	    {
	      drawfuzz(224, 192, 192, 96);
	      drawtext(224, 192, "PLAYER");
	      drawnumber(304, 224, player + 1, IMG_NUMBERS_0 + player);
	      drawtext(240, 256, "READY");
	    }

	  
	  /* Count timer down: */
	  
	  show_player--;
	  
	  
	  /* Erase "Player X Ready" message: */
	  
	  if (show_player == 0)
	    {
	      if (num_players == 1)
		{
		  erase(240, 224, 160, 32,
			IMG_BACKGROUND_0 + background_frame);
		  addrect(240, 224, 160, 32);
		}
	      else if (num_players == 2)
		{
		  erase(224, 192, 192, 96,
			IMG_BACKGROUND_0 + background_frame);
		  addrect(224, 192, 192, 96);
		}
	    }
	}
      
      
      /* Update the screen: */
      
      if (use_low == 0 || (frame % 2))
	SDL_UpdateRects(screen, num_rects, rects);
      
      
      /* Pause: */
      
      now_time = SDL_GetTicks();
      if (now_time < last_time + FPS)
	SDL_Delay(last_time + FPS - now_time);
      
      
      /* Keep playing music: */
      
#ifndef NOSOUND
      if (use_sound == 1)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_game, 0);
	      Mix_VolumeMusic((music_vol * MIX_MAX_VOLUME) / 3);
	    }
	}
#endif
    }
  while (done == 0 && quit == 0);

  
  /* Ungrab mouse: */
  
  if (mouse_grabbed == 1)
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  
  
  /* Stop music and sounds: */
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      Mix_HaltMusic();
      Mix_HaltChannel(-1);
    }
#endif
  

  /* Show scores: */
  
  if (quit == 0)
    {
      /* Darken screen: */
      
      drawfuzz(0, 0, 640, 480);
      SDL_Flip(screen);
      SDL_Delay(300);
      
      
      /* Blank screen: */
      
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
      
      
      /* Draw "Final Score(s)" Text: */
      
      if (num_players == 1)
	drawtext(144, 144, "FINAL SCORE");
      else
	drawtext(128, 144, "FINAL SCORES");
      
      
      /* Show players' scores: */
      
      if (num_players == 1)
	{
	  drawnumber(224, 272, score[0], IMG_NUMBERS_0);
	  
	  if (has_highscore == 0)
	    {
	      drawtext(208, 0, "HISCORE");
	      draw(207, 64, IMG_ENTER_INITIALS);
	    }
	}
      else
	{
	  /* Show player 1's score: */
	  
	  drawtext(0, 208, "PLAYER");
	  drawtext(208, 208, "ONE");
	  drawnumber(0, 272, score[0], IMG_NUMBERS_0);
	  
	  if (has_highscore == 0)
	    {
	      drawtext(0, 0, "HISCORE");
	      draw(0, 64, IMG_ENTER_INITIALS);
	    }
	  
	  
	  /* Show player 2's score: */
	  
	  drawtext(336, 208, "PLAYER");
	  drawtext(544, 208, "TWO");
	  drawnumber(336, 272, score[1], IMG_NUMBERS_1);
	  
	  if (has_highscore == 1)
	    {
	      drawtext(416, 0, "HISCORE");
	      draw(415, 64, IMG_ENTER_INITIALS);
	    }
	}
      
      
      /* Did someone get a high score?  Set high score initials to null*/
      
      if (has_highscore != -1)
	strcpy(highscorer[highscore_index], "");
      len = 0;
      
      
      SDL_Flip(screen);
      
      done = 0;
      
      
      /* Wait for something: */
      
      do
	{
	  frame++;
	  
	  while (SDL_PollEvent(&event) > 0)
	    {
	      if (event.type == SDL_QUIT)
		{
		  /* Quit request: done and quit! */
		  
		  done = 1;
		  quit = 1;
		}
	      else if (event.type == SDL_KEYDOWN)
		{
		  /* A keypress! */
		  
		  key = event.key.keysym.sym;
		  
		  if (key == SDLK_ESCAPE)
		    {
		      /* Escape: done: */
		      
		      done = 1;
		    }
		  
		  
		  /* Does someone have high score? Accept input! */
		  
		  if (has_highscore != -1)
		    {
		      if (key >= SDLK_a && key <= SDLK_z && len < 3)
			{
			  highscorer[highscore_index][len] = ((key - SDLK_a) +
							      'A');
			  len++;
			  highscorer[highscore_index][len] = '\0';
			  
			  playsound(SND_KEYPRESS);
			}
		      else if ((key == SDLK_BACKSPACE ||
				key == SDLK_DELETE) &&
			       len > 0)
			{
			  len--;
			  highscorer[highscore_index][len] = '\0';
			  playsound(SND_POP);
			}
		      else if (key == SDLK_RETURN)
			{
			  done = 1;
			  playsound(SND_HIGHSCORE);
			}
		      
		      
		      /* Update text: */
		      
		      if (has_highscore == 0)
			{
			  if (num_players == 1)
			    {
			      /* Erase: */
			      
			      dest.x = 272;
			      dest.y = 32;
			      dest.w = 96;
			      dest.h = 32;
			      
			      SDL_FillRect(screen, &dest,
					   SDL_MapRGB(screen->format,
						      0, 0, 0));
			      
			      
			      /* Redraw: */
			      
			      drawtext(272, 32, highscorer[highscore_index]);
			      
			      
			      /* Update: */
			      
			      SDL_UpdateRect(screen, 272, 32, 96, 32);
			    }
			  else
			    {
			      /* Erase: */
			      
			      dest.x = 0;
			      dest.y = 32;
			      dest.w = 96;
			      dest.h = 32;
			      
			      SDL_FillRect(screen, &dest,
					   SDL_MapRGB(screen->format,
						      0, 0, 0));
			      
			      
			      /* Redraw: */
			      
			      drawtext(0, 32, highscorer[highscore_index]);
			      
			      
			      /* Update: */
			      
			      SDL_UpdateRect(screen, 0, 32, 96, 32);
			    }
			}
		      else
			{
			  /* Erase: */
			  
			  dest.x = 544;
			  dest.y = 32;
			  dest.w = 96;
			  dest.h = 32;
			  
			  SDL_FillRect(screen, &dest,
				       SDL_MapRGB(screen->format, 0, 0, 0));
			  
			  
			  /* Redraw: */
			  
			  drawtext(544, 32, highscorer[highscore_index]);
			  
			  
			  /* Update: */
			  
			  SDL_UpdateRect(screen, 544, 32, 96, 32);
			}
		    }
		}
	      else if (event.type == SDL_MOUSEBUTTONDOWN &&
		       has_highscore == -1)
		{
		  /* Click - done: */
		  
		  done = 1;
		}
	    }
	  
	  
	  /* Draw sad clown: */
	  
	  draw(512, 320, IMG_SADCLOWN_0 + (frame / 5) % 3);
	  SDL_UpdateRect(screen, 512, 320, 128, 160);
	  
	  
	  /* Play game-over or high-score music: */
	  
#ifndef NOSOUND
	  if (use_sound == 1)
	    {
	      if (!Mix_PlayingMusic())
		{
		  if (has_highscore == -1)
		    Mix_PlayMusic(mus_gameover, 0);
		  else
		    Mix_PlayMusic(mus_hiscore, 0);
		  
		  Mix_VolumeMusic((music_vol * MIX_MAX_VOLUME) / 3);
		}
	    }
#endif
	  
	  
	  /* Pause: */
	  
	  SDL_Delay(30);
	}
      while (done == 0);

      
      /* Did a name get entered? */
      
      if (has_highscore != -1)
	{
	  if (highscorer[highscore_index][0] == '\0')
	    strcpy(highscorer[highscore_index], username_initials);
	}
      
      /* Silence music: */
      
#ifndef NOSOUND
      if (use_sound == 1)
	Mix_HaltMusic();
#endif
    }
  
  
  /* Back to main!: */
  
  return(quit);
}


/* Intro screen: */

void intro(void)
{
  int i;
  
  for (i = 0; i < 50; i++)
    {
      if (i == 5)
	{
	  drawtext(32, 176, "NEW BREED SOFTWARE");
	  SDL_UpdateRect(screen, 0, 176, 640, 32);
	}
      else if (i == 25)
	{
	  drawtext(192, 288, "PRESENTS");
	  SDL_UpdateRect(screen, 0, 288, 640, 32);
	}
      
      SDL_Delay(30);
    }
}


/* Title screen: */

int title(void)
{
  SDL_Event event;
  int done, quit, i, j, frame, tmp, text_x, text_xm, text_img, text_time,
    highlight_x, highlight_y, highlight_w, highlight_h,
    old_highlight_x, old_highlight_y, old_highlight_w, old_highlight_h;
  int x[NUM_TITLE_BALLOONS], y[NUM_TITLE_BALLOONS],
    xm[NUM_TITLE_BALLOONS], ym[NUM_TITLE_BALLOONS],
    color[NUM_TITLE_BALLOONS], bumped[NUM_TITLE_BALLOONS];
  SDL_Rect src, dest;
  SDLKey key;
  

  /* Show mouse pointer: */
  
  SDL_ShowCursor(1);
  
  
  /* Draw background: */

  SDL_BlitSurface(images[IMG_TITLE], NULL, screen, NULL);
  SDL_Flip(screen);
  
  
  /* Reset highlight info: */
  
  highlight_x = -1;
  highlight_y = -1;
  highlight_w = -1;
  highlight_h = -1;

  old_highlight_x = -1;
  old_highlight_y = -1;
  old_highlight_w = -1;
  old_highlight_h = -1;
  
  
  /* Set balloon positions, directions and colors: */
  
  for (i = 0; i < NUM_TITLE_BALLOONS; i++)
    {
      x[i] = rand() % (640 - 32);
      y[i] = rand() % (480 - 32);
      
      xm[i] = (rand() % 5) + 1;
      
      if ((rand() % 2) == 0)
	xm[i] = -xm[i];
      
      ym[i] = (rand() % 5) + 1;

      if ((rand() % 2) == 0)
	ym[i] = -ym[i];
      
      color[i] = ((rand() % 8) * 6) + IMG_BALLOON_RED_LEFT_0;
      bumped[i] = 0;
    }
  
  highscore_effect = 0;
  
  
  /* --- MAIN TITLE LOOP --- */
  
  done = 0;
  show_highscores = 0;
  quit = 0;
  frame = 0;
  
  text_x = -640;
  text_xm = 36;
  text_img = 0;
  text_time = 0;
  
  do
    {
      frame++;
      
      
      /* Handle events: */
      
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* Quit event!  */
	      
	      quit = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* A keypress! */
	      
	      key = event.key.keysym.sym;
	      
	      if (key == SDLK_ESCAPE)
		{
		  /* Escape: done: */
		  
		  quit = 1;
		}
	    }
	  else if (event.type == SDL_MOUSEBUTTONDOWN)
	    {
	      /* Click: */
	      
	      if (event.button.x >= 16 && event.button.x <= 16 + 238 &&
		  event.button.y >= 283 && event.button.y <= 283 + 27)
		{
		  /* Start one-player game: */
		  
		  num_players = 1;
		  done = 1;
		}	
	      else if (event.button.x >= 16 && event.button.x <= 16 + 264 &&
		  event.button.y >= 310 && event.button.y <= 310 + 27)
		{
		  /* Start two-player game: */
		  
		  num_players = 2;
		  coop = 0;
		  done = 1;
		}
	      else if (event.button.x >= 16 && event.button.x <= 16 + 356 &&
		  event.button.y >= 337 && event.button.y <= 337 + 27)
		{
		  /* Start two-player coop game: */
		  
		  num_players = 2;
		  coop = 1;
		  done = 1;
		}
	      else if (event.button.x <= 207 &&
		  event.button.y >= 371 && event.button.y <= 371 + 27)
		{
		  /* Toggle Barriers: */
		  
		  barriers = !barriers;
		  playsound(SND_TEETER2 - barriers);
		}
	      else if (event.button.x <= 374 &&
		  event.button.y >= 398 && event.button.y <= 398 + 27)
		{
		  /* Toggle Bouncy Balloons: */
		  
		  bouncy = !bouncy;
		  playsound(SND_TEETER2 - bouncy);
		}
	      else if (event.button.x <= 234 &&
		  event.button.y >= 425 && event.button.y <= 425 + 27)
		{
		  /* Toggle Clear-All Mode: */
		  
		  clearall = !clearall;
		  playsound(SND_TEETER2 - clearall);
		}
	      else if (event.button.x >= 559 && event.button.x <= 559 + 73 &&
		       event.button.y >= 284 && event.button.y <= 284 + 52)
		{
		  /* Set SFX Volume: */
		  
		  sfx_vol = (sfx_vol + 1) % 4;
		  
#ifndef NOSOUND
		  if (use_sound)
		    Mix_Volume(-1, (sfx_vol * MIX_MAX_VOLUME) / 3);
#endif
		  
		  playsound(SND_POP);
		}
	      else if (event.button.x >= 512 && event.button.x <= 512 + 121 &&
		       event.button.y >= 336 && event.button.y <= 336 + 52)
		{
		  /* Set Music Volume: */
		  
		  music_vol = (music_vol + 1) % 4;
		  
#ifndef NOSOUND
		  if (use_sound)
		    Mix_VolumeMusic((music_vol * MIX_MAX_VOLUME) / 3);
#endif
		  
		  playsound(SND_POP);
		}
	      else if (event.button.x >= 440 && event.button.x <= 440 + 195 &&
		       event.button.y >= 398 && event.button.y <= 398 + 29)
		{
		  /* High score screen: */
		  
		  playsound(SND_HIGHSCORE);
		  
		  done = 1;
		  show_highscores = 1;
		}
	      else if (event.button.x >= 535 && event.button.x <= 535 + 100 &&
		       event.button.y >= 429 && event.button.y <= 429 + 29)
		{
		  /* Exit: */
		  
		  quit = 1;
		}
	    }
	  else if (event.type == SDL_MOUSEMOTION)
	    {
	      /* Mouse motion.  Highlight options: */
	      
	      if (event.motion.x >= 16 && event.motion.x <= 16 + 238 &&
		  event.motion.y >= 283 && event.motion.y <= 283 + 27)
		{
		  /* Start one-player game: */
		  
		  highlight_x = 16;
		  highlight_y = 283;
		  highlight_w = 238;
		  highlight_h = 27;
		}	
	      else if (event.motion.x >= 16 && event.motion.x <= 16 + 264 &&
		  event.motion.y >= 310 && event.motion.y <= 310 + 27)
		{
		  /* Start two-player game: */
		  
		  highlight_x = 16;
		  highlight_y = 310;
		  highlight_w = 264;
		  highlight_h = 27;
		}
	      else if (event.motion.x >= 16 && event.motion.x <= 16 + 356 &&
		  event.motion.y >= 337 && event.motion.y <= 337 + 27)
		{
		  /* Start two-player coop game: */
		  
		  highlight_x = 16;
		  highlight_y = 337;
		  highlight_w = 356;
		  highlight_h = 27;
		}
	      else if (event.motion.x <= 207 &&
		  event.motion.y >= 371 && event.motion.y <= 371 + 27)
		{
		  /* Toggle Barriers: */
		  
		  highlight_x = 0;
		  highlight_y = 371;
		  highlight_w = 207;
		  highlight_h = 27;
		}
	      else if (event.motion.x <= 374 &&
		  event.motion.y >= 398 && event.motion.y <= 398 + 27)
		{
		  /* Toggle Bouncy Balloons: */
		  
		  highlight_x = 0;
		  highlight_y = 398;
		  highlight_w = 374;
		  highlight_h = 27;
		}
	      else if (event.motion.x <= 234 &&
		  event.motion.y >= 425 && event.motion.y <= 425 + 27)
		{
		  /* Toggle Clear-All Mode: */
		  
		  highlight_x = 0;
		  highlight_y = 425;
		  highlight_w = 234;
		  highlight_h = 27;
		}
	      else if (event.motion.x >= 559 && event.motion.x <= 559 + 73 &&
		       event.motion.y >= 284 && event.motion.y <= 284 + 52)
		{
		  /* Set SFX Volume: */

		  highlight_x = 559;
		  highlight_y = 284;
		  highlight_w = 73;
		  highlight_h = 52;
		}
	      else if (event.motion.x >= 512 && event.motion.x <= 512 + 121 &&
		       event.motion.y >= 336 && event.motion.y <= 336 + 52)
		{
		  /* Set Music Volume: */
		  
		  highlight_x = 512;
		  highlight_y = 336;
		  highlight_w = 121;
		  highlight_h = 52;
		}
	      else if (event.motion.x >= 440 && event.motion.x <= 440 + 195 &&
		       event.motion.y >= 398 && event.motion.y <= 398 + 29)
		{
		  /* Hiscore: */
		  
		  highlight_x = 440;
		  highlight_y = 398;
		  highlight_w = 195;
		  highlight_h = 29;
		}
	      else if (event.motion.x >= 535 && event.motion.x <= 535 + 100 &&
		       event.motion.y >= 429 && event.motion.y <= 429 + 29)
		{
		  /* Exit: */
		  
		  highlight_x = 535;
		  highlight_y = 429;
		  highlight_w = 100;
		  highlight_h = 29;
		}
	      else
		{
		  highlight_x = -1;
		}
	    }
	}
      
      
      SDL_Delay(30);
      

      /* Erase highlight: */
      
      if (old_highlight_x != highlight_x ||
	  old_highlight_y != highlight_y)
	{
	  if (old_highlight_x != -1)
	    {
	      erase(old_highlight_x, old_highlight_y,
		    old_highlight_w, old_highlight_h,
		    IMG_TITLE);
	    }
	  
	  old_highlight_x = highlight_x;
	  old_highlight_y = highlight_y;
	  old_highlight_w = highlight_w;
	  old_highlight_h = highlight_h;
	}
      
      
      /* Erase the balloons: */
      
      if (use_low == 0)
	{
	  for (i = 0; i < NUM_TITLE_BALLOONS; i++)
	    {
	      erase(x[i], y[i], 32, 32, IMG_TITLE);
	    }
	}
      
      
      /* Erase credits: */
      
      erase(0, 252, 640, 32, IMG_TITLE);
      
      
      /* Move the balloons: */
      
      for (i = 0; i < NUM_TITLE_BALLOONS; i++)
	{
	  /* Move the balloon: */
	  
	  x[i] = x[i] + xm[i];
	  y[i] = y[i] + ym[i];
	  

	  /* Apply gravity: */
	  
	  if ((frame % 3) == 0)
	    {
	      ym[i] = ym[i] + 1;
	      
	      if (ym[i] > 16)
		ym[i] = 16;
	    }
	  
	  
	  /* Reset 'bumped' flag: */
	  
	  bumped[i] = 0;
	}

      
      /* Make balloons bounce into each other: */
      
      for (i = 0; i < NUM_TITLE_BALLOONS; i++)
	{
	  for (j = 0; j < NUM_TITLE_BALLOONS; j++)
	    {
	      if (j != i && bumped[j] == 0 && bumped[i] == 0)
		{
		  if (x[i] > x[j] - 32 &&
		      x[i] < x[j] + 32 &&
		      y[i] > y[j] - 32 &&
		      y[i] < y[j] + 32)
		    {
		      x[i] = x[i] - (xm[i] * 2) / 3;
		      y[i] = y[i] - (ym[i] * 2) / 3;
		      
		      tmp = xm[i];
		      xm[i] = xm[j];
		      xm[j] = tmp;

		      tmp = ym[i];
		      ym[i] = ym[j];
		      ym[j] = tmp;
		      
		      bumped[i] = 1;
		      bumped[j] = 1;
		    }
		}
	    }
	}


      /* Keep balloons in bounds: */
      
      for (i = 0; i < NUM_TITLE_BALLOONS; i++)
	{
	  if (x[i] <= 0)
	    {
	      xm[i] = (rand() % 5) + 1;
	      x[i] = 0;
	    }
	  else if (x[i] >= 640 - 32)
	    {
	      xm[i] = -((rand() % 5) + 1);
	      x[i] = 640 - 32;
	    }
	  
	  if (y[i] <= 0)
	    {
	      ym[i] = (rand() % 5) + 1;
	      y[i] = 0;
	    }
	  else if (y[i] >= 480 - 32)
	    {
	      ym[i] = -(ym[i]);
	      y[i] = 480 - 32;
	    }
	}
      
      
      /* Handle credits: */
      
      if (text_x < 0)
	{
	  text_x = text_x + text_xm;
	  
	  if (text_x >= 0)
	    {
	      text_x = 0;
	      text_xm = 0;
	    }
	  else
	    text_xm--;
	}
      else if (text_x == 0 && text_time < 100)
	{
	  text_time++;
	  text_xm = 0;
	}
      else if (text_x < 640)
	{
	  text_x = text_x + text_xm;
	  text_xm++;
	}
      else
	{
	  text_img = (text_img + 1) % 3;
	  text_x = -640;
	  text_xm = 36;
	  text_time = 0;
	}
      
      
      /* Draw highlight: */
      
      if (highlight_x != -1)
	{
	  src.x = highlight_x;
	  src.y = highlight_y - 275;
	  src.w = highlight_w;
	  src.h = highlight_h;
	  
	  dest.x = highlight_x;
	  dest.y = highlight_y;
	  dest.w = highlight_w;
	  dest.h = highlight_h;
	  
	  SDL_BlitSurface(images[IMG_TITLE_HIGHLIGHTS], &src,
			  screen, &dest);
	}
      
      
      /* Draw whether options are on: */
      
      if (barriers)
	draw(0, 376, IMG_LIGHT_ON);
      else
	draw(0, 376, IMG_LIGHT_OFF);
      
      if (bouncy)
	draw(0, 404, IMG_LIGHT_ON);
      else
	draw(0, 404, IMG_LIGHT_OFF);

      if (clearall)
	draw(0, 429, IMG_LIGHT_ON);
      else
	draw(0, 429, IMG_LIGHT_OFF);
      
      for (i = 0; i < 3; i++)
	{
	  if (sfx_vol > i)
	    draw(583 + i * 16, 311, IMG_LIGHT_ON);
	  else
	    draw(583 + i * 16, 311, IMG_LIGHT_OFF);

	  if (music_vol > i)
	    draw(583 + i * 16, 363, IMG_LIGHT_ON);
	  else
	    draw(583 + i * 16, 363, IMG_LIGHT_OFF);	
	}
      
      
      /* Draw the balloons: */
      
      if (use_low == 0)
	{
	  for (i = 0; i < NUM_TITLE_BALLOONS; i++)
	    {
	      if (xm[i] > 0)
		draw(x[i], y[i], color[i] + 2 + (rand() % 2));
	      else
		draw(x[i], y[i], color[i] + (rand() % 2));
	    }
	}
      
      
      /* Draw the credits: */
      
      draw(text_x, 252, IMG_PROGRAMMER + text_img);
      
      
      /* Update the screen: */
      
      SDL_Flip(screen);
      
      
      /* Keep playing music: */
      
#ifndef NOSOUND
      if (use_sound == 1)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_title, 0);
	      Mix_VolumeMusic((music_vol * MIX_MAX_VOLUME) / 3);
	    }
	}
#endif
    }
  while (done == 0 && quit == 0);
  
  
  /* Play a pop noise: */
  
  playsound(SND_POP);
  
  
  /* Stop title music: */
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      Mix_HaltMusic();
    }
#endif  
  
  return(quit);
}


/* Set video mode: */
/* Mattias Engdegard <f91-men@nada.kth.se> */

SDL_Surface * set_vid_mode(unsigned flags)
{
  /* Prefer 16bpp, but also prefer native modes to emulated 16bpp. */
  
  int depth;
  
  depth = SDL_VideoModeOK(640, 480, 16, flags);
  return depth ? SDL_SetVideoMode(640, 480, depth, flags) : NULL;
}



/* Set up screen, load graphics, sound and music: */

void setup(void)
{
  int i, col;
  SDL_Surface * image;
  SDL_Rect dest;
  
  
  /* Init SDL Video: */
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr,
              "\nError: I could not initialize video!\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", SDL_GetError());
      exit(1);
    }


  /* Init Joystick: */
  
#ifdef JOY_YES
  if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
      fprintf(stderr, "Warning: I could not initialize joystick!\n"
	      "The Simple DirectMedia error that occured was:\n"
	      "%s\n\n", SDL_GetError());
      
      use_joystick = 0;
    }
  else
    {
      /* Open joystick: */
      
      if (SDL_NumJoysticks() <= 0)
	{
	  fprintf(stderr, "Warning: No joysticks are available.\n");
	  
	  use_joystick = 0;
	}
      else
	{
	  js = SDL_JoystickOpen(0);
	  
	  if (js == NULL)
	    {
	      fprintf(stderr, "Warning: Could not open joystick 1.\n"
		      "The Simple DirectMedia error that occured was:\n"
		      "%s\n\n", SDL_GetError());
	      
	      use_joystick = 0;
	    }
	  else
	    {
	      /* Check for proper joystick configuration: */
	      
	      if (SDL_JoystickNumAxes(js) < 1)
		{
		  fprintf(stderr,
			  "Warning: Joystick does not have enough axes!\n");
		  
		  use_joystick = 0;
		}
	      else
		{
		  if (SDL_JoystickNumButtons(js) < 1)
		    {
		      fprintf(stderr,
			      "Warning: "
			      "Joystick does not have enough buttons!\n");
		      
		      use_joystick = 0;
		    }
		}
	    }
	}
    }
#endif
  
  
  /* Init SDL Audio: */
  
  if (use_sound == 1)
    {
      if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not initialize audio!\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }
  
  
  /* Open sound: */
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      if (Mix_OpenAudio(44100, AUDIO_S16, 2, 256) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up audio for 44100 Hz "
                  "16-bit stereo.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }
#endif


  /* Open display: */
  
  if (use_fullscreen == 1)
    {
      screen = set_vid_mode(SDL_FULLSCREEN | SDL_HWSURFACE);
      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up fullscreen video for "
                  "640x480 mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_fullscreen = 0;
        }
    }
  
  if (use_fullscreen == 0)
    {
      screen = set_vid_mode(0);
      
      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not set up video for 640x480 mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          exit(1);
        }
    }
  
  
  /* Set icon image: */
  
  seticon();

  
  /* Set window manager stuff: */
  
  SDL_WM_SetCaption("Circus Linux!", "Circus Linux!");
  
  
  /* Load graphics: */
  
  for (i = 0; i < NUM_IMAGES; i++)
    {
      /* Load image file: */
      
      image = IMG_Load(image_names[i]);

      if (image == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't load a graphics file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }
      
      
      /* Set transparency: */
      
      if (SDL_SetColorKey(image, (SDL_SRCCOLORKEY | SDL_RLEACCEL),
                          SDL_MapRGB(image -> format,
                                     0xFF, 0xFF, 0xFF)) == -1)
        {
          fprintf(stderr,
                  "\nError: I could not set the color key for the file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }

      images[i] = SDL_DisplayFormat(image);
      if (images[i] == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't convert a file to the display format:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }
      
      
      SDL_FreeSurface(image);
      
      
      /* Draw percentage bar: */
      
      dest.x = 0;
      dest.y = 470;
      dest.w = (640 * i) / NUM_IMAGES;
      dest.h = 10;
      
      col = (255 * i) / NUM_IMAGES;
      
      SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, col, col, col));
      SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
      SDL_Delay(1);
    }
  
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      /* Load sounds: */
      
      for (i = 0; i < NUM_SOUNDS; i++)
        {
          sounds[i] = Mix_LoadWAV(sound_names[i]);
          if (sounds[i] == NULL)
            {
              fprintf(stderr,
                      "\nError: I could not load the sound file:\n"
                      "%s\n"
                      "The Simple DirectMedia error that occured was:\n"
                      "%s\n\n", sound_names[i], SDL_GetError());
              exit(1);
            }
        }
      
      
      /* Load musics: */
      
      /* (title) */
      
      mus_title = Mix_LoadMUS(MUS_TITLE);
      if (mus_title == NULL)
	{
	  fprintf(stderr,
		  "\nError: I could not load the music file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", MUS_TITLE, SDL_GetError());
	  exit(1);
	}

      /* (game) */
      
      mus_game = Mix_LoadMUS(MUS_GAME);
      if (mus_game == NULL)
	{
	  fprintf(stderr,
		  "\nError: I could not load the music file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", MUS_GAME, SDL_GetError());
	  exit(1);
	}

      /* (gameover) */
      
      mus_gameover = Mix_LoadMUS(MUS_GAMEOVER);
      if (mus_gameover == NULL)
	{
	  fprintf(stderr,
		  "\nError: I could not load the music file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", MUS_GAMEOVER, SDL_GetError());
	  exit(1);
	}

      /* (hiscore) */

      mus_hiscore = Mix_LoadMUS(MUS_HISCORE);
      if (mus_hiscore == NULL)
	{
	  fprintf(stderr,
		  "\nError: I could not load the music file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", MUS_HISCORE, SDL_GetError());
	  exit(1);
	}

      /* (hiscore screen) */

      mus_hiscreen = Mix_LoadMUS(MUS_HISCORESCREEN);
      if (mus_hiscreen == NULL)
	{
	  fprintf(stderr,
		  "\nError: I could not load the music file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", MUS_HISCORESCREEN, SDL_GetError());
	  exit(1);
	}
    }
#endif

  
  
  /* Seed random generator: */
  
  srand(SDL_GetTicks());
}


/* Erase part of the screen back to the background: */

void erase(int x, int y, int w, int h, int bkgd)
{
  SDL_Rect dest;
  
  dest.x = x;
  dest.y = y;
  dest.w = w;
  dest.h = h;
  
  if (use_low == 0 || bkgd == IMG_TITLE)
    SDL_BlitSurface(images[bkgd], &dest, screen, &dest);
  else
    SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 50, 50, 80));
}


/* Draw an object: */

void draw(int x, int y, int pict)
{
  SDL_Rect src, dest;
  
  src.x = 0;
  src.y = 0;
  src.w = images[pict] -> w;
  src.h = images[pict] -> h;
  
  dest.x = x;
  dest.y = y;
  dest.w = src.w;
  dest.h = src.h;
  
  SDL_BlitSurface(images[pict], &src, screen, &dest);
}


/* Draw a clown: */

void drawclown(int x, int y, int side,
	       int left_arm, int right_arm,
	       int left_leg, int right_leg)
{
  draw(x, y, IMG_CLOWN_BODY_LEFT + side);
  
  if (left_arm != -1)
    draw(x, y, IMG_CLOWN_LEFT_ARM_0 + left_arm);
  
  if (right_arm != -1)
    draw(x, y, IMG_CLOWN_RIGHT_ARM_0 + right_arm);
  
  draw(x, y, IMG_CLOWN_LEFT_LEG_0 + left_leg);
  draw(x, y, IMG_CLOWN_RIGHT_LEG_0 + right_leg);
}


/* Clear rectangle list: */

void clearrects(void)
{
  num_rects = 0;
}


/* Add a rectangle: */

void addrect(int x, int y, int w, int h)
{
  rects[num_rects].x = x;
  rects[num_rects].y = y;
  rects[num_rects].w = w;
  rects[num_rects].h = h;
  
  if (num_rects < MAX_RECTS - 1)
    num_rects++;
}


/* Activate a clown: */

void newclown(void)
{
  /* Activate the clown: */
  
  flying_active = 1;
  flying_splat = 0;
  
  
  /* Start somewhere: */
  
  flying_x = 608 * (rand() % 2);
  flying_y = FLYING_START_Y;
  
  if (flying_x == 0)
    flying_dir = RIGHT;
  else
    flying_dir = LEFT;
  
  
  /* Speed: */
  
  flying_xm = 0;
  flying_ym = FLYING_START_YM;
  
  
  /* Set limbs randomly: */
  
  flying_left_arm = rand() % 3;
  flying_right_arm = rand() % 3;
  flying_left_leg = rand() % 2;
  flying_right_leg = rand() % 2;
}


/* Play a sound: */

void playsound(int snd)
{
#ifndef NOSOUND
  if (use_sound)
    Mix_PlayChannel(-1, sounds[snd], 0);
#endif
}


/* Reset a row of balloons: */

void resetballoons(int player, int row)
{
  int i;
  
  for (i = 0; i < 20; i++)
    balloons[player][row][i] = NORMAL;
}


/* Draw a balloon: */

void drawballoon(int player, int x, int y, int off)
{
  SDL_Rect dest;
  int img;
  
  
  img = -1;
  
  if (balloons[player][y][x] != GONE)
    {
      /* What color balloon? */
      
      img = IMG_BALLOON_RED_LEFT_0 + (balloon_colors[player][y] * 6);
      
      if (highscore_effect)
	img = IMG_BALLOON_RED_LEFT_0 + ((rand() % 8) * 6);
      
      
      /* Is it popping? */
      
      if (balloons[player][y][x] != NORMAL)
	{
	  /* Yes: */
	  
	  img = img + 4;
	  
	  if (balloons[player][y][x] == POPPING0)
	    img++;
	}
      else
	{
	  /* No:  Wave string */
	  
	  img = img + (rand() % 2);
	  
	  
	  /* What direction is it going? */
	  
	  img = img + ((y % 2) * 2);
	}
      
      
      /* Draw it! */
      
      dest.x = x * 32;
      
      /* (Shove it left or right 1/2 a space?) */
      
      if (off == 1)
	{
	  if ((y % 2) == 0)
	    dest.x = dest.x - 16;
	  else
	    dest.x = dest.x + 16;
	}
      
      dest.y = (y * 32) + 32;
      dest.w = 32;
      dest.h = 32;
      
      SDL_BlitSurface(images[img], NULL,
		      screen, &dest);
    }
}


/* Update bits of the background that have changed: */

void update_background(int which)
{
  int i;
  SDL_Rect rect;
  
  for (i = 0; i < NUM_BACKGROUND_CHANGES; i++)
    {
      rect.x = background_change_rects[i][0];
      rect.y = background_change_rects[i][1];
      rect.w = background_change_rects[i][2];
      rect.h = background_change_rects[i][3];

      SDL_BlitSurface(images[IMG_BACKGROUND_0 + which], &rect,
		      screen, &rect);
      
      addrect(rect.x, rect.y, rect.w, rect.h);
   }
}


/* Draw a number on the screen: */

void drawnumber(int x, int y, int v, int img)
{
  char str[16];
  int i;
  SDL_Rect src, dest;
  
  
  sprintf(str, "%d", v);
  
  for (i = 0; i < strlen(str); i++)
    {
      src.x = (str[i] - '0') * 32;
      src.y = 0;
      src.w = 32;
      src.h = 32;
      
      dest.x = x + (i * 32);
      dest.y = y;
      dest.w = 32;
      dest.h = 32;
      
      SDL_BlitSurface(images[img], &src,
		      screen, &dest);
    }
  
  addrect(x, y, strlen(str) * 32, 32);
}


/* Draw text: */

void drawtext(int x, int y, char * str)
{
  int i;
  SDL_Rect src, dest;
  
  for (i = 0; i < strlen(str); i++)
    {
      if (str[i] >= 'A' && str[i] <= 'Z')
	{
	  src.x = (str[i] - 'A') * 32;
	  src.y = 0;
	  src.w = 32;
	  src.h = 32;
	  
	  dest.x = x + (i * 32);
	  dest.y = y;
	  dest.w = 32;
	  dest.h = 32;
	  
	  SDL_BlitSurface(images[IMG_LETTERS], &src,
			  screen, &dest);
	}
    }
  
  addrect(x, y, strlen(str) * 32, 32);
}


/* Draw a darkened area: */

void drawfuzz(int x, int y, int w, int h)
{
  int xx, yy;
  SDL_Rect src, dest;
  
  for (yy = y; yy < y + h; yy = yy + (images[IMG_FUZZ] -> h))
    {
      for (xx = x; xx < x + w; xx = xx + (images[IMG_FUZZ] -> w))
	{
	  src.x = 0;
	  src.y = 0;
	  src.w = images[IMG_FUZZ] -> w;
	  src.h = images[IMG_FUZZ] -> h;
	  
	  if (xx + src.w > x + w)
	    src.w = x + w - xx;
	  
	  if (yy + src.h > y + h)
	    src.h = y + h - yy;
	  
	  dest.x = xx;
	  dest.y = yy;
	  
	  dest.w = src.w;
	  dest.h = src.h;
	  
	  SDL_BlitSurface(images[IMG_FUZZ], &src,
			  screen, &dest);
	}
    }
  
  addrect(x, y, w, h);
}


/* Set the application's icon: */

void seticon(void)
{
  int masklen;
  Uint8 * mask;
  SDL_Surface * icon;
  
  
  /* Load icon into a surface: */
  
  icon = IMG_Load(DATA_PREFIX "images/icon.png");
  if (icon == NULL)
    {
      fprintf(stderr,
              "\nError: I could not load the icon image: %s\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", DATA_PREFIX "images/icon.png", SDL_GetError());
      exit(1);
    }
  
  
  /* Create mask: */
  
  masklen = (((icon -> w) + 7) / 8) * (icon -> h);
  mask = malloc(masklen * sizeof(Uint8));
  memset(mask, 0xFF, masklen);
  
  
  /* Set icon: */
  
  SDL_WM_SetIcon(icon, mask);
  
  
  /* Free icon surface & mask: */
  
  free(mask);
  SDL_FreeSurface(icon);
}


/* Display usage screen: */

void usage(int ret)
{
  FILE * fs;
  
  if (ret == 1)
    fs = stderr;
  else
    fs = stdout;
  
  fprintf(fs, "\n"
	  "Command-line options:\n"
	  "\n"
	  "  --disable-sound - Disable sound and music. (Also \"--nosound\" or \"-q\")\n"
	  "  --fullscreen    - Display in full screen instead of a window, if possible.\n"
	  "                    (Also \"-f\")\n"
#ifdef JOY_YES
	  "  --digital       - Joystick will be seen as a digital game pad, not analog.\n"
	  "                    (Also \"-d\")\n"
	  "  --paddle        - Joystick will be seen as a real Atari paddle.\n"
          "                    (Also \"-p\")\n"
#endif
	  "  --low           - Use less graphics to work on lower-end hardware.\n"
	  "                    (Also \"-l\")\n"
	  "  --help          - Display help on playing the game. (Also \"-h\")\n"
	  "  --usage         - Display this usage information. (Also \"-u\")\n"
	  "  --version       - Display what version you're running. (Also \"-v\")\n"
	  "  --copying       - Display copying (GPL) information. (Also \"-c\")\n"
	  "\n");
  
  exit(ret);
}


/* Open the option file: */

FILE * open_option_file(char * mode)
{
  char * filename;
  FILE * fi;
#ifdef LINUX
  char * home;
#endif
  
  
#ifdef LINUX
  /* Get home directory (from $HOME variable)... if we can't determine it,
     use the current directory ("."): */

  if (getenv("HOME") != NULL)
    home = getenv("HOME");
  else
    home = ".";
  
  
  /* Create the buffer for the filename: */
  
  filename = (char *) malloc(sizeof(char) * (strlen(home) +
                                             strlen("/.circuslinux") + 1));
  
  strcpy(filename, home);
  strcat(filename, "/.circuslinux");
#else
  filename = "circuslinux.dat";
#endif
  
  
  /* Try opening the file: */
  
  fi = fopen(filename, mode);
  
  if (fi == NULL)
    {
      fprintf(stderr, "\nWarning: I could not open the options file "); 
      
      if (strcmp(mode, "r") == 0)
        fprintf(stderr, "for read:");
      else if (strcmp(mode, "w") == 0)
        fprintf(stderr, "for write:");
      
     fprintf(stderr, "\n%s\n"
              "The error that occured was:\n"
              "%s\n\n", filename, strerror(errno));
    }
  
  return(fi);
}


/* Increment a player's high score: */

void addscore(int player, int inc)
{
  score[player] = score[player] + inc;
  
  
  /* Is this a high score? */
  
  if (score[player] >= highscore[highscore_index])
    {
      /* Did they just get this high score? */
      
      if (has_highscore != player)
	{
	  has_highscore = player;
	  playsound(SND_HIGHSCORE);
	  highscore_effect = 50;
	}
      
      highscore[highscore_index] = score[player];
    }
}


/* Show high scores: */

int highscorescreen(void)
{
  SDL_Event event;
  int done, quit, i;
  
  
  /* Clear screen: */
  
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  draw(0, 0, IMG_HIGHSCORE_TOP);
  draw(0, images[IMG_HIGHSCORE_TOP] -> h, IMG_HIGHSCORE_LEFT);
  
  
  /* Draw scores: */
  
  for (i = 0; i < 8; i++)
    {
      /* Score: */
      
      drawnumber(32, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 16,
		 highscore[i], IMG_NUMBERS_0 + (i % 2));
      
      
      /* Initials: */
      
      drawtext(224, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 16,
	       highscorer[i]);
      
      
      /* Modes: */

      /* (Barriers): */
      
      if (i >= 4)
	{
	  draw(336, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_ON);
	}
      else
	{
	  draw(336, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_OFF);
	}
      
      
      /* (Bouncy Balloons): */
      
      if (i == 2 || i == 3 || i == 6 || i == 7)
	{
	  draw(444, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_ON);
	}
      else
	{
	  draw(444, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_OFF);
	}


      /* (Clear All): */
      
      if ((i % 2) == 1)
	{
	  draw(564, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_ON);
	}
      else
	{
	  draw(564, (images[IMG_HIGHSCORE_TOP] -> h) + (i * 32) + 24,
	       IMG_LIGHT_OFF);
	}
    }
  
  SDL_UpdateRect(screen, 0, 0, 640, 480);
  
  
  /* Wait for input: */
  
  done = 0;
  quit = 0;
  
  do
    {
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* Quit event!  */
	      
	      quit = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* A keypress! */
	      
	      done = 1;
	    }
	  else if (event.type == SDL_MOUSEBUTTONDOWN)
	    {
	      /* A mouseclick! */
	      
	      done = 1;
	    }
	}
      
      SDL_Delay(30);
      
      
      /* Keep playing music: */
      
#ifndef NOSOUND
      if (use_sound == 1)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_hiscreen, 0);
	      Mix_VolumeMusic((music_vol * MIX_MAX_VOLUME) / 3);
	    }
	}
#endif

    }
  while (!done && !quit);


  /* Stop music: */
  
#ifndef NOSOUND
  if (use_sound == 1)
    Mix_HaltMusic();
#endif

  
  return(quit);
}


/* Pause screen: */

int pausescreen(void)
{
  SDL_Event event;
  SDLKey key;
  int done, quit;
  
  
  /* Stop music: */
  
#ifndef NOSOUND
  if (use_sound)
    {
      Mix_PauseMusic();
    }
#endif
  
  
  /* Display "PAUSED" Message: */
  
  drawfuzz(224, 224, 192, 32);
  drawtext(224, 224, "PAUSED");
  SDL_UpdateRect(screen, 224, 224, 192, 32);
  
  
  /* Wait for keypress: */
  
  done = 0;
  quit = 0;
  
  do
    {
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* Quit event!  */
	      
	      quit = 2;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* A keypress! */

              key = event.key.keysym.sym;

	      if (key == SDLK_SPACE || key == SDLK_TAB ||
		  key == SDLK_p)
		{
		  /* SPACE, TAB or P: Pause! */
		  
		  done = 1;
		}
	      else if (key == SDLK_ESCAPE)
		{
		  /* ESCAPE: Quit! */
		  
		  quit = 1;
		}
	    }
	}
    }
  while (quit == 0 && done == 0);
  
  
  /* Erase message: */
  
  erase(224, 224, 192, 32, IMG_BACKGROUND_0);
  SDL_UpdateRect(screen, 224, 224, 192, 32);


  /* Unpause music: */
  
#ifndef NOSOUND
  if (use_sound)
    {
      Mix_ResumeMusic();
    }
#endif
  
  
  return(quit);
}


/* Determine user's initials: */

void getinitials(void)
{
  int i;
#ifdef LINUX
  struct passwd * pw;
#endif
  char * tmp;
  
  
  /* Default to nothing: */
  
  for (i = 0; i < 3; i++)
    username_initials[i] = '\0';
  
  
#ifdef LINUX
  if (getenv("USER") != NULL)
    {
      pw = getpwnam(getenv("USER"));
      
      if (pw != NULL && pw->pw_gecos != NULL && strlen(pw->pw_gecos) != 0)
	{
	  /* Grab initials from real name: */
	  
	  /* First initial (easy!) */
	  
	  username_initials[0] = toupper(pw->pw_gecos[0]);
	  
	  
	  /* Second initial (after first space): */
	  
	  for (i = 0; (i < strlen(pw->pw_gecos) &&
		       username_initials[1] == '\0'); i++)
	    {
	      if (pw->pw_gecos[i] == ' ')
		username_initials[1] = toupper(pw->pw_gecos[i + 1]);
	    }
	  
	  
	  /* Third initial (next space): */
	  
	  for (i = i; (i < strlen(pw->pw_gecos) &&
		       username_initials[2] == '\0'); i++)
	    {
	      if (pw->pw_gecos[i] == ' ')
		username_initials[2] = toupper(pw->pw_gecos[i + 1]);
	    }
	}
      else
	{
	  /* Grab initials from username: */
	  
	  tmp = getenv("USER");
	  
	  for (i = 0; i < 3 && i < strlen(tmp); i++)
	    {
	      username_initials[i] = toupper(tmp[i]);
	    }
	}
    }
#endif
}
