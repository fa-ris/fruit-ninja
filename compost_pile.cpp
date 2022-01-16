//=================================================================
// The file is for module "compost pile"
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "compost_pile_private.h"

COMPOST compost_record[NUM_PILE];
int tallest_pile_height;


// See the comments in compost_pile_public.h
void compost_pile_init() {
    for (int index = 0; index < NUM_PILE; index++) {
        compost_record[index].x = index * 11;
        compost_record[index].y = 127;
        compost_record[index].height = 0;
        compost_record[index].width = 11;
    }
    tallest_pile_height = 0;
}

COMPOST compost_get_info(int index){
    if (index < MAX_NUM_COMPOST_PILES) return compost_record[index];
}

void compost_add(int index) {
    if (index >= MAX_NUM_COMPOST_PILES) {
        return;
    }
    compost_record[index].height += 11;
    uLCD.filled_rectangle(compost_record[index].x, compost_record[index].y, compost_record[index].width, 127 - compost_record[index].height, 0xFF0000); 

}

void draw_compost(void){
    for (int index = 0; index < MAX_NUM_COMPOST_PILES; index++) {
        uLCD.filled_rectangle(compost_record[index].x, compost_record[index].y, compost_record[index].width, 127 - compost_record[index].height, 0xFF0000); 
    }
}

int get_compost_tallest_height() {
    for (int index = 0; index < NUM_PILE; index++) {
        if (compost_record[index].height > tallest_pile_height) {
            tallest_pile_height = compost_record[index].height;
        }
    }
    return tallest_pile_height;
}

int get_compost_height(int index) {
    if (index < MAX_NUM_COMPOST_PILES) return compost_record[index].height;
}