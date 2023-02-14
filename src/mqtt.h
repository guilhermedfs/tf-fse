#ifndef _MQTT_H
#define _MQTT_H

#define MQTT_TELEMETRY_PATH "v1/devices/me/telemetry"
#define MQTT_ATTRIBUTES_PATH "v1/devices/me/attributes"

void mqtt_start();
void mqtt_stop();

typedef enum {
    MQTT_TELEMETRY = 0,
    MQTT_ATRIBUTES = 1
} mqtt_message_type;

void mqtt_send_message(mqtt_message_type type, char * message);
int is_dash_buttom_on();

#endif