typedef struct 
{
	char* type;
	char* cmd_id;
	char* content;
	char* sensor_name;
	double timestamp;
}board_msg_t;


void init_server(unsigned int, unsigned int, unsigned int);
void init_server_blocking(unsigned int, unsigned int, unsigned int);

void push_float(float, float);

void send_data_s3(char* sensor_name, float timestamp, float x, float y, float z);
void send_data_s1(char* sensor_name, float timestamp, float x);
void send_data_sb(char* sensor_name, float timestamp, char* signal_name, unsigned int x);

void set_board_handler(void (*f)(board_msg_t*));
