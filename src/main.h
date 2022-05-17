#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS

#define DEVEUI "70B3D57ED004F42C"
#define APPEUI "0000000000000000"
#define APPKEY "13DAB84ECF4E42F9FA8C5A264D7B12A2"



void read_temperature_humidity();
void irrigate();
void read_soil();
void write_display(char* message);
void init_components();

// function for LoRaWAN
int loramac_init(char* deui, char* aeui, char* akey);
int send_message(char* message);

