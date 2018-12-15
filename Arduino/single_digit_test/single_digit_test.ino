int pinA = 47;
int pinB = 49;
int pinC = 51;
int pinD = 53;
int A, B, C, D;
int count = 0;

void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
}

void loop() {
  for (int count = 0; count < 10; count++) {
    switch(count) {
      case 0:
        D = 0; C = 0; B = 0; A = 0;
        break;
      case 1:
        D = 0; C = 0; B = 0; A = 1;
        break;
      case 2:
        D = 0; C = 0; B = 1; A = 0;
        break;
      case 3:
        D = 0; C = 0; B = 1; A = 1;
        break;
      case 4:
        D = 0; C = 1; B = 0; A = 0;
        break;
      case 5:
        D = 0; C = 1; B = 0; A = 1;
        break;
      case 6:
        D = 0; C = 1; B = 1; A = 0;
        break;
      case 7:
        D = 0; C = 1; B = 1; A = 1;
        break;
      case 8:
        D = 1; C = 0; B = 0; A = 0;
        break;
      case 9:
        D = 1; C = 0; B = 0; A = 1;
        break;
    }
    digitalWrite(pinA, A);
    digitalWrite(pinB, B);
    digitalWrite(pinC, C);
    digitalWrite(pinD, D);
    delay(1000);
  }
}
