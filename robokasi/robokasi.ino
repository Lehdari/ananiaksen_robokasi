#include <Servo.h>

Servo servo[6];
const int posMin[] = { 0, 80, 10, 20, 0, 90 };
const int posMax[] = { 180, 160, 120, 140, 180, 160 };
int pos[] = { 90, 90, 90, 90, 90, 120 };
bool dir[] = { true, true, true, true, true, true };

void updateServos() {
  for (int s=0; s<6; ++s) {
    // safety limits
    if (pos[s] < posMin[s])
      pos[s] = posMin[s];
    if (pos[s] > posMax[s])
      pos[s] = posMax[s];

    // write position
    servo[s].write(pos[s]);
  }
}

void setup() {
  servo[0].attach(11);
  servo[1].attach(10);
  servo[2].attach(9);
  servo[3].attach(6);
  servo[4].attach(5);
  servo[5].attach(3);
  
  Serial.begin(9600, SERIAL_8N1);
}

int a = 0;

void loop() {
  
  // read serial input
  static char buf[1024];
  bool serialRead = false;
  int i=0;
  for (; i<1024; ++i)
    buf[i]='\0';
  for (i=0; Serial.available(); ++i) {
    buf[i] = Serial.read();
    serialRead = true;
  }
  buf[i] = '\0';

  if (serialRead) {
    int a, b, c, d, e, f;
    sscanf(buf, "A%dB%dC%dD%dE%dF%d", &a, &b, &c, &d, &e, &f);
    pos[0] = a;
    pos[1] = b;
    pos[2] = c;
    pos[3] = d;
    pos[4] = e;
    pos[5] = f;
    
    for (int j=0; j<6; ++j) {
      Serial.print(pos[j]);
      Serial.print(" ");
    }
    
    Serial.flush();
  }

  updateServos();

  delay(100); //Delay in milliseconds
}
