#include <TimerOne.h>
#include <Math.h>

const int TICK_PERIOD_10MS = 10000;
const int TICKS_1S = 100;

int tick_counter = 0;

typedef enum gear_t {
  GEAR_1,
  GEAR_2,
  GEAR_3,
  GEAR_4,
  GEAR_5,
  GEAR_6,
  NEUTRAL,
  MAX_NUM_OF_GEARS
};

struct gears_struct_t {
  float gear_ref_voltage;
  gear_t gear;
};

gears_struct_t gears[MAX_NUM_OF_GEARS] =
{
  {1.39, GEAR_1},
  {1.81, GEAR_2},
  {2.55, GEAR_3},
  {3.25, GEAR_4},
  {4.10, GEAR_5},
  {4.55, GEAR_6},
  {5.00, NEUTRAL}
};

int gearPin = A0;
int tempSensorPin = A1;
int batteryPin = A2;

float temperature = 0.0;
float batteryVoltage = 0.0;
float gearVoltage = 0.0;

gear_t actualGear = NEUTRAL;

const float TOLERANCE = 0.2;

void updateGear()
{
  gearVoltage = analogRead(gearPin);

  for (int i = MAX_NUM_OF_GEARS - 1; i >= 0 ; i--)
  {
    if (gearVoltage > (gears[i].gear_ref_voltage - TOLERANCE))
    {
      actualGear = gears[i].gear;
    }
  }
}

void updateTemp()
{
  temperature = analogRead(tempSensorPin);
  temperature = round(temperature * 10) / 10;
}

void updateVoltage()
{
  batteryVoltage = analogRead(batteryPin);
  batteryVoltage = round(batteryVoltage * 10) / 10;
}

void updateInputs()
{
  updateGear();
  if (tick_counter > TICKS_1S)
  {
    updateTemp();
    updateVoltage();

    tick_counter = 0.0;
  }
}

void updateDisplay()
{
  ;
}

void tickUpdate()
{
  tick_counter++;
}

// the setup function runs once when you press reset or power the board
void setup() {
  Timer1.initialize(TICK_PERIOD_10MS);
  Timer1.attachInterrupt(tickUpdate);

  updateInputs();
  updateDisplay();
}

// the loop function runs over and over again forever
void loop() {
  updateInputs();
  updateDisplay();
}
