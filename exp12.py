def total_movement(head, order):
    positions = [head, *order]
    return sum(abs(following - current) for current, following in zip(positions, positions[1:]))


def sstf(requests, head):
    pending = list(requests)
    order = []
    while pending:
        closest = min(pending, key=lambda track: abs(track - head))
        pending.remove(closest)
        order.append(closest)
        head = closest
    return order


def scan(requests, head, last_track):
    higher = sorted(track for track in requests if track >= head)
    lower = sorted((track for track in requests if track < head), reverse=True)
    if lower:
        return higher + [last_track] + lower
    return higher


def c_scan(requests, head, last_track):
    higher = sorted(track for track in requests if track >= head)
    lower = sorted(track for track in requests if track < head)
    if lower:
        return higher + [last_track, 0] + lower
    return higher


def main():
    requests = [82, 170, 43, 140, 24, 16, 190]
    head = 50
    last_track = 199
    algorithms = {
        "FCFS": requests,
        "SSTF": sstf(requests, head),
        "SCAN": scan(requests, head, last_track),
        "C-SCAN": c_scan(requests, head, last_track),
    }
    for name, order in algorithms.items():
        print(f"{name} = {total_movement(head, order)} tracks")
        print("  " + " -> ".join(str(track) for track in [head, *order]))


if __name__ == "__main__":
    main()
