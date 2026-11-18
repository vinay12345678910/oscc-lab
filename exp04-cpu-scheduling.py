from collections import deque
from dataclasses import dataclass


@dataclass(frozen=True)
class Process:
    name: str
    arrival: int
    burst: int
    priority: int


def nonpreemptive(processes, order):
    pending = list(processes)
    time = 0
    schedule = []
    while pending:
        ready = [process for process in pending if process.arrival <= time]
        if not ready:
            time = min(process.arrival for process in pending)
            continue

        process = min(ready, key=order)
        pending.remove(process)
        schedule.append((process.name, time, time + process.burst))
        time += process.burst
    return schedule


def round_robin(processes, quantum):
    if quantum <= 0:
        raise ValueError("The time quantum must be positive.")

    pending = deque(sorted(processes, key=lambda process: process.arrival))
    ready = deque()
    remaining = {process.name: process.burst for process in processes}
    schedule = []
    time = 0

    while pending or ready:
        if not ready:
            time = max(time, pending[0].arrival)
        while pending and pending[0].arrival <= time:
            ready.append(pending.popleft())

        process = ready.popleft()
        duration = min(remaining[process.name], quantum)
        schedule.append((process.name, time, time + duration))
        time += duration
        remaining[process.name] -= duration

        while pending and pending[0].arrival <= time:
            ready.append(pending.popleft())
        if remaining[process.name] > 0:
            ready.append(process)

    return schedule


def report(name, processes, schedule):
    print(f"\n{name}")
    print(" ".join(f"{pid}[{start}-{end}]" for pid, start, end in schedule))

    completion = {pid: end for pid, _, end in schedule}
    print("Process  Arrival  Burst  Priority  Finish  Turnaround  Waiting")
    waiting_times = []
    turnaround_times = []
    for process in processes:
        finish = completion[process.name]
        turnaround = finish - process.arrival
        waiting = turnaround - process.burst
        waiting_times.append(waiting)
        turnaround_times.append(turnaround)
        print(
            f"{process.name:>7}  {process.arrival:>7}  {process.burst:>5}  "
            f"{process.priority:>8}  {finish:>6}  {turnaround:>10}  {waiting:>7}"
        )

    print(f"Average waiting time = {sum(waiting_times) / len(processes):.2f}")
    print(f"Average turnaround time = {sum(turnaround_times) / len(processes):.2f}")


def main():
    processes = [
        Process("P1", arrival=0, burst=5, priority=3),
        Process("P2", arrival=1, burst=3, priority=2),
        Process("P3", arrival=2, burst=8, priority=1),
        Process("P4", arrival=3, burst=6, priority=4),
    ]
    report("FCFS", processes, nonpreemptive(processes, lambda p: p.arrival))
    report("SJF", processes, nonpreemptive(processes, lambda p: (p.burst, p.arrival)))
    report("Round Robin (quantum = 2)", processes, round_robin(processes, 2))
    report(
        "Priority (lower number runs first)",
        processes,
        nonpreemptive(processes, lambda p: (p.priority, p.arrival)),
    )


if __name__ == "__main__":
    main()
