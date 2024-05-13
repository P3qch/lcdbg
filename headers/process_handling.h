#pragma once
#ifndef _H_PTRACE
#define _H_PTRACE

#include <sys/user.h>
#include <sys/ptrace.h>
#include <cstdint>
#include <unistd.h>

void ptrace_s(enum __ptrace_request request, pid_t pid,
                   void *addr, void *data);
void ptrace_me();
uint64_t ptrace_peektext(pid_t pid, uint64_t addr);
void ptrace_poketext(pid_t pid, uint64_t addr, uint64_t word);
void ptrace_cont(pid_t pid);
user_regs_struct ptrace_getregs(pid_t pid);


#endif
