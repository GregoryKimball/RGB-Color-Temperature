

class RGB
{
public:
  float R;
  float G;
  float B;
  virtual void set_color_temperature(int kelvin)
  {
    float k = (float)kelvin;

    R = 255;
    G = (k - 1000) / 9000. * 165. + 75.;
    B = (k - 1000) / 9000. * 60. + 5.;
  }

  virtual int map_color_temperature(int analog_in_value)
  {
    // range is 1000K to 10000K

    float temp = 1000. + ((float)analog_in_value) / 1024. * 9000.;

    return (int)temp;
  }
};

RGB *c;
int GREEN = 11;
int RED = 10;
int BLUE = 9;

bool flick = true;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(0, INPUT);
  pinMode(1, INPUT);

  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);

  c = new RGB();

  TCCR1A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR1B = _BV(CS22);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS22);
}

void loop()
{

  int a = analogRead(0);

  int b = analogRead(1);

  float INTENSITY = (float)b / 1024. - 0.05;

  if (INTENSITY < 0.03)
  {
    INTENSITY = 0.0;
  }

  int temp = c->map_color_temperature(a);
  Serial.println(temp);

  c->set_color_temperature(temp);

  /*
  Timer output	Arduino output	
  OC0A	        6	
  OC0B	        5	
  OC1A	        9	blue
  OC1B	        10	red
  OC2A	        11	green
  OC2B	        3	
 */

  for (int i = 0; i < 10; i++)
  {
    float remainder = 0;
    remainder = (c->G * INTENSITY) - (float)(int)(c->G * INTENSITY);
    if (remainder > (float)i * 0.1)
    {
      OCR2A = c->G * INTENSITY + 1;
    }
    else
    {
      OCR2A = c->G * INTENSITY;
    }

    remainder = (c->R * INTENSITY) - (float)(int)(c->R * INTENSITY);
    if (remainder > (float)i * 0.1)
    {
      OCR1B = c->R * INTENSITY + 1;
    }
    else
    {
      OCR1B = c->R * INTENSITY;
    }

    remainder = (c->B * INTENSITY) - (float)(int)(c->B * INTENSITY);
    if (remainder > (float)i * 0.1)
    {
      OCR1A = c->B * INTENSITY + 1;
    }
    else
    {
      OCR1A = c->B * INTENSITY;
    }
  }
}
