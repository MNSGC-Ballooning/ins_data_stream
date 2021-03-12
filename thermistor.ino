// Constants
#define ADC_MAX 8191.0                        // Maximum ADC value given to the thermistor, should be 8192 for a Teensy 4.0
#define CONST_A 0.001125308852122             // A, B, and C are constants used for a 10k resistor and 10k thermistor for the steinhart-hart equation
#define CONST_B 0.000234711863267             // NOTE: These values change when the thermistor and/or resistor change value, so if that happens, more research needs to be done on those constants
#define CONST_C 0.000000085663516                                       
#define CONST_R 10000                         // 10k Ω resistor 
#define C2K 273.15                            // Celsius to Kelvin conversion

// Thermistor functions

void updateThermistor() {
  int analog_thermistor = analogRead(THERMISTOR);

  thermTemp = thermAnalogCalc(analog_thermistor);
}

// Thermistor analog calculations
float thermAnalogCalc(int analog) {
  float t = log((float(ADC_MAX/analog) - 1)*CONST_R);
  t = CONST_A + CONST_B*t + CONST_C*t*t*t;        // A + B*t + C*(t^3)
  t = 1/t - C2K;

  return t;                                       // returns temperature in Celsius
}
