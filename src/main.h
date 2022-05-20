#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS
#endif


void* read_temperature_humidity(void* arg);
void* read_soil(void* arg);
void* display_logic(void *arg);
void irrigate(int irrigate);
void write_display(char* message);
int init_components(void);

// function for LoRaWAN
static int loramac_init(void);
static void send_message(char* message);

//sprintf(json,"{\"temperature\": \"%f\", \"location\": \"%s\",\"unixTime\": \"%s\"}",temperature,location,strCurrTime);

