#include <FreeRTOS_ARM.h>
#include <Wire.h>
#include <PowerDue.h>
#include "LSM303CTypes.h"
#include "SparkFunLSM303C.h"
#include "SparkFunIMU.h"

LSM303C imu;

//FreeRTOS things
QueueHandle_t magQueue;
TaskHandle_t magHandle, readHandle;

long samp_period = 25; //ms

void threadMag(void* arg);
void threadRead(void* arg);

void threadMag(void* arg) {
  while(1) {
    vTaskResume(readHandle);
    float mag_value;
    //would check if data was available before doing this, but driver does not expose this
      // would have to change function from protected to public permissions
    //read X
    mag_value = imu.readMagX();
    if (xQueueSend(magQueue, (void*) &mag_value, (TickType_t) 0) != pdPASS)  {
      SerialUSB.println("Failed to add X value to queue");
    }

    mag_value = imu.readMagY();
    if (xQueueSend(magQueue, (void*) &mag_value, (TickType_t) 0) != pdPASS)  {
      SerialUSB.println("Failed to add Y value to queue");
    }
    
    mag_value = imu.readMagZ();
    if (xQueueSend(magQueue, (void*) &mag_value, (TickType_t) 0) != pdPASS)  {
      SerialUSB.println("Failed to add Z value to queue");
    }
    
    // delay for the duration of the sample rate
    vTaskDelay(samp_period * configTICK_RATE_HZ / 1000); 
    //unfortunately, driver as-is does not tell when a value is new; API very simple. 
    
  }
  
}

void threadRead(void* arg) {
  while (1) {
    static float magX, magY, magZ;
    
    if (uxQueueMessagesWaiting(magQueue) == 0) {
      xQueueReceive(magQueue, (void*) &magX, 0);
      xQueueReceive(magQueue, (void*) &magY, 0);
      xQueueReceive(magQueue, (void*) &magZ, 0);

      SerialUSB.print("Magnetometer: X="); SerialUSB.print(magX);
      SerialUSB.print("  Y="); SerialUSB.print(magY);
      SerialUSB.print("  Z="); SerialUSB.println(magZ);
    
    } else {
      vTaskSuspend(readHandle); //if nothing is there, then suspend this task
      // task will be resumed the next time the sensor task runs
    }
    
    
  }
}



void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(9600);

  while (~Serial);
  SerialUSB.println("Serial configured");
  
  Wire1.begin();
  Wire1.setClock(400000L);
  SerialUSB.println("Wire1 configured");

//  while(1) {
//    SerialUSB.println("test");
//  }

  if (imu.begin())
    while(1);

  if (imu.WhoAmIMag() || imu.WhoAmIAccel()) {
	SerialUSB.println("Failed to read WHO_AM_I values correctly");
	while(1);
  }

  SerialUSB.println("IMU configured, now setup RTOS tasks and queue");

  portBASE_TYPE magTask, readTask;
  magTask = xTaskCreate(threadMag, NULL, configMINIMAL_STACK_SIZE * 4, NULL , 0, &magHandle);
  if (magTask != pdPASS) {
    SerialUSB.println("Failed to setup magnetometer read task");
    while(1);
  }

  readTask = xTaskCreate(threadRead, NULL, configMINIMAL_STACK_SIZE * 4, NULL, 1, &readHandle);
  if (readTask != pdPASS) {
    SerialUSB.println("Failed to setup queue read task");
    while(1);
  }

  magQueue = xQueueCreate( (UBaseType_t) 128 * 3, (UBaseType_t) sizeof(float));
  if (magQueue == NULL) {
    SerialUSB.println("Queue failed init");
    while(1);
  }

  SerialUSB.println("Starting Scheduler");
    
}

void loop() {
  // put your main code here, to run repeatedly:
}
