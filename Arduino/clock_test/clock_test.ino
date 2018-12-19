/*******************************************************************************
* File: clock_test.ino
* Author: Dylan Oh
* Date Created: 16 Dec 2018
* Desc: Uses an Arduino Mega 2560 to create a (somewhat) accurate Nixie tube
        clock. Note: This file is meant as a prototyping tool and is not 
        indicative of the methodology that will be used in the final clock. As 
        such it does not incorporate a real time clock and instead uses a basic
        delay to pass a second.
*******************************************************************************/

/* Creating struct Nixie_t to store pin and value data for nixie tubes */
typedef struct{
  int pinA, pinB, pinC, pinD;
  int val = 0;
} Nixie_t;

/*******************************************************************************
 * Function Definitions                                                        *
 *******************************************************************************/

/*
 * Passing in a digit to be displayed as well as the struct of the tube that is
 * displaying the digit and using binary masks to effectively convert a decimal
 * int to a byte that represents whether a pin will be high or low.
 */

void set_nixie_tube(Nixie_t* x, int num);         // Function Declaration

void set_nixie_tube(Nixie_t* x, int num){         // Function Definition
  digitalWrite(x -> pinA, num & 0b0001);
  digitalWrite(x -> pinB, (num & 0b0010) >> 1);
  digitalWrite(x -> pinC, (num & 0b0100) >> 2);
  digitalWrite(x -> pinD, (num & 0b1000) >> 3);
}

/*******************************************************************************
 * Assigning Pins to Tubes                                                     *
 *******************************************************************************/

Nixie_t tube1 = {.pinA = 50, .pinB = 51, .pinC = 52, .pinD = 53}; // second digit 2
Nixie_t tube2 = {.pinA = 46, .pinB = 47, .pinC = 48, .pinD = 49}; // second digit 1
Nixie_t tube3 = {.pinA = 42, .pinB = 43, .pinC = 44, .pinD = 45}; // minute digit 2
Nixie_t tube4 = {.pinA = 38, .pinB = 39, .pinC = 40, .pinD = 41}; // minute digit 1
Nixie_t tube5 = {.pinA = 34, .pinB = 35, .pinC = 36, .pinD = 37}; // hour digit 2
Nixie_t tube6 = {.pinA = 30, .pinB = 31, .pinC = 32, .pinD = 33}; // hour digit 1

/*******************************************************************************
 * Main Function                                                               *
 *******************************************************************************/

void setup(){
  pinMode(tube1.pinA, OUTPUT);
  pinMode(tube1.pinB, OUTPUT);
  pinMode(tube1.pinC, OUTPUT);
  pinMode(tube1.pinD, OUTPUT);
  pinMode(tube2.pinA, OUTPUT);
  pinMode(tube2.pinB, OUTPUT);
  pinMode(tube2.pinC, OUTPUT);
  pinMode(tube2.pinD, OUTPUT);
  pinMode(tube3.pinA, OUTPUT);
  pinMode(tube3.pinB, OUTPUT);
  pinMode(tube3.pinC, OUTPUT);
  pinMode(tube3.pinD, OUTPUT);
  pinMode(tube4.pinA, OUTPUT);
  pinMode(tube4.pinB, OUTPUT);
  pinMode(tube4.pinC, OUTPUT);
  pinMode(tube4.pinD, OUTPUT);
}

void loop(){
  /* Running clock logic for each digit/tube */
  if (tube1.val == 10){
    tube2.val++;
    tube1.val = 0;
  } else {
    tube1.val++;
  }

  if (tube2.val == 6){
    tube3.val++;
    tube2.val = 0;
  } else {
    tube2.val++;
  }

  if (tube3.val == 10){
    tube4.val++;
    tube3.val = 0;
  } else {
    tube3.val++;
  }

  if (tube4.val == 10){
    tube5.val++;
    tube4.val = 0;
  } else {
    tube4.val++;
  }

  if (tube5.val == 4 && tube6.val == 2){
    tube5.val = 0;
    tube6.val = 0;
  } else if (tube5.val == 10){
    tube6.val++;
    tube5.val = 0;
  } else {
    tube5.val++;
  }

  /* Setting pins with the display values */
  set_nixie_tube(&tube1, tube1.val);
  set_nixie_tube(&tube2, tube2.val);
  set_nixie_tube(&tube3, tube3.val);
  set_nixie_tube(&tube4, tube4.val);
  set_nixie_tube(&tube5, tube5.val);
  set_nixie_tube(&tube6, tube6.val);
  
  /* Incrementing and repeating every second (more or less) */
  delay(1000);
}
