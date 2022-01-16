//=================================================================
// Implementation for the player module
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "player_private.h"

PLAYER player; // structure of player

PLAYER player_get_info(void){
    return player;
}

void player_init(void) {    
    player.y = 0;
    player.x = 0;
    player.width = 10;
    player.height = 10;
    player.score = 0;
    player.knife_length = 2;
    point in1;
    in1.x = 11;
    in1.y = player.y; 
    point in2;
    in2.x = 17;
    in2.y = player.y + 2;
    player.knife_boundingBox.topLeft = in1;
    player.knife_boundingBox.bottomRight = in2;
    player.player_has_knife = true;
    player.lives = 3;
    player.strikes = 7;
    player.things = 1;
    player.mult = 1;
}

void player_moveUp(void) {
    if (player.y <= 120 && player.y >= 0) {
        if (player.player_has_knife) {
            player_draw(0x000000);
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.y -= player.height;
            player.knife_boundingBox.bottomRight.y -= player.height;
            player.knife_boundingBox.topLeft.y -= player.height;
            return;
        }
        player_draw(0x000000);
        player.y -= player.height;
        return;
    }
    if (player.y < 0) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.y = 0;
    } else if (player.y > 120) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.y = 120;
    }
    return;
}

void player_moveDown(void) { 
    if (player.y <= 120 && player.y >= 0) {
        if (player.player_has_knife) {
            player_draw(0x000000);
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.y += player.height;
            player.knife_boundingBox.bottomRight.y += player.height;
            player.knife_boundingBox.topLeft.y += player.height;
            return;
        }
        player_draw(0x000000);
        player.y += player.height;
        return;
    }
    if (player.y < 0) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.y = 0;
    } else if (player.y > 120) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.y = 120;
    }
    return;
}

void player_moveRight(void) {
    if (player.x <= 120 && player.x >= 0) {
        if (player.player_has_knife) {
            player_draw(0x000000);
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.x += player.width;
            player.knife_boundingBox.bottomRight.x += player.width;
            player.knife_boundingBox.topLeft.x += player.width;
            return;
        }
        player_draw(0x000000);
        player.x += player.width;
        return;
    }
    if (player.x < 0) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.x = 0;
        player.knife_boundingBox.topLeft.x = 11;
        player.knife_boundingBox.bottomRight.x = 17;
    } else if (player.x > 120) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.knife_boundingBox.topLeft.x = player.width + 112;
        player.knife_boundingBox.bottomRight.x = player.width + 118;
        player.x = 111;
    }
    return;
}

void player_moveLeft(void) {
    if (player.x <= 120 && player.x >= 0) {
        if (player.player_has_knife) {
            player_draw(0x000000);
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.x -= player.width;
            player.knife_boundingBox.bottomRight.x -= player.width;
            player.knife_boundingBox.topLeft.x -= player.width;
            return;
        }
        player_draw(0x000000);
        player.x -= player.width;
        return;
    }
    if (player.x < 0) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.x = 0;
        player.knife_boundingBox.topLeft.x = 11;
        player.knife_boundingBox.bottomRight.x = 17;
    } else if (player.x > 120) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.knife_boundingBox.topLeft.x = player.width + 112;
        player.knife_boundingBox.bottomRight.x = player.width + 118;
        player.x = 111;
    }
    return;
}

void player_throw(void) { 
    player.player_has_knife = false;
}

void player_knife_draw(void) {
    if (player.player_has_knife) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0xFF0000);
        return;
    }
    if (player.x <= 64) {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.knife_boundingBox.topLeft.x += 10;
        player.knife_boundingBox.bottomRight.x += 10;
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0xFF0000);
        if (player.knife_boundingBox.bottomRight.x >= 120) {
            if (player.things == 1) player_strikes_dec();
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.things = 1;
            player_knife_return();
        }
    return;
    } else {
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
        player.knife_boundingBox.topLeft.x -= 10;
        player.knife_boundingBox.bottomRight.x -= 10;
        uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.knife_boundingBox.topLeft.y, player.knife_boundingBox.bottomRight.x, player.knife_boundingBox.bottomRight.y, 0xFF0000);
        if (player.knife_boundingBox.topLeft.x <= 0) {
            if (player.things == 1) player_strikes_dec();
            uLCD.filled_rectangle(player.knife_boundingBox.topLeft.x, player.y, player.knife_boundingBox.bottomRight.x, player.y + player.knife_length, 0x000000);
            player.things = 1;
            player_knife_return();
        }
    }
}

void player_knife_return(void) {
    player.player_has_knife = true;
    player.knife_boundingBox.bottomRight.x = player.x + 17;
    player.knife_boundingBox.topLeft.x = player.x + 11;
    player.knife_boundingBox.bottomRight.y = player.y + player.knife_length;
    player.knife_boundingBox.topLeft.y = player.y;
    player_knife_draw();
}

// ==== player_private.h implementation ====
void player_draw(int color) {
    uLCD.filled_rectangle(player.x, player.y, player.x + player.width, player.y + player.height, color); 
}

void player_score_inc(void) {
    player.score += player.mult;
}

void player_lives_dec(void) {
    player.lives -= 1;
    player.mult = 1;
}

void player_strikes_dec(void) {
    player.strikes -= 1;
    player.mult = 1;
}

void player_things_inc(void) {
    player.things += 1;
}

void player_mult_inc(void) {
    player.mult *= 2;
}