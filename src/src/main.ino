#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <stdint.h>
#include <esp_sleep.h>

// CONSTANTES

// ----WIFI----
//const char* ssid = "Cuarto";
//const char* pass = "19986116";
const char* ssid = "HUAWEI-IoT";
const char* pass = "ORTWiFiIoT";

//      ------

// ----THINGSBOARD----
const char* mqtt_server_host = "demo.thingsboard.io";
const int mqtt_server_port = 1883;
const char* thingsboard_device_token = "eaWYLcMhOO0mjuT6VA7f";
//      ------

// ----GPIO----
const int pin_servo = 12;  // GPIO12
const int pin_dt = 14;      // GPIO14
const int pin_clk = 15;    // GPIO15
const int pin_anem = 5;    // GPIO5
const int pin_scl = 22;    //GPIO22
const int pin_sda = 21;    //GPIO21
const int pin_tope_inferior = 18; //GPIO18
const int pin_tope_superior = 19; //GPIO19
//      ------

// =====================================================================

// VARIABLES

// ----ENCODER----
int estado_actual_clk;
int estado_anterior_clk;
int contador = 0;
//      ------

// ----ANEMOMETRO----
unsigned long tiempo_vuelta;
unsigned long tiempo_vuelta_inicio;
bool contando_vuelta = false;
bool se_destapo = false;
//      ------

// ----RESGUARDO----
bool resguardo = false;
bool tope_superior = false;
bool tope_inferior = false;
//      ------

// ----SENSOR----
int presion_nivel_mar = 1013;
uint16_t direccion = 0;
uint16_t temperatura = 0;
uint16_t humedad = 0;
uint16_t presion = 0;
uint16_t viento = 0;
//      ------

// ----DATOS----
DynamicJsonDocument mensaje(256);
int delay_mensajes = 10000;
unsigned long tiempo_ultimo_mensaje = millis();
//      ------

// ----DIA----
bool es_dia = true;
int tiempo_ultimo_sleep = millis();
//      ------

// =====================================================================

// Inicializo servo

Servo servo;

// =====================================================================

// Inicializo el sensor humedad-temperatura-presion

Adafruit_BME280 sensor;

// =====================================================================

// Crear instancias de las clases de la librería WiFi y PubSubClient

WiFiClient objeto_wifi;
PubSubClient objeto_mqtt(objeto_wifi);

// =====================================================================

void setup() {

  // Inicio monitor serial
  Serial.begin(9600);

  // Inicio wifi
  setup_wifi();

  // Inicio conexión IoT
  objeto_mqtt.setServer(mqtt_server_host, mqtt_server_port);
  objeto_mqtt.setCallback(callback_mqtt);
  conectar_mqtt();

  // Defino modos
  pinMode(pin_servo, OUTPUT);
  pinMode(pin_clk, INPUT);
  pinMode(pin_dt, INPUT);
  pinMode(pin_anem, INPUT);
  pinMode(pin_tope_inferior, INPUT_PULLUP);
  pinMode(pin_tope_superior, INPUT_PULLUP);

  // Seteo encoder
  estado_anterior_clk = digitalRead(pin_clk);

  // Seteo el servo
  servo.attach(pin_servo);

  // Seteo el sensor
  if (sensor.begin(0x76)) {
    Serial.println("Se inicio el sensor");
  }

  // Evaluo los topes
  tope_inferior = !digitalRead(pin_tope_inferior);
  tope_superior = !digitalRead(pin_tope_superior);
}

void loop() {
  // Retomar la conexion si es necesario
  if(!objeto_mqtt.connected()){
    Serial.println("Retomando conexion THINGSBOARD");
    conectar_mqtt();
  }
  
  // Lectura encoder
  check_encoder();

  // Lectura anemómetro
  check_anemometro();

  // Lectura sensor
  check_sensor();

  // Movimiento resguardo
  check_resguardo();

  // Envia la telemetria
  enviar_telemetria();

  // Ahorro de energia
  if(!es_dia && (millis() - tiempo_ultimo_sleep > 10000)){
    ahorro_energia();
  }

  objeto_mqtt.loop();
}

// =====================================================================

