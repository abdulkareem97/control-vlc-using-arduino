const int trigger1 = 2;  //Trigger pin of 1st Sesnor
const int echo1 = 3;     //Echo pin of 1st Sesnor
const int trigger2 = 4;  //Trigger pin of 2nd Sesnor
const int echo2 = 5;     //Echo pin of 2nd Sesnor

long time_taken;
int dist, distL, distR;


const int max_value = 40;
const int pause_play_distance = 30;
const int locked_min_value = 5;
const int locked_max_value = 20;
const int unloked_value = 30;
const int low_value = 15;
const int high_value = 16;




void setup() {
  Serial.begin(9600);

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
}

/*###Function to calculate distance###*/
void calculate_distance(int trigger, int echo) {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  time_taken = pulseIn(echo, HIGH);
  dist = time_taken * 0.034 / 2;
  // Serial.println("Distance: " + String(dist) + " cm");
  if (dist > max_value)
    dist = max_value;
}

void loop() {  //infinite loopy
  calculate_distance(trigger1, echo1);
  distL = dist;  //get distance of left sensor

  calculate_distance(trigger2, echo2);
  distR = dist;  //get distance of right sensor

  //Uncomment for debudding
  /*Serial.print(“L=”);
Serial.println(distL);
Serial.print(“R=”);
Serial.println(distR);
*/

  //Pause Modes -Hold
  // Serial.println(distL + " " + distR);
  // if ((distL > 30 && distR > 30) && (distL < 50 && distR < 50))  //Detect both hands
  if ((distL > pause_play_distance && distR > pause_play_distance) && (distL < max_value && distR < max_value))  //Detect both hands
  {
    Serial.println("Play/Pause");
    delay(500);
  }

  calculate_distance(trigger1, echo1);
  distL = dist;

  calculate_distance(trigger2, echo2);
  distR = dist;

  //Control Modes
  //Lock Left – Control Mode
    // Serial.println(distL + " " + distR);

  if (distL >= locked_min_value && distL <= locked_max_value) {
    delay(100);  //Hand Hold Time
    calculate_distance(trigger1, echo1);
    distL = dist;
    if (distL >= locked_min_value && distL <= locked_max_value) {
      Serial.println("Left Locked");
      while (distL <= unloked_value) {
        calculate_distance(trigger1, echo1);
        distL = dist;
        if (distL < low_value)  //Hand pushed in
        {
          Serial.println("Vup");
          delay(300);
        }
        if (distL > high_value)  //Hand pulled out
        {
          Serial.println("Vdown");
          delay(300);
        }
      }
      Serial.println("Left UNLocked");
    }
  }

  //Lock Right – Control Mode
  if (distR >= locked_min_value && distR <= locked_max_value) {
    delay(100);  //Hand Hold Time
    calculate_distance(trigger2, echo2);
    distR = dist;
    if (distR >= locked_min_value && distR <= locked_max_value) {
      Serial.println("Right Locked");
      while (distR <= unloked_value) {
        calculate_distance(trigger2, echo2);
        distR = dist;
        if (distR < low_value)  //Right hand pushed in
        {
          Serial.println("Rewind");
          delay(300);
        }
        if (distR > high_value)  //Right hand pulled out
        {
          Serial.println("Forward");
          delay(300);
        }
      }
      Serial.println("Right UnLocked");

    }
  }

  delay(200);
}