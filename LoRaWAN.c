
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

bool check_connection (void) {

    connection = false;
    int str_compare = 0;
    char str[STRLEN];
    do {
        connection = uart_is_readable_within_us(uart1, 500*1000);
        sleep_ms(50);
        pos = uart_read(UART_NR, (uint8_t *) str, STRLEN);
        if (pos > 0) {
            str[pos] = '\0';
            str_compare = !strcmp(str, RESPONSE_AT);
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


// remember your appkey!!!!!!
bool connect_to_server (void) {
    const char server_data[Data_array_length][STRLEN] = {"AT+MODE=LWOTAA\r\n",
                                                                        "AT+APPKEY=\"33de3cd72bc755f93ee97f9d343d677c\"\r\n",
                                                                        "AT+CLASS=A\r\n",
                                                                        "AT+PORT=8\r\n",
                                                                        "AT+JOIN\r\n"};

    for (int i = 0; i < Data_array_length - 1; ++i) {
        sending_process(UART_NR, server_data[i]);
        responding_process();
        sleep_ms(50);

        if (!connection) {
            return false;
        }
    }

    return true;
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

void responding_process (void) {
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
        connection = false;
    }
}
