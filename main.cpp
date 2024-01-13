#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h" // adaugat 01.12
#include <semphr.h>

//------
#include <pico/cyw43_arch.h>
#include <lwip/apps/lwiperf.h>
#include <lwip/ip4_addr.h>
#include <lwip/netif.h>
#include <lwip/sockets.h>

#include "hardware/clocks.h"

constexpr uint PIN_OUT = 0;
#define LED_PIN 14

typedef unsigned int UInt32;
const int buttonPin = 16;
int currentMillis = 1000;
const int servoPin = 0;
bool direction = true;
bool btn_state = false;
bool btn_state_server = false;
const int stopThreshold_jos = 1000;
const int stopThreshold_sus = 5;
bool flag= false;

char read_value_server[4];
int len = 4;
int mem = 0;

SemaphoreHandle_t button_signal = 0;

static void TaskButton(void *arg)
{
   while (true)
   {
      bool read_value = gpio_get(buttonPin);
      xSemaphoreTake(button_signal, portMAX_DELAY);
      btn_state = read_value;
      xSemaphoreGive(button_signal);
   }
}

static void TaskBlink(void *arg)
{
   gpio_put(LED_PIN, true);
   bool read_value= false;
   while (true)
   {
      
      xSemaphoreTake(button_signal, portMAX_DELAY);
       read_value = btn_state;
     // read_value = btn_state_server;
      xSemaphoreGive(button_signal);

      if (read_value)
      {
         gpio_put(LED_PIN, 0);
      }
      else
      {
         gpio_put(LED_PIN, 1);
      }
   }
}

static void TaskServo(void *arg)
{

   bool direction = true;
   int currentMillis = 1000;
   int servoPin = 0;
   bool read_value = false; 

   gpio_set_function(PIN_OUT, GPIO_FUNC_PWM);  /// Set the pin 0 to be PWM
    auto slice   = pwm_gpio_to_slice_num(PIN_OUT);
    auto channel = pwm_gpio_to_channel(PIN_OUT);

    pwm_set_clkdiv(slice, 256.0f);  /// Setting the divider to slow down the clock
    pwm_set_wrap(slice, 9804);      /// setting the Wrap time to 9764 (20 ms)
    pwm_set_enabled(slice, true);
   while (true)
   {
      
      xSemaphoreTake(button_signal, portMAX_DELAY);
     read_value = btn_state;
     flag = read_value;
    //  read_value = btn_state_server;
      xSemaphoreGive(button_signal);
    //  printf("read_value %d\n",read_value );
      if (read_value == 0) // Verifică dacă butonul este apăsat
      {
         // currentMillis += (direction) ? 5 : -5;
         // if (currentMillis >= 1000)
         //    direction = false;
         // if (currentMillis <= 50)
         //    direction = true;
         if(direction)
         {
            currentMillis = 2000;
            direction =false;
          //  printf("sus\n");
            pwm_set_chan_level(slice, channel, 490);  /// Setting the duty period (0.6 ms)
            vTaskDelay(900);
         }
         else {
            currentMillis = 0;
            direction =true;
           // printf("jos\n");
            pwm_set_chan_level(slice, channel, 978);  /// 
            vTaskDelay(900);
         }
      }
      
      
   }
}

// SemaphoreHandle_t button_signal = 0;

