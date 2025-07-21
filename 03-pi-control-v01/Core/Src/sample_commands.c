/*
 * sample_commands.c
 *
 *  Created on: Jul 20, 2025
 *      Author: kevinfox
 */
#include "sample_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pwm.h"

// Shared UART driver instance provided by application
extern uart_drv_t *shared_uart;
extern volatile bool pid_enabled;
extern float pid_kp;
extern float pid_setpoint;
extern PwmChannel_t pwm;

// Helper to send strings over UART (blocking with 100ms timeout)
static void send_str(const char *s) {
    if (shared_uart) {
        uart_send_blocking(shared_uart, (uint8_t *)s, strlen(s), 100);
    }
}

// 'help' command: list all available commands
void cmd_help(Args *args) {
    send_str("Available commands:\r\n");
    for (size_t i = 0; i < cmd_count; i++) {
        send_str("  ");
        send_str(cmd_list[i].name);
        send_str("\r\n");
    }
}

// 'echo' command: repeat back provided parameters
void cmd_echo(Args *args) {
    for (int i = 1; i < args->argc; i++) {
        send_str(args->argv[i]);
        if (i < args->argc - 1) send_str(" ");
    }
    send_str("\r\n");
}

// 'add' command: add two integers and print result
void cmd_add(Args *args) {
    if (args->argc != 3) {
        send_str("Usage: add <a> <b>\r\n");
        return;
    }
    int a = atoi(args->argv[1]);
    int b = atoi(args->argv[2]);
    char buf[32];
    snprintf(buf, sizeof(buf), "Sum: %d\r\n", a + b);
    send_str(buf);
}

// 'pid_start' command: enable PID loop
void cmd_pid_start(Args *args) {
    pid_enabled = true;
    send_str("PID started\r\n");
}

// 'pid_stop' command: disable PID loop
void cmd_pid_stop(Args *args) {
    pid_enabled = false;
    Pwm_setDuty(&pwm, 0);
    send_str("PID stopped\r\n");
}

// 'kp' command: set proportional gain
void cmd_kp(Args *args) {
    if (args->argc != 2) {
        send_str("Usage: kp <value>\r\n");
        return;
    }
    pid_kp = strtof(args->argv[1], NULL);
    send_str("OK\r\n");
}

// 'sp' command: set PID setpoint
void cmd_sp(Args *args) {
    if (args->argc != 2) {
        send_str("Usage: sp <value>\r\n");
        return;
    }
    char *endptr;
    float value = strtof(args->argv[1], &endptr);
    if (endptr == args->argv[1] || *endptr != '\0') {
        send_str("Error: Invalid number format\r\n");
        return;
    }
    if (value < 0.0f || value > 100.0f) {
        send_str("Error: Value out of range (0.0 - 100.0)\r\n");
        return;
    }
    pid_setpoint = value;
    send_str("OK\r\n");
}

// Define command table and expose to interpreter
const Command cmd_list[] = {
    { "help", cmd_help },
    { "echo", cmd_echo  },
    { "add",  cmd_add   },
    { "pid_start", cmd_pid_start },
    { "pid_stop",  cmd_pid_stop  },
    { "kp",        cmd_kp        },
    { "sp",        cmd_sp        },
};
const size_t cmd_count = sizeof(cmd_list) / sizeof(cmd_list[0]);

