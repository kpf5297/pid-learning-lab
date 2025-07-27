#ifndef SAMPLE_COMMANDS_H
#define SAMPLE_COMMANDS_H
/**
 * @file sample_commands.h
 * @brief Example set of CLI command handlers.
 */
#include "uart_driver_config.h"

#if USE_CMD_INTERPRETER

#include "command_module.h"


/** Print list of supported commands. */
void cmd_help(Args *args);
/** Echo back arguments to the console. */
void cmd_echo(Args *args);
/** Add two integers and output result. */
void cmd_add(Args *args);
/** List active faults. */
void cmd_faults(Args *args);
/** Clear specific or all faults. */
void cmd_fault_clear(Args *args);
/** Enable PID control loop. */
void cmd_pid_start(Args *args);
/** Disable PID control loop. */
void cmd_pid_stop(Args *args);
/** Set proportional gain Kp. */
void cmd_kp(Args *args);
/** Set PID setpoint. */
void cmd_sp(Args *args);

/** Table of available commands. */
extern const Command cmd_list[];
/** Number of commands in ::cmd_list. */
extern const size_t  cmd_count;

#endif // USE_CMD

#endif // SAMPLE_COMMANDS_H


