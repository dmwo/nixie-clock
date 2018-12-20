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
  int flag = 0;
} Nixie_t;

Nixie_t tube1, tube2, tube3, tube4, tube5, tube6;

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
 * Main Function                                                               *
 *******************************************************************************/

void setup(){
  /* Setting pins for the BCD outputs */
  tube1.pinA = 50; tube1.pinB = 51; tube1.pinC = 52; tube1.pinD = 53;
  tube2.pinA = 46, tube2.pinB = 47, tube2.pinC = 48, tube2.pinD = 49;
  tube3.pinA = 42, tube3.pinB = 43, tube3.pinC = 44, tube3.pinD = 45;
  tube4.pinA = 38, tube4.pinB = 39, tube4.pinC = 40, tube4.pinD = 41;
  tube5.pinA = 34, tube5.pinB = 35, tube5.pinC = 36, tube5.pinD = 37;
  tube6.pinA = 30, tube6.pinB = 31, tube6.pinC = 32, tube6.pinD = 33;

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
  if (tube1.val == 9){
    tube1.flag = 1;
    tube1.val = 0;
  } else {
    tube1.val++;
  }
  
  while (tube1.flag == 1){
    tube1.flag = 0;
    if (tube2.val == 5){
      tube2.flag = 1;
      tube2.val = 0;
    } else {
      tube2.val++;
    }
  }

  while (tube2.flag == 1){
    tube2.flag = 0;
    if (tube3.val == 9){
      tube3.flag = 1;
      tube3.val = 0;
    } else {
      tube3.val++;
    }
  }

  while (tube3.flag == 1){
    tube3.flag = 0;
    if (tube4.val == 5){
      tube4.flag = 1;
      tube4.val = 0;
    } else {
      tube4.val++;
    }
  }

  while (tube4.flag == 1){
    tube4.flag = 0;
    if (tube5.val == 3 && tube6.val == 2){
      tube5.val = 0;
      tube6.val = 0;
    } else if (tube5.val == 9){
      tube6.val++;
      tube5.val = 0;
    } else {
      tube5.val++;
    }
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





