#include <Arduino.h>
#include "Task_Floodfill.h"
#include "Task_Sensor.h"

void setup()
{
    Serial.begin(115200);

    xTaskCreate (task_floodfill, "floodfill", 5120, NULL, 3, NULL);

    xTaskCreate (task_sensor, "sensor", 1024, NULL, 5, NULL);
}

void loop ()
{
}