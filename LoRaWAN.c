
#include "LoRaWAN.h"
#include "uart.h"

volatile bool connection = false;

volatile int strcompare = 0;
volatile int pos = 0;
volatile int count = 0; //maybe no volatile
volatile int i,n = 0;


void sending_process (int uart_num, const char *str);
void responding_process (void);
void check_connection (void);


void lorawan_connection (void) {
    const char test[] = "AT\r\n";

    uart_setup(UART_NR, UART_TX_PIN, UART_RX_PIN, BAUD_RATE);

    sending_process(UART_NR, test);
    check_connection();
}


void check_connection (void) {//maybe bool function for switch
    connection = false;
    char *ptr = malloc(sizeof(char*)*STRLEN);
    char str[STRLEN];
    printf("starting do while\n");
    do {
        connection = uart_is_readable_within_us(uart1, 500*1000);
        sleep_ms(50);
        pos = uart_read(UART_NR, (uint8_t *) str, STRLEN);
        if (pos > 0) {
            str[pos] = '\0';
            ptr = strchr(str, 'O');
            strcompare = strcmp("OK\r\n", ptr);
            sleep_ms(50);
        }
        if(count == 5) {
            printf("Module not responding\n");
            count=0;
        }
        else if (connection == false && strcompare != 0 ) {
            count++;
        }
        if (strcompare == 0 && connection == true) {
            printf("Connected to LoRa module\n");
            count = 5;

        }
    }
    while (!connection && count < 5);
    count = 0;
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

void speak_to_server (const char *str)  {
    const char Message[] = "AT+MSG=\"message\"\r\n";//pitää muuttaa stringin vastaanotto mainist

    sending_process(UART_NR, Message);
    responding_process();
    sleep_ms(50);
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
