#pragma once

void start_exe_task();
void pause_exe_task();

void start_hold_mutex(void * pmutex);
void end_hold_mutex(void * pmutex);

void print_results();
