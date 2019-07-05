#ifndef _PROTOCOL_HPP_
#define _PROTOCOL_HPP_

/* Message types */
#define MSG_COMMAND         "C"
#define MSG_RESPONSE        "R"
#define MSG_SENSOR_3_AXIS   "S3"
#define MSG_SENSOR_1_AXIS   "S1"
#define MSG_BOARD         	"B"

/* CMD_IDs */
#define CMD_CONNECT         "CONNECT" /* Not yet added to protocol */
#define CMD_DISCONNECT      "DISCONNECT" /* Not yet added to protocol */
#define CMD_BOARD_INFO      "BOARD_INFO" /* Not yet added to protocol */
#define CMD_G_STATUS        "G_STATUS"
#define CMD_SM_PACK         "SM_PACK"
#define CMD_STREAM_START    "STREAM_START"
#define CMD_STREAM_STOP     "STREAM_STOP"

#define CMD_ACCEL_POWER_MODE "ACCEL_POWER_MODE"
#define CMD_ACCEL_OUTPUT_DATA_RATE "ACCEL_OUTPUT_DATA_RATE"
#define CMD_ACCEL_RANGE "ACCEL_RANGE"
#define CMD_GYRO_POWER_MODE "GYRO_POWER_MODE"
#define CMD_GYRO_BANDWIDTH "GYRO_BANDWIDTH"
#define CMD_GYRO_RANGE "GYRO_RANGE"

/* App statuses */
#define STATUS_ACTIVE       "0"
#define STATUS_READY        "1"

/* Response messages */
#define RSP_OK              "0"
#define RSP_FAIL            "1"

/* Sensor types */
#define SENSOR_ACCELEROMETER "A"
#define SENSOR_MAGNETOMETER  "M"
#define SENSOR_GYROSCOPE     "G"

/* Delimiters */
#define DELIMITER           ":"
#define AXIS_DELIMITER      ","


/* Formated commands and expected responses */
#endif
