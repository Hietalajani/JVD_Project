
#include "LoRaWAN.h"
#include "uart.h"

volatile bool connection = false;

volatile int pos = 0;
volatile int count = 0;

bool lorawan_connection (void) {
    const char test[] = "AT\r\n";

    uart_setup(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE);

    sending_process(UART_NR, test);
    return check_connection();
}

bool check_connection (void) {//maybe bool function for switch

    connection = false;
    int str_compare = 0;
    char str[STRLEN];
    printf("starting do while\n");
    do {
        connection = uart_is_readable_within_us(uart1, 500*1000);
        sleep_ms(50);
        pos = uart_read(UART_NR, (uint8_t *) str, STRLEN);
        if (pos > 0) {
            str[pos] = '\0';
            str_compare = strcmp(str, RESPONSE_AT);
            sleep_ms(50);
        }
        if(count == 5) {
            printf("Module not responding\n");
            count=0;
        }
        else if (connection == false && str_compare != 0) {
            count++;
        }
        if (str_compare == 0 && connection == true) {
            printf("Connected to LoRa module\n");
            count = 5;

        }
    }
    while (!connection && count < 5);
    count = 0;

    return connection;
}

void connect_to_server (void) {
    const char Mode[] = " AT+MODE=LWOTAA\r\n";
    const char APPKEY[] = "AT+APPKEY=\"33de3cd72bc755f93ee97f9d343d677c\"\r\n";
    const char Class[] = "AT+CLASS=A\r\n";
    const char Port[] = "AT+PORT=8\r\n";
    const char Join_Request[] = "AT+JOIN\r\n";

    sending_process(UART_NR, Mode);
    responding_process();
    sleep_ms(50);

    sending_process(UART_NR, APPKEY);
    responding_process();
    sleep_ms(50);

    sending_process(UART_NR, Class);
    responding_process();
    sleep_ms(50);

    sending_process(UART_NR, Port);
    responding_process();
    sleep_ms(50);

    sending_process(UART_NR, Join_Request);
    responding_process();
    sleep_ms(50);
}

void speak_to_server (const char* str)  {
    char* message;

    message_adaption(&message, str);
    sending_process(UART_NR, message);

    if (message != NULL) {
        free(message);
        message = NULL;
        sleep_ms(50);
    }

    responding_process();
    sleep_ms(50);
}

void message_adaption (char** result, const char* str) {
    const char Message[] = "AT+MSG=\"message\"\r\n";
    const char* substr = "message";

    int index = strstr(Message, substr) - Message;
    int old_size = strlen(substr);
    int new_size = strlen(Message) - old_size + strlen(str);

    *result = (char*)malloc(new_size + 1);

    strncpy(*result, Message, index);
    strncpy(*result + index, str, strlen(str));
    strncpy(*result + index + strlen(str), Message + index + old_size, strlen(Message) - (index + old_size));

    (*result)[new_size] = '\0';
}

void sending_process (int uart_num, const char *str) {
    uart_send(uart_num, str);
}


void responding_process (void) {//maybe bool function for switch
    char str[STRLEN];
    connection = uart_is_readable_within_us(uart1, 500*1000);
    if (connection == true) {
        sleep_ms(50);
        pos = uart_read(UART_NR, (uint8_t *) str, STRLEN);
        if (pos > 0) {
            str[pos] = '\0';
            printf("%s\n", str);
        }
    }
    else{
        printf("Module stopped responding\n");
    }
}
