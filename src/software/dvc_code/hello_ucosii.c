/*************************************************************************
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 **************************************************************************
 * Description:                                                           *
 * The following is a simple hello world program running MicroC/OS-II.The *
 * purpose of the design is to be a very simple application that just     *
 * demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
 * for issues such as checking system call return codes. etc.             *
 *                                                                        *
 * Requirements:                                                          *
 *   -Supported Example Hardware Platforms                                *
 *     Standard                                                           *
 *     Full Featured                                                      *
 *     Low Cost                                                           *
 *   -Supported Development Boards                                        *
 *     Nios II Development Board, Stratix II Edition                      *
 *     Nios Development Board, Stratix Professional Edition               *
 *     Nios Development Board, Stratix Edition                            *
 *     Nios Development Board, Cyclone Edition                            *
 *   -System Library Settings                                             *
 *     RTOS Type - MicroC/OS-II                                           *
 *     Periodic System Timer                                              *
 *   -Know Issues                                                         *
 *     If this design is run on the ISS, terminal output will take several*
 *     minutes per iteration.                                             *
 **************************************************************************/

#include <stdio.h>
#include "includes.h"
#include <stdlib.h>
#include <system.h>
#include "io.h"

#include "./drivers/drivers.h"

//VIDEO and GRAPS
#include "vip_fr.h"
#include "./graphic_lib/simple_graphics.h"
#include "./graphic_lib/gimp_bmp.h"
#include "./graphic_lib/draw_gimps.h"
//OS

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
#define FREQ_READER_INCREMENT (100)
#define FREQ_READER_NOMINAL (44000)
#define FREQ_READER_MAX (2*FREQ_READER_NOMINAL)


OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task3_stk[TASK_STACKSIZE];
OS_STK task4_stk[TASK_STACKSIZE];
OS_STK task5_stk[TASK_STACKSIZE];
OS_STK task6_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define TASK4_PRIORITY      4
#define TASK5_PRIORITY      5
#define TASK6_PRIORITY      6

//tasks
void task1(void* pdata);
void task2(void* pdata);

//video
VIP_FRAME_READER *SW_Frame;

//IMAGENES GUARDADAS Y LEIDAS POR SOFWTARE
extern struct gimp_image_struct back_down;
extern struct gimp_image_struct back_up;
extern struct gimp_image_struct ford_up;
extern struct gimp_image_struct ford_down;
extern struct gimp_image_struct pause_up;
extern struct gimp_image_struct pause_down;
extern struct gimp_image_struct ford_down;
extern struct gimp_image_struct play_down;
extern struct gimp_image_struct play_up;
extern struct gimp_image_struct stop_down;
extern struct gimp_image_struct stop_up;
extern struct gimp_image_struct forward_arrow;
extern struct gimp_image_struct backward_arrow;


extern unsigned char song[];
extern unsigned char song_2[];
extern unsigned char song_3[];

extern unsigned int size_song;
// VARIABLES GLOBALES DE BOTONES
unsigned char buttons_control1[12];
unsigned char buttons_control2[12];

void draw_button(int x, int y, int w, int h, int color, char * text,
		char event_type, alt_video_display* display) {

	vid_draw_box(x, y, x + w, y + h, BLACK_24, DO_FILL, display);
	vid_draw_box(x, y, x + w, y + h, event_type ? ~color : color, DO_FILL,
			display);
	vid_print_string_alpha(x + w / 4, y + h / 8, WHITE_24,
			event_type ? ~color : color, tahomabold_20, display, text);

}
/*Special button with text that fits inside a smaller box */
void draw_nice_button(int x, int y, int w, int h, int color, char * text,
		char event_type, alt_video_display* display) {

	vid_draw_box(x, y, x + w, y + h, BLACK_24, DO_FILL, display);
	vid_draw_box(x, y, x + w, y + h, event_type ? ~color : color, DO_FILL,
			display);
	vid_print_string_alpha(x, y + h / 20, WHITE_24,
			event_type ? ~color : color, tahomabold_20, display, text);

}

