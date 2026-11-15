from threading import Lock, Thread


def main():
    counter = 0
    lock = Lock()

    def increment():
        nonlocal counter
        for _ in range(5):
            with lock:
                counter += 1
                print("Counter =", counter)

    threads = [Thread(target=increment) for _ in range(2)]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()

    print("Final Counter =", counter)


if __name__ == "__main__":
    main()
