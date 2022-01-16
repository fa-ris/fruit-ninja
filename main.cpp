//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "compost_pile_public.h"
#include "fruit_public.h"
#include "player_public.h"


//For sound components
////AnalogOut DACout(p18);
//PwmOut pwmout(p26);
////PwmOut speaker(p25);

////wave_player waver(&DACout);
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// ===User implementations start===
int fruit_contact(void); // iterate through fruits and see if any collided
void compost_pile_update(void);

void playSound(char * wav);
void playNotes(void);
void set_random_seed(Timer);

int main()
{
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Program Starting\n");

    // ===User implementations start===
    // Game state variables
    
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    set_random_seed(t);
 
    //pwmout.period(1.0/4000.0);
    //playSound("/sd/wavfiles/BUZZER.wav");//test the sound convert to thread
    //wait(0.1);
    //playNotes();
    
    //initialize functions
    compost_pile_init();
    fruit_init();
    player_init();        
    pc.printf("Initialization complete\n");
    
    while(1)
    {
        t.start();

        // Draw s first
        //pc.printf("fruit_generator\n");
        if (player_get_info().score < 5) fruit_generator();
        if (player_get_info().score >= 5){
            fruit_generator_new();
            uLCD.locate(150, 150);
            uLCD.printf("Green grapes unlocked!");
        }
        
        //Put score on screen
        // NOTE: Text is 8 pixels tall
        uLCD.locate(0, 0);
        uLCD.printf("Score: %d Lives: %d\nStrikes: %d\n", player_get_info().score, player_get_info().lives, player_get_info().strikes);

        //pc.printf("player_fruit_draw\n");
        player_draw(0x0000FF);
        player_knife_draw();
        
        
        
        pc.printf("Reading inputs\n");
        inputs = read_inputs();

        //pc.printf("player motions\n");
        if (inputs.b1 || inputs.ay>=0.3) player_moveUp();            
        if (inputs.b2 || inputs.ay<= -0.3) player_moveDown();  
        if (inputs.ax <= -0.3) player_moveRight();
        if (inputs.ax >= 0.3) player_moveLeft();              
        if (inputs.b3) player_throw();
        
        //pc.printf("compost_pile_update\n");
        compost_pile_update();
        
        //pc.printf("fruit_contact\n");
        int is_bomb = fruit_contact();
        
        // TODO: Check end of the game        
        //pc.printf("check endgame\n");
        if (player_get_info().lives <= 0 || get_compost_tallest_height() >= 55) {
            uLCD.locate(0, 0);
            uLCD.cls();
            uLCD.printf("Loser :p\n");
            return 0;
        }
        
        if (player_get_info().strikes <= 0) {
            uLCD.locate(0, 0);
            uLCD.cls();
            uLCD.printf("Loser :p\n");
            return 0;
        }
        
        // TODO: Check if game won
        if (player_get_info().score >= 15) {
            uLCD.locate(0, 0);
            uLCD.cls();
            uLCD.printf("Winner B-)\n");
            return 1;
        }

        // Compute update time
        t.stop();
        dt = t.read_ms();
        
        if (dt < 100) wait_ms(100 - dt);
    }
    //pc.printf("out of main loop\n");
    destroyList(get_fruit_list());
    
    return 0; // comment
    // ===User implementations end===
}

// ===User implementations start===

/* This function iterate through the fruit list, checking if anyone one of them is contacting 
*  with the player's knife. Don't forget to check if the "fruit" is the bomb, since we 
*  generated bombs using the fruit_generator. 
*/ 

