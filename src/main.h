#ifndef MAIN_FUNCTIONS
#define MAIN_FUNCTIONS
#endif

void read_temperature_humidity(void);
void irrigate(void);
void read_soil(void);
void write_display(char* message);
int init_components(void);

// function for LoRaWAN
int loramac_init(void);
int send_message(char* message);



// function for LoRaWAN
int loramac_init(char* deui, char* aeui, char* akey);
int send_message(char* message);

