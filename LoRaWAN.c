
#include "LoRaWAN.h"
#include "uart.h"

//connection to lorawan
volatile bool connection = false;

volatile int pos = 0;
volatile int count = 0;


bool lorawan_connection (void) {
    const char test[] = "AT\r\n";

    // Determining uart
    uart_setup(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE);

    //send test message to uart
    sending_process(UART_NR, test);
    //responding answer from uart
    return check_connection();
}

bool check_connection (void) {
    connection = false;
    int str_compare = 0;
    char str[STRLEN];

    do {
        // give connection state
        connection = uart_is_readable_within_us(uart1, 500*1000);
        sleep_ms(50);
        //give single char
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



bool connect_to_server (void) {
    //setting up the lorawan for connection to server
    const char server_connection_data[Data_array_length][STRLEN] = {"AT+MODE=LWOTAA\r\n",
                                                                    "AT+KEY=APPKEY,\"33DE3CD72BC755F93EE97F9D343D677C\"\r\n", // remember your appkey!!!!!!
                                                                    "AT+CLASS=A\r\n",
                                                                    "AT+PORT=8\r\n",
                                                                    "AT+JOIN\r\n"};

    for (int i = 0; i < Data_array_length; ++i) {
        sending_process(UART_NR, server_connection_data[i]);
        responding_process();
        sleep_ms(50);

        if (!connection) {
            return false;
        }
    }

    return true;
}

void speak_to_server (const char* str)  {
    char Message[STRLEN];
    sprintf(Message, "AT+MSG=\"%s\"\r\n", str);

    sending_process(UART_NR, Message);
    responding_process();
    sleep_ms(50);
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