int fruit_contact(void) {
    PLAYER p1 = player_get_info();
    DLinkedList* list = get_fruit_list();
    LLNode* temp = list->head;
    FRUIT* fruit;
    int knifex1 = p1.knife_boundingBox.bottomRight.x;
    int knifex2 = p1.knife_boundingBox.topLeft.x;
    int knifey1 = p1.knife_boundingBox.bottomRight.y;
    int knifey2 = p1.knife_boundingBox.topLeft.y;
    int x1, x2, y1, y2;
    for (int i = 0; i < list->size; i++) {
        fruit = (FRUIT*) temp->data;
        x1 = fruit->box.bottomRight.x;
        x2 = fruit->box.topLeft.x;
        y1 = fruit->box.bottomRight.y;
        y2 = fruit->box.topLeft.y;
        if ((p1.player_has_knife == false) && ((!(knifex2 >= x1) && !(knifex1 <= x2)) && (!(knifey1 <= y2) && !(knifey2 >= y1)))) {
            if (fruit->type == 0) {
                fruit->status = FRUIT_SLICED;
                draw_nothing(fruit->box);
                uLCD.filled_rectangle(p1.knife_boundingBox.topLeft.x, p1.knife_boundingBox.topLeft.y, p1.knife_boundingBox.bottomRight.x, p1.knife_boundingBox.bottomRight.y, 0x000000);
                player_knife_return();
                deleteNode(list, temp);
                player_lives_dec();
            } else if (fruit->type == 3) {
                fruit->status = FRUIT_SLICED;
                draw_nothing(fruit->box);
                uLCD.filled_rectangle(p1.knife_boundingBox.topLeft.x, p1.knife_boundingBox.topLeft.y, p1.knife_boundingBox.bottomRight.x, p1.knife_boundingBox.bottomRight.y, 0x000000);
                //player_knife_return();
                deleteNode(list, temp);
                player_score_inc();
                player_score_inc(); // double score increase
                player_things_inc();
                player_mult_inc();
            } else {
                fruit->status = FRUIT_SLICED;
                draw_nothing(fruit->box);
                uLCD.filled_rectangle(p1.knife_boundingBox.topLeft.x, p1.knife_boundingBox.topLeft.y, p1.knife_boundingBox.bottomRight.x, p1.knife_boundingBox.bottomRight.y, 0x000000);
                //player_knife_return();
                deleteNode(list, temp);
                player_score_inc();
                player_things_inc();
                player_mult_inc();
            }
        }
        temp = temp->next;
    }
    return 0;
}

/** Call compost_pile_update() repeatedly in your game-loop. ex: main()
    This function iterate through the fruit list. Add fruits that reach the bottom of the screen
    to the compost pile.
    @return  Number of remaining cities. You might end the game when all cities are demolished.
*/
void compost_pile_update(void){
/*    DLinkedList* list = get_fruit_list();
    LLNode* temp = list->head;
    FRUIT* fruit;
    int x;
    for (int i = 0; i < list->size; i++) {
        fruit = (FRUIT*) temp->data;
        if (fruit->box.bottomRight.y >= 127 && fruit->box.bottomRight.x <= 127 && fruit->box.topLeft.x >= 0) {
            x = fruit->box.bottomRight.x / 11;
            draw_nothing(fruit->box);
            deleteNode(list, temp);
            compost_add(x);
        }
        temp = temp->next;
    }*/
}

//fcn to play a wav
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
        return;
}

/* Wanna hear some annoying sounds?*/
void playNotes(void)
{
    int i;
// generate a 500Hz tone using PWM hardware output
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(3);
    speaker=0.0; // turn off audio
    wait(2);
// generate a short 150Hz tone using PWM hardware output
// something like this can be used for a button click effect for feedback
    for (i=0; i<10; i++) {
        speaker.period(1.0/150.0); // 500hz period
        speaker =0.25; //25% duty cycle - mid range volume
        wait(.02);
        speaker=0.0; // turn off audio
        wait(0.5);
    }

// sweep up in frequency by changing the PWM period
    for (i=0; i<8000; i=i+100) {
        speaker.period(1.0/float(i));
        speaker=0.25;
        wait(.1);
    }
    wait(2);

// two tone police siren effect -  two periods or two frequencies
// increase volume - by changing the PWM duty cycle
    for (i=0; i<26; i=i+2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    // decrease volume
    for (i=25; i>=0; i=i-2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    speaker =0.0;
    wait(2);
}

void set_random_seed(Timer t) {
    GameInputs inputs; 
    t.start();
    uLCD.printf("Push any button to start.\n");
    while(1){
      inputs = read_inputs();
      if (inputs.b1 || inputs.b2 || inputs.b3) break;
      }
    uLCD.cls();
    t.stop();
    int seed = t.read_ms();    
    srand(seed);
    }
// ===User implementations end===
