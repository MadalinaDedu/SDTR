#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h" // adaugat 01.12
// #include "blynklib" // adaugat 01.12
#include <semphr.h>

// #include <machine.h>
// #include <network.h>
// #include <time.h>

// #include <blynk.h>
// #include <bme280.h>

// #define LED0 28
// #define LED1 27
#define LED_PIN 14
const int buttonPin = 16;
int currentMillis = 1000;
const int servoPin = 0;
bool direction = true;
const int stopThreshold_jos = 1000;  
const int stopThreshold_sus = 5;     


// setMillis este responsabilă pentru setarea nivelului PWM (în funcție de timpul de funcționare)
void setMillis(int servoPin, float millis)
{
   pwm_set_gpio_level(servoPin, (millis/20000.f)*39062.f);  // convertește timpul în milisecunde
                                                            // presupune o perioadă PWM de 20 ms (1 / 50 Hz), 
                                                            // care este frecvența comună pentru servomotoare.
}

//setServo este responsabilă pentru inițializarea și configurarea modulului PWM pentru controlul unui servomotor
void setServo(int servoPin, float startMillis)
{
   gpio_set_function(servoPin, GPIO_FUNC_PWM);          // configurează pinul GPIO pentru a fi utilizat în modulul PWM
   uint slice_num = pwm_gpio_to_slice_num(servoPin);    // returnează numărul de slice asociat unui pin GPIO specific.

   pwm_config config = pwm_get_default_config();        //furnizează o configurație implicită pentru modulul PWM.
   pwm_config_set_clkdiv(&config, 64.f);                // setează divizorul de ceas
   pwm_config_set_wrap(&config, 39062.f);               // seteaza valoarea de wrap pentru modulul PWM

   pwm_init(slice_num, &config, true);                  // inițializează modulul PWM cu configurația specificată

   setMillis(servoPin, startMillis);                    // este apelată pentru a inițializa poziția inițială a servomotorului
}

SemaphoreHandle_t button_signal = 0;

void TaskButton (){
   bool direction = true;
   int currentMillis = 1000;
   int servoPin = 0;
   while(true)
      if (gpio_get(buttonPin))          // Verifică dacă butonul este apăsat
       { 
         xSemaphoreGive(button_signal);
         printf("Buton apasat!\n");
        } else {
            printf("Butonul nu este apasat!\n");
        }
    }

// void TaskButton() {
//    bool buttonState = false;
//    while (true) {
//       bool currentButtonState = gpio_get(buttonPin);
//       if (currentButtonState != buttonState) {
//          if (currentButtonState) {
//             xSemaphoreGive(button_signal);
//             printf("Buton apasat!\n");
//          } else {
//             printf("Butonul nu este apasat!\n");
//          }
//          buttonState = currentButtonState;
//       }
//    }
// }

void vBlinkTask2() {
   while(true) {
      if (xSemaphoreTake(button_signal, portMAX_DELAY)) {
         printf("LED1 BLINK!\n");
         gpio_put(LED_PIN, 1);
         vTaskDelay(100);
      } else {
         gpio_put(LED_PIN, 0);
         vTaskDelay(100);
      }
         // printf("LED1 BLINK!\n");
         // gpio_put(LED_PIN, 1);
         // vTaskDelay(100);
         // gpio_put(LED_PIN, 0);
         // vTaskDelay(100);
   }
}

void TaskServo (){
   bool direction = true;
   int currentMillis = 1000;
   int servoPin = 0;
   while(true)
      if (xSemaphoreTake(button_signal, portMAX_DELAY)) { // Verifică dacă butonul este apăsat
            currentMillis += (direction) ? 5 : -5;
            if (currentMillis >= 1000) direction = false;
            if (currentMillis <= 50)   direction = true;
            setServo(servoPin, currentMillis);
           // gpio_put(LED_PIN, 1);
           // vTaskDelay((10)); // Așteaptă 10 ms
        } else {
           // gpio_put(LED_PIN, 0); //  dacă butonul nu este apăsat
        }
    }



void main() {
   

   int pushedButton = 0;
//------------------------------Start UART--------------------------------
   stdio_init_all();

   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, GPIO_OUT);
   gpio_put(LED_PIN, true);

   gpio_init(servoPin);
   gpio_set_dir(servoPin, GPIO_OUT);
   gpio_put(servoPin, true);

   gpio_init(buttonPin);
   gpio_set_dir(buttonPin, GPIO_OUT);
   gpio_put(buttonPin, true);


   setServo(servoPin, currentMillis);

   button_signal = xSemaphoreCreateBinary();

//----------------------------Stepper------------------------------
   // xTaskCreate(TaskStepper, "Moto", 128, NULL, 1, NULL);

//------------------------------LED--------------------------------
   xTaskCreate(TaskServo, " Task TaskServo", 128, NULL, 1, NULL);
   xTaskCreate(vBlinkTask2, "Blink Task 2", 128, NULL, 1, NULL);
   xTaskCreate(TaskButton, "TaskButton ", 128, NULL, 2, NULL);
   vTaskStartScheduler();
}