void init_background() { // INIT BACKGROUND

	//Clean screen
	vid_clean_screen(SW_Frame, BLACK_24);

	//Init Images
	DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
	DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);



	//init lines
	vid_draw_line(652, 234, 800, 234, 4, WHITE_24, SW_Frame);
	vid_draw_line(0, 407, 800, 407, 4, WHITE_24, SW_Frame);
	vid_draw_line(0, 598, 800, 598, 4, WHITE_24, SW_Frame);
	vid_draw_line(647, 0, 647, 600, 4, WHITE_24, SW_Frame);
	vid_draw_line(795, 0, 795, 600, 4, WHITE_24, SW_Frame);
	vid_draw_line(261, 0, 261, 262, 4, WHITE_24, SW_Frame);
	vid_draw_line(261, 262, 647, 262, 4, WHITE_24, SW_Frame);
	vid_draw_line(261, 0, 800, 0, 4, WHITE_24, SW_Frame);

	/*Init colors*/
	draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 1, SW_Frame);
	draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
	draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
	draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
	draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
	draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
	draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
	draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);




	draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 1, SW_Frame);
	draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 0, SW_Frame);
	draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 0, SW_Frame);
	draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 0, SW_Frame);
	draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 0, SW_Frame);


	draw_button(667, 426, 120, 35, 0xCC6600, "SINE", 0, SW_Frame);
	draw_button(667, 470, 120, 35, 0xCC6600, "COS", 0, SW_Frame);
	draw_button(667, 512, 120, 35, 0xCC6600, "SAW", 0, SW_Frame);
	draw_button(667, 554, 120, 35, 0xCC6600, "SQUARE", 1, SW_Frame);

}

/*Read song from EPCS, and send it to the FIFO*/
#define MAX_ADDRESS 524287
#define NUMERO_DE_MUESTRAS 1000 // mini buffervoid send_audio_fifo(unsigned char *buf) {int i;for (i = 0; i < NUMERO_DE_MUESTRAS; i++) {while (audio_dac_fifo_full() == 1);audio_dac_wr_fifo( buf[i % size_song]);
}

}

unsigned char play_song = 0;
unsigned char pause_song = 0;
unsigned char stop_song = 0;

unsigned char which_song = 0; //Determines which song is suppose to play

int freq_reader = FREQ_READER_NOMINAL;

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata) // ANIMATION BY SOFTWARE TASK
{
int address_counter = 0;
set_audio_frequency_audio_controller(freq_reader);
while (1) {

if (play_song == 1) {

	if(which_song == 0){
		send_audio_fifo(&song[address_counter % size_song]);
	} else if(which_song == 1){
		send_audio_fifo(&song_2[address_counter % size_song]);
	} else if(which_song == 2){
		send_audio_fifo(&song_3[address_counter % size_song]);
	}

	if (address_counter < size_song) {
		address_counter += NUMERO_DE_MUESTRAS;

	} else {
		address_counter = 0;
	}
} else if (pause_song == 1) {

} else if (stop_song == 1) {
	address_counter = 0;
}
OSTimeDlyHMSM(0, 0, 0, 10);
}

}

