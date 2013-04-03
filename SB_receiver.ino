const int datapin  = 11; // DI
const int latchpin = 10; // LI
const int enablepin = 9; // EI
const int clockpin = 8; // CI

unsigned long SB_CommandPacket;
int SB_CommandMode;
int SB_BlueCommand;
int SB_RedCommand;
int SB_GreenCommand;

int inByte = 0;

void setup() {
   pinMode(datapin, OUTPUT);
   pinMode(latchpin, OUTPUT);
   pinMode(enablepin, OUTPUT);
   pinMode(clockpin, OUTPUT);

   digitalWrite(latchpin, LOW);
   digitalWrite(enablepin, LOW);
   
   Serial.begin(9600);
   
   SB_CommandMode = B01; // Write to current control registers
   SB_RedCommand = 127; // Full current
   SB_GreenCommand = 127; // Full current
   SB_BlueCommand = 127; // Full current
   SB_SendPacket();
}

void loop() {
  switch(readSerial()) {
    case 49:
      alarm();
      break;
    case 50:
      notification();
      break;
    default:
      idle();
  } 
}


int readSerial() {
  if(Serial.available() > 0) {
    inByte = Serial.read();
  }
  return inByte;
}

void SB_SendPacket() {
   SB_CommandPacket = SB_CommandMode & B11;
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_BlueCommand & 1023);
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_RedCommand & 1023);
   SB_CommandPacket = (SB_CommandPacket << 10)  | (SB_GreenCommand & 1023);

   shiftOut(datapin, clockpin, MSBFIRST, SB_CommandPacket >> 24);
   shiftOut(datapin, clockpin, MSBFIRST, SB_CommandPacket >> 16);
   shiftOut(datapin, clockpin, MSBFIRST, SB_CommandPacket >> 8);
   shiftOut(datapin, clockpin, MSBFIRST, SB_CommandPacket);

   delay(1); // adjustment may be necessary depending on chain length
   digitalWrite(latchpin,HIGH); // latch data into registers
   delay(1); // adjustment may be necessary depending on chain length
   digitalWrite(latchpin,LOW);
}


  
  
  
  
void idle() {
  for(int i = 500; i < 701; i++){
   SB_CommandMode = B00;
   SB_RedCommand = 0;
   SB_GreenCommand = i;
   SB_BlueCommand = 0;
   SB_SendPacket();
   
   delay(10);
  }
  
  delay(100);
  
  for(int i = 700; i > 499; i--){
   SB_CommandMode = B00;
   SB_RedCommand = 0;
   SB_GreenCommand = i;
   SB_BlueCommand = 0;
   SB_SendPacket();
   
   delay(10);
  }

  delay(200);
}

void alarm() {
  for(int i = 750; i < 1023; i++){
   SB_CommandMode = B00;
   SB_RedCommand = i;
   SB_GreenCommand = 0;
   SB_BlueCommand = 0;
   SB_SendPacket();
  }
  
  delay(200);
  
  for(int i = 1023; i >= 0; i--){
   SB_CommandMode = B00;
   SB_RedCommand = i;
   SB_GreenCommand = 0;
   SB_BlueCommand = 0;
   SB_SendPacket();
  }

  delay(1000);
}

void notification() {
  for(int i = 0; i < 1023; i++){
   SB_CommandMode = B00;
   SB_RedCommand = i;
   SB_GreenCommand = i;
   SB_BlueCommand = 0;
   SB_SendPacket();
  }
  
  delay(500);
  
  for(int i = 1023; i >= 0; i--){
   SB_CommandMode = B00;
   SB_RedCommand = i;
   SB_GreenCommand = i;
   SB_BlueCommand = 0;
   SB_SendPacket();
  }

  delay(2000);
}
