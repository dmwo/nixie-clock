typedef struct nixie{
  int A;
  int B;
  int C;
  int D;
  int pinA;
  int pinB;
  int pinC;
  int pinD;
  int flag = 0;
};

nixie tube[6];
int PinA[6] = {46, 47, 38, 39, 30, 31};
int PinB[6] = {48, 49, 40, 41, 32, 33};
int PinC[6] = {50, 51, 42, 43, 34, 35};
int PinD[6] = {52, 53, 44, 45, 36, 37};

void setup() {
  for (int i = 0; i < 6; i++) {
    tube[i].pinA = PinA[i];
    tube[i].pinB = PinB[i];
    tube[i].pinC = PinC[i];
    tube[i].pinD = PinD[i];
  }
  pinMode(PinA, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(PinC, OUTPUT);
  pinMode(PinD, OUTPUT);
}

int num_to_array (num) {
  int out[3];
  switch(num) {
    case 0:
      out = {LOW, LOW, LOW, LOW}
    case 1:
      out = 
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
  }
}

void loop() {

  
  
  if (count == 9) {
    if (tube[5].flag < 6){
      tube[5].flag++;
    }
    else { tube[5].flag = 0; }
  }
  
  digitalWrite(tube[i].pinA, tube[i].A)
  digitalWrite(tube[i].pinB, tube[i].B)
  digitalWrite(tube[i].pinC, tube[i].C)
  digitalWrite(tube[i].pinD, tube[i].D)
  
  delay(1000);
}