/*refresh buttons states, and actions*/
void task2(void* pdata) {

int x_mouse, y_mouse;
unsigned char event;

while (1) {
event = mouse_pos(&x_mouse, &y_mouse);

if (event == 1) {	//down event


	//Logic for the modulation buttons
	if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 252
			&& y_mouse <= (252 + 28)) {
		draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 1, SW_Frame);
		draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 0, SW_Frame);
		draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 0, SW_Frame);
		draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 0, SW_Frame);
		draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 0, SW_Frame);

		select_modulation(0);
	}

	else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 282
			&& y_mouse <= (282 + 28)) {
		draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 0, SW_Frame);
		draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 1, SW_Frame);
		draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 0, SW_Frame);
		draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 0, SW_Frame);
		draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 0, SW_Frame);

		select_modulation(1);
	} else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 312
			&& y_mouse <= (312 + 28)) {
		draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 0, SW_Frame);
		draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 0, SW_Frame);
		draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 1, SW_Frame);
		draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 0, SW_Frame);
		draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 0, SW_Frame);


		select_modulation(2);
	} else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 342
			&& y_mouse <= (342 + 28)) {
		draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 0, SW_Frame);
		draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 0, SW_Frame);
		draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 0, SW_Frame);
		draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 1, SW_Frame);
		draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 0, SW_Frame);

		select_modulation(3);
	}else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 372
		&& y_mouse <= (372 + 28)) {
		draw_button(667, 252, 120, 28, 0x0066CC, "ASK", 0, SW_Frame);
		draw_button(667, 282, 120, 28, 0x0066CC, "FSK", 0, SW_Frame);
		draw_button(667, 312, 120, 28, 0x0066CC, "BPSK", 0, SW_Frame);
		draw_button(667, 342, 120, 28, 0x0066CC, "LFSR", 0, SW_Frame);
		draw_button(667, 372, 120, 28, 0x0066CC, "QPSK", 1, SW_Frame);

		select_modulation(4);
		}

	//Logic for the signal selector buttons
	if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 426
			&& y_mouse <= (426 + 35)) {
		draw_button(667, 426, 120, 35, 0xCC6600, "SINE", 1, SW_Frame);
		draw_button(667, 470, 120, 35, 0xCC6600, "COS", 0, SW_Frame);
		draw_button(667, 512, 120, 35, 0xCC6600, "SAW", 0, SW_Frame);
		draw_button(667, 554, 120, 35, 0xCC6600, "SQUARE", 0, SW_Frame);

		select_signal(0);
	}

	else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 470
			&& y_mouse <= (470 + 35)) {
		draw_button(667, 426, 120, 35, 0xCC6600, "SINE", 0, SW_Frame);
		draw_button(667, 470, 120, 35, 0xCC6600, "COS", 1, SW_Frame);
		draw_button(667, 512, 120, 35, 0xCC6600, "SAW", 0, SW_Frame);
		draw_button(667, 554, 120, 35, 0xCC6600, "SQUARE", 0, SW_Frame);

		select_signal(1);
	} else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 512
			&& y_mouse <= (512 + 35)) {
		draw_button(667, 426, 120, 35, 0xCC6600, "SINE", 0, SW_Frame);
		draw_button(667, 470, 120, 35, 0xCC6600, "COS", 0, SW_Frame);
		draw_button(667, 512, 120, 35, 0xCC6600, "SAW", 1, SW_Frame);
		draw_button(667, 554, 120, 35, 0xCC6600, "SQUARE", 0, SW_Frame);

		select_signal(2);
	} else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 554
			&& y_mouse <= (554 + 35)) {
		draw_button(667, 426, 120, 35, 0xCC6600, "SINE", 0, SW_Frame);
		draw_button(667, 470, 120, 35, 0xCC6600, "COS", 0, SW_Frame);
		draw_button(667, 512, 120, 35, 0xCC6600, "SAW", 0, SW_Frame);
		draw_button(667, 554, 120, 35, 0xCC6600, "SQUARE", 1, SW_Frame);

		select_signal(3);
	}

	//play song buttons
	if (x_mouse >= 296 && x_mouse <= (296 + play_up.width) && y_mouse >= 45
			&& y_mouse <= (45 + play_up.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_down, 296, 45,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);



		play_song = 1;
		pause_song = 0;
		stop_song = 0;

	} else if (x_mouse >= 404 && x_mouse <= (404 + stop_down.width)
			&& y_mouse >= 45 && y_mouse <= (45 + stop_down.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_down, 404, 45,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);



		play_song = 0;
		pause_song = 0;
		stop_song = 1;
	} else if (x_mouse >= 523 && x_mouse <= (523 + pause_down.width)
			&& y_mouse >= 45 && y_mouse <= (45 + pause_down.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_down, 523, 45,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);



		play_song = 0;
		pause_song = 1;
		stop_song = 0;
	} else if (x_mouse >= 355 && x_mouse <= (355 + back_up.width)
			&& y_mouse >= 177 && y_mouse <= (177 + back_up.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_down, 355, 177,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 180, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);


		freq_reader -= FREQ_READER_INCREMENT;
		if (freq_reader < FREQ_READER_INCREMENT) {
			freq_reader = FREQ_READER_INCREMENT;
		}
		set_audio_frequency_audio_controller(freq_reader);
	} else if (x_mouse >= 500 && x_mouse <= (500 + ford_down.width)
			&& y_mouse >= 177 && y_mouse <= (177 + ford_down.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);



		freq_reader += FREQ_READER_INCREMENT;
		if (freq_reader > FREQ_READER_MAX) {
			freq_reader = FREQ_READER_MAX;
		}
		set_audio_frequency_audio_controller(freq_reader); //plays the next song
	} else if (x_mouse >= 570 && x_mouse <= (570 + forward_arrow.width)
			&& y_mouse >= 173 && y_mouse <= (173 + forward_arrow.height)) {
		DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177,
		BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
		DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);

		play_song = 0;
		pause_song = 0;
		stop_song = 1;

		if(which_song < 2){
			which_song++;
		}
	}
		else if (x_mouse >= 285 && x_mouse <= (285 + backward_arrow.width)//plays the previous song
					&& y_mouse >= 173 && y_mouse <= (173 + backward_arrow.height)) {
				DispIMAGE_from_gimp(SW_Frame, &play_up, 296, 45, BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &stop_up, 404, 45, BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &pause_up, 523, 45, BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &back_up, 355, 177, BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &ford_up, 500, 177,
				BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &forward_arrow, 570, 173, BLACK_24);
				DispIMAGE_from_gimp(SW_Frame, &backward_arrow, 285, 173, BLACK_24);

				play_song = 0;
				pause_song = 0;
				stop_song = 1;

				if(which_song > 0){
					which_song--;
		}
	}
		//changes the color of the graph
		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 5
					&& y_mouse <= (5 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 1, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);

			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xefc180);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 35
					&& y_mouse <= (35 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 1, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xc7a8cb);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 65
					&& y_mouse <= (65 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 1, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0x7a2e39);

		}


		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 95
					&& y_mouse <= (95 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 1, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xb5e5f8);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 125
					&& y_mouse <= (125 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 1, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0x746f70);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 155
					&& y_mouse <= (155 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 1, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xf7b0cc);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 185
					&& y_mouse <= (185 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 1, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 0, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xcdc9c1);

		}

		else if (x_mouse >= 667 && x_mouse <= (667 + 120) && y_mouse >= 215
					&& y_mouse <= (215 + 28)){
			draw_nice_button(667, 5, 120, 28, 0xefc180, "FEARLESS", 0, SW_Frame);
				draw_nice_button(667, 35, 120, 28, 0xc7a8cb, "SPEAK NOW", 0, SW_Frame);
				draw_nice_button(667, 65, 120, 28, 0x7a2e39, "RED", 0, SW_Frame);
				draw_nice_button(667, 95, 120, 28, 0xb5e5f8, "1989", 0, SW_Frame);
				draw_nice_button(667, 125, 120, 28, 0x746f70, "REPUTATION", 0, SW_Frame);
				draw_nice_button(667, 155, 120, 28, 0xf7b0cc, "LOVER", 0, SW_Frame);
				draw_nice_button(667, 185, 120, 28, 0xcdc9c1, "FOLKLORE", 0, SW_Frame);
				draw_nice_button(667, 215, 120, 28, 0xc5ac90, "EVERMORE", 1, SW_Frame);


			IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xc5ac90);

		}


}

