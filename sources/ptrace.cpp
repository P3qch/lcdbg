#include <cerrno>
#include <ptrace.h>
#include <sys/ptrace.h>
#include <sys/user.h>

class PtraceExcpetion {};


void ptrace_me()
{
    long r = ptrace(PTRACE_TRACEME);
    if (r == -1)
    {
        throw PtraceExcpetion();
    }
}

uint64_t ptrace_peektext(pid_t pid, uint64_t addr)
{
    errno = 0;
    uint64_t r = ptrace(PTRACE_PEEKDATA, pid, addr);
    if (r == -1 && errno != 0)
    {
        throw PtraceExcpetion();
    }
    return r;
}

void ptrace_poketext(pid_t pid, uint64_t addr, uint64_t data)
{
    if (ptrace(PTRACE_POKETEXT, pid, addr, data) == -1)
    {
        throw PtraceExcpetion();
    }
}
void ptrace_cont(pid_t pid)
{
    if (ptrace(PTRACE_CONT, pid, 0, 0))
    {
        throw PtraceExcpetion();
    }
}

user_regs_struct ptrace_getregs(pid_t pid)
{
    user_regs_struct regs;

    if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1)
    {
        throw PtraceExcpetion();
    }

    return regs;
}