#define limitSpeed 255
#define limitMotor 5000
#define motor 33 //D33
#define maju 18 //D18
#define mundur 19 //D19

void setup(){
  Serial.begin(9600);
  pinMode(maju, OUTPUT); digitalWrite(maju, LOW);
  pinMode(mundur, OUTPUT); digitalWrite(mundur, LOW);
}

void loop(){
	int cahaya = map(analogRead(A0), 4095, 0, 0, 100);
	if(cahaya > 50){
		aksiAngkat();
	}
	else{
		aksiJemur();
	}
  delay(1000);
}

void aksiJemur(){
  Serial.println("\tSedang Menjemur");
    analogWrite(motor, limitSpeed);
    digitalWrite(maju, HIGH);
    digitalWrite(mundur, LOW);
    delay(limitMotor);
}
 
void aksiAngkat(){
  Serial.println("\tSedang Mengangkat");
    analogWrite(motor, limitSpeed);
    digitalWrite(maju, LOW);
    digitalWrite(mundur, HIGH);
    delay(limitMotor);
}
