int button = 2;
int led = 13;
int sound = 7;
int status = false;

void setup(){
pinMode(led, OUTPUT);
pinMode(sound, OUTPUT);
pinMode(button, INPUT_PULLUP); 
}

void loop(){

if (digitalRead(button) == true) {
status = !status;
digitalWrite(led, status);
digitalWrite(sound, status);
	{
		status = true;
 		tone(sound, 10000, 800);
		delay (100);
	}
} while(digitalRead(button) == true);
delay(50);
}

