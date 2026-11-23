# smallsh

A small Unix shell written in C. Reads with a `: ` prompt and runs three
built-in commands. The shell runs other commands as a child process
through `PATH`. It supports input and output redirection, background processes, and
custom handling of Ctrl-C and Ctrl-Z.

## Features

- **Built-in commands:** `cd`, `status`, `exit`
- **External commands:** using `PATH` (`ls`, `cat`, `wc`, `sleep`, `ps`, others),  with `fork` + `execvp`
- **Redirection:** `< input_file` and `> output_file`, in any order, but only after the command
- **Background jobs:** `&` at the end of a command runs the command without waiting; finished jobs are reaped before the next prompt
- **Comments and blank lines:** supports `#` comments; empty lines are ignored
- **Signals:**
    - Ctrl-C (`SIGINT`) is ignored by the shell and by background jobs, but terminates a foreground job
    - Ctrl-Z (`SIGTSTP`) toggles *foreground-only mode*, in which `&` is ignored

## Command syntax

```
command [arg1 arg2 ...] [< input_file] [> output_file] [&]
```

Syntax requirements: there must be spaces between words, redirections must follow commands, and
`&` for background must be last and needs a space before it.

## Building

C version is C99, C++ version (testing) is 17, and CMake is 3.24.

```bash
cmake --build build
```
This produces three executables in `build/`: `smallsh`, `parser_test`, and
`interpreter_test`.
The built files end up in cmake-build-debug and you must run (from terminal) test files there.

## Running

```bash
./cmake-build-debug/smallsh
```
Best to run in the terminal rather than the terminal of an IDE (like CLion).
In CLion's terminal, output sometimes comes out of order, which made me think I had
a bug, when I didn't.

### Example session

```
: ls
CMakeLists.txt  interpreter.c  main.c  parser.c ...
: wc < junk > junk2
: cat junk2
 9  9 78
: sleep 5 &
background pid is 48213
: status
exit value 0
:
background pid 48213 is done: exit value 0
: sleep 10
^Cterminated by signal 2
: ^Z
Entering foreground-only mode (& is now ignored)
: exit
```

## Testing

Unit tests use GoogleTest and are registered with CTest:
I ran these mostly in my IDE. You can also run the executables directly:

```bash
./cmake-build-debug/parser_test
./cmake-build-debug/interpreter_test
```

## Project structure

| File | Role                                                                                                |
|---|-----------------------------------------------------------------------------------------------------|
| `main.c` | Creates the shell state, installs signal handling, runs the read-parse-run loop                     |
| `parser.c` / `parser.h` | Reads a line and turns it into a `command_line_t` struct                                            |
| `interpreter.c` / `interpreter.h` | Shell state, built-ins, running external commands, redirection, background jobs, the Ctrl-Z handler |
| `parser_test.cc`, `interpreter_test.cc` | GoogleTest unit tests                                                                               |
| `CMakeLists.txt` | Builds each module as a library, plus the shell and the test executables                            |

Dependencies go in this direction: `main.c` --> `interpreter` --> `parser`.
Library header files needed are #include -d in the custom header file.
## Design

### Parser

`parse_input()` prints the prompt, reads a line with `fgets`, and passes the string to
`parse_line()`. The `parse_line()` function does the tokenizing. It accepts a string and 
returns a command_line_t, this way the parser can be tested.

`parse_line()` fills in `command_line_t`:

```c
typedef struct CommandLine {
    char *argv[MAX_ARGS + 1];  // NULL-terminated, ready for execvp
    int argc;
    char *input_file;
    char *output_file;
    bool is_bg;
} command_line_t;
```

`<`, `>`, `&`, are used as flags and are not saved in `argv`. Every
string is copied with `strdup`, which calls malloc on its own (alternative is allocating
memory for cmd->input_file and doing strcpy()). `free_command_line()` releases the memory 
for command_line_t (including the strdup calls), and `main` calls it after every command.

| Input | `argc` | `argv` | Input file | Output file | Background |
|---|---|---|---|---|---|
| `ls` | 1 | `ls` | — | — | no |
| `ls -l -a /tmp` | 4 | `ls -l -a /tmp` | — | — | no |
| `sort < in.txt > out.txt &` | 1 | `sort` | `in.txt` | `out.txt` | yes |

### Shell state

State that has to survive from one command to the next lives ina  state_t struct,
which is the first thing done in `main`. This struct is passed by pointer to the interpreter
for updates:

```c
typedef struct State {
    int last_fg_status;          // raw waitpid status of the last foreground job
    pid_t bg_pids[MAX_BG_PIDS];  // background jobs still running
    int bg_pids_count;           // number of background processes
    bool keep_running;           // set to false by `exit` to end the loop
} state_t;
```

I tried hard to avoid globals and state is an example. I was unable to circumvent a global for
the foreground-only-mode flag, a `static volatile sig_atomic_t` in `interpreter.c`, because the
signal handler can accept one parameter, an `int`.

### Running commands

`command_lookup()` maps `argv[0]` to a `command_t` enum (`EMPTY`, `EXIT_BI`,
`CD_BI`, `STATUS_BI`, or `EXTERNAL`), and `run_command()` chooses the run path with switch().

- **Built-ins** run in the shell's own process, such as `cd` that changes the
  shell's directory and `exit` which ends the shell.
- **External commands** go to `run_other_comm()`, which decides once, before
  `fork`, whether the command is meant and can run in the background (`&` was given and
  foreground-only mode is off). The child then does 3 things: 1. redirects (if asked) with `open`
  and `dup2`, 2. checks the signals, and 3. executes with `execvp` The parent either waits for 
  the child with `waitpid` and saves its status, or records its PID (bg) and returns to the prompt.

### Background processes

Before each prompt, `check_bg_pids()` calls `waitpid(pid, &status, WNOHANG)`
to check if any background PID have been reaped. If the returned value is the PID, the
process has finished: its status is reported and removed from the array. Removing from 
the array is done by moving the last PID into the current slot; since we now have something
unchecked here we check the same slot again. A return value of `0` means the process is still
running. 

### Signals
Here is how background and foreground processes handle Ctrl-C and Ctrl-Z:

| Process | Ctrl-C (`SIGINT`) | Ctrl-Z (`SIGTSTP`) |
|---|---|---|
| Shell | Ignored | Handled: toggles foreground-only mode |
| Foreground child | Default (terminates) | Ignored |
| Background child | Ignored | Ignored |

The shell's handling of `SIGINT` and `SIGTSTP` are set once in `main`. Children inherit the 
handling through `fork` and then alter the handling.For a  foreground process, the child 
restores the default for `SIGINT` so interruption does not occur. For all processes, the child 
sets `SIGTSTP` to be ignored before `execvp`  is called. The Ctrl-Z
handler uses only `write` instead of `printf`, as it is async-signal-safe. The handler is 
installed with `SA_RESTART` so that an interrupted `fgets` or `waitpid` can restart.

## Things I am working through

- `exit` ends the shell without terminating background jobs that are still running.
- Background jobs without explicit redirection still read from and write to the terminal instead of `/dev/null`.
- No pipes (`|`), quoting, globbing, or environment variable expansion.
- At most 512 arguments per command and 100 background jobs at a time.
- In parse_input(), need to check fgets. 
- In interepreter.c run_other_comm, in the parent at the bottom, only handling termination by SIGINT or 2.