/* Code for Arduino to use with an IC Counter Chip circuit to
 *  measure how long before a certain signal is received.
 * Author: Jasmine Le
 * Datasheet for IC Counter Chip:
 http://www.ti.com/lit/ds/symlink/sn74lv8154.pdf */

/* Counter inputs */
#define clka 37
#define rcoa 51
#define clkb 39
#define clkben 53
#define rclk 49
#define cclr 35
/* Pins to get upper or lower byte of the 16 bit register */
#define gal 41
#define gau 43
#define gbl 45
#define gbu 47;
/* Pins to data bus */
#define v0 22
#define v1 24
#define v2 26
#define v3 28
#define v4 30
#define v5 32
#define v6 34
#define v7 36

/* Interrupt pin to get an external signal */
#define interruptPin 2

/* Output pin for Arduino's square wave */
int out = clkb

int value = 0;
int toPrint = 0;
int del = 100;

/* Set pin modes and values */
void setup() {
  pinMode(out, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), responsecalc, HIGH);

  pinMode(clkben, OUTPUT);
  digitalWrite(clkben, LOW);

  pinMode(rclk, OUTPUT);
  digitalWrite(rclk, LOW);

  pinMode(cclr, OUTPUT);
  digitalWrite(cclr, LOW);
  digitalWrite(cclr, HIGH);

  pinMode(gal, OUTPUT);
  pinMode(gau, OUTPUT);
  pinMode(gbl, OUTPUT);
  pinMode(gbu, OUTPUT);
  digitalWrite(gal, HIGH);
  digitalWrite(gau, HIGH);  
  digitalWrite(gbl, LOW);
  digitalWrite(gbu, HIGH);    

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);
}

/* Output clock signal to counter and register */
void loop() {
  digitalWrite(out, LOW);
  digitalWrite(rclk, HIGH);
  delay(del);
  digitalWrite(out, HIGH);
  digitalWrite(rclk, LOW);
  if (toPrint) {
    Serial.println(value);
    toPrint = 0;
  }
  delay(del);
}

/* Calculate counter value when signaled */
void responsecalc() {
  digitalWrite(LED_BUILTIN, LOW);
  
  digitalWrite(rclk, HIGH);
  digitalWrite(rclk, LOW);
  
  digitalWrite(gbl, HIGH);
  digitalWrite(gbu, LOW);  
    
  value = getByte();
  value <<= 8;
  
  digitalWrite(gbl, LOW);
  digitalWrite(gbu, HIGH);
  
  value |= getByte(); 
  toPrint = 1;
  
  digitalWrite(LED_BUILTIN, HIGH);
}

/* Get data output of counter */
int getByte() {
  return (digitalRead(v7) << 7) | (digitalRead(v6) << 6) 
                                | (digitalRead(v5) << 5) 
                                | (digitalRead(v4) << 4) 
                                | (digitalRead(v3) << 3) 
                                | (digitalRead(v2) << 2) 
                                | (digitalRead(v1) << 1) 
                                | digitalRead(v0);
}