OSTimeDlyHMSM(0, 0, 0, 100);
}

}

/* The main function creates two task and starts multi-tasking */
int main(void) {
//Init video
init_lfsr_interrupt();
SW_Frame = VIPFR_Init(VGA_ALT_VIP_VFR_0_BASE, (void *) FR_FRAME_0,
	(void *) FR_FRAME_0, FRAME_WIDTH, FRAME_HEIGHT);
VIPFR_Go(SW_Frame, TRUE);
VIPFR_ActiveDrawFrame(SW_Frame);

//Draw background images, and start the desktop
init_background();

//init hw
//default audio channel with default color
IOWR_ALTERA_AVALON_PIO_DATA(GRAPH_COLOUR_BASE, 0xefc180);
audio_selector(1);
//init selectors
select_modulation(0);
select_signal(3);

//task song
OSTaskCreateExt(task1,
NULL, (void *) &task1_stk[TASK_STACKSIZE - 1],
TASK1_PRIORITY,
TASK1_PRIORITY, task1_stk,
TASK_STACKSIZE,
NULL, 0);

//events and graphics refreshing
OSTaskCreateExt(task2,
NULL, (void *) &task2_stk[TASK_STACKSIZE - 1],
TASK2_PRIORITY,
TASK2_PRIORITY, task2_stk,
TASK_STACKSIZE,
NULL, 0);

OSStart();

while (1) {
}

return 0;
}
