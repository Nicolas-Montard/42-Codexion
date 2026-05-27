*This project has been created as part of the 42 curriculum by \<nmontard\>.*

# Codexion

## Description

Codexion is a concurrent simulation inspired by the classic dining philosophers problem, reimagined in a software engineering context. A set of coders sit in a circular co-working hub around a shared Quantum Compiler. Each coder alternates between three activities: compiling, debugging, and refactoring. Compiling requires holding two USB dongles simultaneously — one on each side — which are shared with neighboring coders.

The goal is to ensure that every coder compiles at least a required number of times without any of them burning out. A coder burns out if they go too long without starting a new compilation. The simulation stops either when all coders have met their compile quota, or when one burns out.

The project explores key concurrency challenges: deadlock prevention, starvation avoidance, fair resource scheduling, and precise timing — all implemented in C using POSIX threads.

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` binary. Standard Makefile targets are available: `all`, `clean`, `fclean`, `re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. Times are in milliseconds. `scheduler` must be either `fifo` or `edf`.

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max time without starting a compile before burnout (ms) |
| `time_to_compile` | Duration of the compile phase (ms) |
| `time_to_debug` | Duration of the debug phase (ms) |
| `time_to_refactor` | Duration of the refactoring phase (ms) |
| `number_of_compiles_required` | Target number of compiles per coder |
| `dongle_cooldown` | Time a dongle is unavailable after being released (ms) |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

### Example

```bash
./codexion 4 800 200 200 200 5 100 fifo
```

Expected output format:

```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
...
1505 4 burned out
```

## Blocking cases handled

### Deadlock prevention

The classic deadlock condition arises when every coder simultaneously holds one dongle and waits for the other. This is prevented by eliminating the "hold and wait" Coffman condition: a coder acquires both dongles atomically, locking them both before entering the wait loop. A coder never holds one dongle while waiting for the other — it either secures both at once or waits without holding any. Circular wait therefore cannot form.

### Starvation prevention

Under both scheduling policies, a coder waiting for a dongle is guaranteed to eventually receive it:

- **FIFO**: requests are served in arrival order, ensuring bounded waiting time regardless of the number of competing coders.
- **EDF (Earliest Deadline First)**: the coder closest to burning out is prioritized. Combined with the liveness guarantee required by the subject, no coder starves as long as parameters are feasible.

In both cases, the waiting queue is implemented as a binary min-heap, so arbitration is deterministic and O(log n).

### Dongle cooldown handling

After a coder releases a dongle, it enters a cooldown period during which it cannot be acquired. The monitor thread continuously checks elapsed time since each dongle's last release and re-marks it as available once the cooldown has passed. Upon doing so, it broadcasts on the dongle's condition variable and on both neighboring dongles, waking any coder that may now be unblocked.

### Precise burnout detection

A dedicated monitor thread polls the last compile start timestamp of each active coder and compares it against the current time. The burnout log is guaranteed to be printed within 10 ms of the actual deadline. To keep the comparison accurate, each coder's `last_compile_start` is updated atomically under its own mutex before the compile phase begins.

### Log serialization

All output is protected by a dedicated `print_lock` mutex. Any call to `printf` with a state message acquires this lock first, ensuring that no two log lines ever interleave, even under heavy thread contention.

## Thread synchronization mechanisms

### Primitives used

**`pthread_mutex_t`** is used in four distinct roles:

- `dongle.lock` — protects the dongle's state (`available`, `has_been_released`, `last_release`, queue).
- `coder_state.lock_compile_start` — protects `last_compile_start`, read by the monitor and written by the coder thread.
- `shared_info.simulation_lock` — protects `simulation_ended` and `can_start`, which are read and written by both coder threads and the monitor.
- `shared_info.print_lock` — serializes all log output.
- `thread_info.lock` — protects per-thread flags `has_started` and `thread_ended`.

**`pthread_cond_t`** is used in two places:

- `dongle.cond` — coders waiting for a dongle block on this condition. The monitor (for cooldown) and other coders (after releasing a dongle) broadcast on it.
- `shared_info.can_start_cond` — odd-indexed coders wait on this condition before starting, allowing even-indexed coders to pick up their dongles first.

### Race condition prevention

A typical compile attempt proceeds as follows:

1. The coder locks both dongles.
2. It enqueues itself in both dongles' priority queues under those locks.
3. It then acquires `simulation_lock` and enters a `pthread_cond_timedwait` loop on `dongles[0].cond`, releasing `dongles[1].lock` during the wait to allow the monitor to broadcast cooldown updates.
4. The wait condition checks both that the coder is at the head of both queues and that both dongles are available — all under held locks.
5. On wakeup, if `simulation_ended` is set, all locks are released and the thread exits cleanly.
6. Otherwise, the coder marks both dongles as unavailable, pops itself from both queues, and releases the locks before compiling.

This sequence ensures that the dongle state is never modified without the appropriate mutex held, and that no coder can observe a partially updated state.

### Monitor and coder coordination

The monitor thread runs a polling loop that calls `check_burnout`, `check_dongle_cooldown`, and `check_all_ended` in sequence. It communicates with coder threads exclusively through shared state protected by mutexes — it never calls into coder logic directly. When it sets `simulation_ended = 1` under `simulation_lock`, all coder threads will observe the change on their next lock acquisition and exit their loops cleanly. The monitor then returns, and the main thread joins all threads before freeing resources.

## Resources

### POSIX threads and concurrency

- [Usage of pthread.h library] https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/

### AI usage

Claude (Anthropic) was used during this project for the following tasks:

- Reformatting existing code to comply with the 42 norm (25-line function limit, naming conventions, style rules).
- Identifying and correcting error handling bugs in resource initialization and cleanup paths.
- Creating the README.MD

All algorithmic design, implementation choices, and final code were produced and validated by the author.