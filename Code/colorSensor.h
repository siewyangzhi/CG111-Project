/**
 * CG1111 mBot Project
 *
 * @file: colorSensor.h
 * @author: 4b-2
 *
 * This library includes functions that enable the mBot
 * to detect the six colors including red, green, yellow,
 * purple, light blue and black. Colours are first detected
 * as RGB values and then converted to HSV values.
 */
MeLightSensor LightSensor(8);
MeRGBLed RGBLED(13);

#define RGBWait 50 //Delay between 2 colors to prevent the previous color from affecting the current color
#define LDRWait 5 // Delay between readings
float blackArray[] = {5, -1, 2};
float currentReading;
float colorDetected[3] = {0, 0, 0};

//Color sensor
float getAvgColorReading(int RGBcolor)
{   
    float total = 0;
    for (int i = 0; i < 5; i++) //taking one sample at a time, total of 5 samples
    {   
        currentReading = ((float)LightSensor.read() - blackArray[RGBcolor]);
        total = total + currentReading;
        delay(LDRWait);
    }
    return total/5;
}

float colorArray[] = {'b', 'R', 'Y', 'G', 'B', 'P', 'R'};
char detectColor(float h, float v)
{   
    /* Every color in the color wheel has specific angle 
       we can split the color wheel into sectors each representing one of our required outputs*/
    int color = 0;
    if (v > 100)
    {   
        color++;
        if (h > 10)
        {   
            color++;
            if (h > 80)
            {   
                color++;
                if (h > 150)
                {   
                    color++;
                    if (h > 220)
                    {   
                        color++;
                        if (h > 300)
                        {   
                            color++;
                        }
                    }
                }
            }
        }
    }
    return colorArray[color];
}
float amax(float a, float b, float c)
{
    return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}
float amin(float a, float b, float c)
{
    return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}
char rgbToHsv(float r, float g, float b)
{
    float h, s, v;
    float cmax = amax(r, g, b); // maximum of r, g, b
    float cmin = amin(r, g, b); // minimum of r, g, b
    float diff = cmax - cmin;   // diff of cmax and cmin.

    //compute hue
    if (cmax == cmin)
        h = 0;
    else if (cmax == r)
        h = fmod((60 * ((g - b) / diff) + 360), 360.0);
    else if (cmax == g)
        h = fmod((60 * ((b - r) / diff) + 120), 360.0);
    else if (cmax == b)
        h = fmod((60 * ((r - g) / diff) + 240), 360.0);

    //compute saturation
    s = (diff / cmax) * 100;

    //compute value
    v = cmax;

    return detectColor(h, v);
}

char colorSensor()
{
    for (int c = 0; c <= 2; c++) //one colour at a time
    {
        RGBLED.setColor(255 * (c == 0), 255 * (c == 1), 255 * (c == 2));
        RGBLED.show();
        delay(RGBWait);
        colorDetected[c] = getAvgColorReading(c);
    }
    return rgbToHsv(colorDetected[0], colorDetected[1], colorDetected[2]);
}