static void TaskWifi(void *pvParameters)
{
   printf("Start wifi task\n");
   if (cyw43_arch_init()) // se incerca initializarea arhitecturii Wi-Fi
   {
      printf("failed to initialise\n");
      return;
   }
   printf("Done init arch\n");

   xTaskCreate(TaskServo, "Task Servo", 1024, NULL, 2, NULL);
   xTaskCreate(TaskBlink, "Task Blink", 1024, NULL, 2, NULL);
   xTaskCreate(TaskButton, "Task Button", 1024, NULL, 2, NULL);

   cyw43_arch_enable_sta_mode(); // se configureaza modulul wi fi in modul statie
   printf("Connecting to WiFi...\n");
   if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) // se incerca conectarea la modulul wi-fi
   {
      printf("failed to connect.\n");
      exit(1);
   }
   else
   {
      printf("Connected.\n");
   }

   int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); //  initializarea unui socket si configurarea serverului
                                                               //  AF_INET - se va utiliza adresa IPv4
                                                               //  Sock_stream - se va utiliza un socket de tip TCP
                                                               //  IPPROTO_IP - indica ca se va utiliza protocolul IP
   struct sockaddr_in listen_addr;                             // se specifica adresa si portul serverului
   bzero(&listen_addr, sizeof(listen_addr));                   // se elimina orice informatie existenta in structura
   listen_addr.sin_len = sizeof(struct sockaddr_in);
   listen_addr.sin_family = AF_INET;
   listen_addr.sin_port = htons(8080);

   if (server_sock < 0)
   {
      printf("Unable to create socket: error %d", errno);
      return;
   }

   if (bind(server_sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
   {
      printf("Unable to bind socket: error %d\n", errno);
      return;
   }

   if (listen(server_sock, 3) < 0) // se asteapta 3 conexiuni
   {
      printf("Unable to listen on socket: error %d\n", errno);
      return;
   }

   printf("Starting server at %s on port %u\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), ntohs(listen_addr.sin_port));

   char buffer[10];
   int read_bytes;
   const char on_char[] = "on";
   const char off_char[] = "off";

   while (true) // ciclul pentru acceptarea conexiunilor
   {
      struct sockaddr_storage remote_addr;
      socklen_t len = sizeof(remote_addr);
      printf("Wait conncection!\n");
      int conn_sock = accept(server_sock, (struct sockaddr *)&remote_addr, &len);
      printf("Got conncection!\n");
      if (conn_sock >= 0)
      {

         read_bytes = read(conn_sock, read_value_server, 9);
         read_value_server[read_bytes] = '\0';
         printf("Message from client: %s\n", read_value_server);

         printf("From client: %s\n", read_value_server);
          if (flag)
             printf("Mesaj: se deschide bariera: \n"); 
         else
             printf("Mesaj: se inchide bariera: \n");

        /* if (strcmp(read_value_server, on_char) == 0)
         {
            xSemaphoreTake(button_signal, portMAX_DELAY);
            btn_state_server = 0;
            xSemaphoreGive(button_signal);

            vTaskDelay(100);
            xSemaphoreTake(button_signal, portMAX_DELAY);
            btn_state_server = 1;
            xSemaphoreGive(button_signal);
         }
         else if (strcmp(read_value_server, off_char) == 0)
         {
            xSemaphoreTake(button_signal, portMAX_DELAY);
            btn_state_server = 0;
            xSemaphoreGive(button_signal);

            vTaskDelay(100);
            xSemaphoreTake(button_signal, portMAX_DELAY);
            btn_state_server = 1;
            xSemaphoreGive(button_signal);
         }
         else 
         {
            xSemaphoreTake(button_signal, portMAX_DELAY);
            btn_state_server = 1;
            xSemaphoreGive(button_signal);
         }*/
         close(conn_sock);
      }
   }
}

int main()
{

   int pushedButton = 0;

   //------------------------------Start--------------------------------
   stdio_init_all();

  sleep_ms(5000);
  printf("Start\n");

   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, GPIO_OUT);

   gpio_init(servoPin);
   gpio_set_dir(servoPin, GPIO_OUT);

   gpio_init(buttonPin);
   gpio_set_dir(buttonPin, GPIO_IN);
   gpio_pull_up(buttonPin);

   //------------------------------CREAREA TASKURILOR--------------------------------
   xTaskCreate(TaskWifi, "Task Wi-Fi", 1024, NULL, 2, NULL);

   button_signal = xSemaphoreCreateMutex();
   vTaskStartScheduler();

   return 0;
}


/*
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

constexpr uint PIN_OUT = 0;
int main()
{
    stdio_init_all();
    sleep_ms(2000);                             /// Just to wait some..
    gpio_set_function(PIN_OUT, GPIO_FUNC_PWM);  /// Set the pin 0 to be PWM
    auto slice   = pwm_gpio_to_slice_num(PIN_OUT);
    auto channel = pwm_gpio_to_channel(PIN_OUT);

    pwm_set_clkdiv(slice, 256.0f);  /// Setting the divider to slow down the clock
    pwm_set_wrap(slice, 9804);      /// setting the Wrap time to 9764 (20 ms)
    pwm_set_enabled(slice, true);

   gpio_init(buttonPin);
   gpio_set_dir(buttonPin, GPIO_IN);
   gpio_pull_up(buttonPin);

    bool read_value = gpio_get(buttonPin);
    
    bool direction = false;
    if(read_value==0){
     if(direction){
    pwm_set_chan_level(slice, channel, 490);  /// Setting the duty period (0.6 ms)
    direction =false;
    sleep_ms(500);
}
else{
       pwm_set_chan_level(slice, channel, 1176);  /// Setting the duty period (2.4 ms)
       direction = true;
    sleep_ms(500);
} 
    }

   
    return 0;
}

*/