// Iniciación del WiFi
void setup_wifi() {
  delay(10);
  Serial.print("Conectando a la red WIFI ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);  // Indica el modo cliente
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado al WIFI!, dirección IP: ");
  Serial.println(WiFi.localIP());
}

// =====================================================================

// Iniciar/retomar la conexión con el servidor MQTT
void conectar_mqtt() {
  while (!objeto_mqtt.connected()) {
    if(WiFi.status() != WL_CONNECTED){
      Serial.println("Retomando conexion WIFI");
      setup_wifi();
    }
    Serial.print("Conectando con el servidor MQTT...");
    if (objeto_mqtt.connect("dispositivo1", thingsboard_device_token, thingsboard_device_token)) {
      Serial.println("Conectado");
      objeto_mqtt.subscribe("v1/devices/me/attributes");
      objeto_mqtt.subscribe("v1/devices/me/rpc/request/+");
    } else {
      Serial.println("Conexión fallida");
      Serial.println(objeto_mqtt.state());
      Serial.print("Se reintentará en 3 segundos.");
      delay(3000);
    }
  }
}

// =====================================================================

// Callback para manejar mensajes MQTT
void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  String topico = String(topic);
  if (topico.startsWith("v1/devices/me/attributes")) {
    deserializeJson(mensaje, payload);
    if (mensaje.containsKey("Resguardo")) {
      resguardo = mensaje["Resguardo"];
    }
    if(mensaje.containsKey("EsDia")){
      es_dia = mensaje["EsDia"];
    }
  }
}

// =====================================================================

// Enviar datos al servidor
void enviar_telemetria(){
  
  if(millis() - tiempo_ultimo_mensaje > delay_mensajes){
    tiempo_ultimo_mensaje = millis();
    Serial.println("Se estan enviando datos");
    DynamicJsonDocument resp(256);
    resp["Real_Humedad"] = humedad;
    resp["Real_Presion"] = presion;
    resp["Real_Temperatura"] = temperatura;
    resp["Real_VelocidadViento"] = viento;
    resp["Real_DirecViento"] = direccion;
    char buffer[256];
    serializeJson(resp, buffer);
    objeto_mqtt.publish("v1/devices/me/telemetry", buffer);
  }
}
// =====================================================================

// Leer la direccion del viento
void check_encoder(){
  estado_actual_clk = digitalRead(pin_clk);
  //Si cambio CLK entonces giro para algun lado tengo que ver para que lado giro dependiendo de DT
  
  if((estado_actual_clk != estado_anterior_clk) ){
    if(digitalRead(pin_dt) != estado_actual_clk){
      // Se movio antihorario
      contador --;      
    }else{
      // Se movio horario
      contador ++;
    }
  }
  if(contador >= 40 || contador <= -40){
    contador = 0;
  }
  estado_anterior_clk = estado_actual_clk;
  actualizar_direccion();
}
// Funcion auxiliar de check_encoder
void actualizar_direccion(){
  int valor = contador * 9;
  if(valor < 0){
    valor = valor + 360;
  }
  direccion = valor;
}

// =====================================================================

// Lee la velocidad del viento
void check_anemometro(){
  if (digitalRead(pin_anem) == 0 && !contando_vuelta) {
    tiempo_vuelta_inicio = millis();
    contando_vuelta = true;
  } else if (digitalRead(pin_anem) == 0 && contando_vuelta && se_destapo) {
    tiempo_vuelta = millis() - tiempo_vuelta_inicio;
    contando_vuelta = false;
    se_destapo = false;
    /* De donde sale el valor 300 ?
    25cm circunferencia -> 1/3 son 8.333cm = 0.0833m = 0.00008333km
    tiempo de vuelta esta en milisegundos -> 1/1000 segundos = 1/60000 minutos = 1/ 3600000 horas 
    0.00008333/(1/3.600.000) = 300 mas o menos
    */
    viento =  300 / tiempo_vuelta;
  }
  if (contando_vuelta && (digitalRead(pin_anem) == 1)) {
    se_destapo = true;
  }

  // Para cubrir el caso que se queda quieto entonces no actualiza el valor de viento.
  if(millis() - tiempo_vuelta_inicio > 2000){
    contando_vuelta = false;
    viento = 0;
  }
}
// =====================================================================

// Lee los datos del sensor
void check_sensor(){
  temperatura = sensor.readTemperature();
  humedad = sensor.readHumidity();
  presion = sensor.readPressure() / 100;
}
// =====================================================================

// Maneja la logica en el movimiento del resguardo
void check_resguardo(){
  // Va negado porque como es PULLUP siempre da 1 a menos de que este presionado.
  tope_inferior = !digitalRead(pin_tope_inferior);
  tope_superior = !digitalRead(pin_tope_superior);
  if (resguardo && !tope_inferior){
    servo.write(110);    
  }else if(!resguardo && !tope_superior){
    servo.write(70);    
  }else{
    servo.write(90);
  }
}
// =====================================================================

// Ahorro de energia
void ahorro_energia(){
  // Configura el modo Light Sleep
  if(digitalRead(pin_clk) == 0){
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1);
  }else{
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 0);
  }  
  esp_sleep_enable_timer_wakeup(3600000000); // tiempo de sleep 1 hora
  Serial.println("Entrando en modo Sleep");
  delay(100);
  esp_light_sleep_start(); // Inicia el modo Sleep
  Serial.println("Despertando del modo Sleep");
  tiempo_ultimo_sleep = millis();
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_EXT0);
  //delay(100);
}
// =====================================================================
