from collections import deque


def fifo(pages, capacity):
    frames = deque()
    faults = 0
    for page in pages:
        if page in frames:
            continue
        faults += 1
        if len(frames) == capacity:
            frames.popleft()
        frames.append(page)
    return faults


def lru(pages, capacity):
    frames = []
    faults = 0
    for page in pages:
        if page in frames:
            frames.remove(page)
        else:
            faults += 1
            if len(frames) == capacity:
                frames.pop(0)
        frames.append(page)
    return faults


def optimal(pages, capacity):
    frames = []
    faults = 0
    for index, page in enumerate(pages):
        if page in frames:
            continue

        faults += 1
        if len(frames) == capacity:
            future = pages[index + 1:]
            victim = max(
                frames,
                key=lambda candidate: (
                    future.index(candidate) if candidate in future else len(future)
                ),
            )
            frames.remove(victim)
        frames.append(page)
    return faults


def main():
    pages = [1, 2, 3, 1, 4, 5]
    capacity = 3
    print("References:", *pages)
    print("Frames:", capacity)
    print("FIFO Page Faults =", fifo(pages, capacity))
    print("LRU Page Faults =", lru(pages, capacity))
    print("Optimal Page Faults =", optimal(pages, capacity))


if __name__ == "__main__":
    main()
