#define TRIG 27
#define ECHO 26
#define HUMIDIFIER 25

unsigned long lastPing = 0;

// states for humidifier sequence
bool sequenceRunning = false;
unsigned long seqTimer = 0;
int seqStep = 0;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(HUMIDIFIER, OUTPUT);

  digitalWrite(HUMIDIFIER, HIGH);  // idle = HIGH
}

// FAST ultrasonic
int getDistanceFast() {
  long duration;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH, 20000);

  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}


// -------------------------
// HUMIDIFIER SEQUENCE
// -------------------------
void triggerHumidifierSequence() {
  if (sequenceRunning) return;

  sequenceRunning = true;
  seqStep = 1;
  seqTimer = millis();
}

// non-blocking sequence handler
void updateHumidifierSequence() {
  if (!sequenceRunning) return;

  unsigned long now = millis();

  switch (seqStep) {

    case 1: // FIRST SIGNAL (ON)
      digitalWrite(HUMIDIFIER, LOW);
      if (now - seqTimer >= 100) {   // 0.1 sec
        digitalWrite(HUMIDIFIER, HIGH);
        seqStep = 2;
        seqTimer = now;
      }
      break;

    case 2: // WAIT 0.9 sec before second signal
      if (now - seqTimer >= 900) {
        digitalWrite(HUMIDIFIER, LOW);
        seqStep = 3;
        seqTimer = now;
      }
      break;

    case 3: // SECOND SIGNAL (closing soon)
      if (now - seqTimer >= 100) {
        digitalWrite(HUMIDIFIER, HIGH);
        seqStep = 4;
        seqTimer = now;
      }
      break;

    case 4: // WAIT 0.1 sec before final close pulse
      if (now - seqTimer >= 100) {
        digitalWrite(HUMIDIFIER, LOW);
        seqStep = 5;
        seqTimer = now;
      }
      break;

    case 5: // THIRD SIGNAL (closed)
      if (now - seqTimer >= 100) {
        digitalWrite(HUMIDIFIER, HIGH);
        sequenceRunning = false; // done
      }
      break;
  }
}


void loop() {

  // ultrasonic ping every 15ms
  if (millis() - lastPing >= 15) {
    lastPing = millis();

    int distance = getDistanceFast();

    Serial.println(distance);

    // Start sequence only if bee detected
    if (distance > 0 && distance < 20 && !sequenceRunning) {
      triggerHumidifierSequence();
    }
  }

  // update humidifier state machine
  updateHumidifierSequence();
}
