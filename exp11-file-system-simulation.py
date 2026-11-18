def main():
    files = set()
    while True:
        print("\n1. Create file\n2. Search file\n3. Delete file\n4. List files\n0. Exit")
        try:
            choice = input("Enter choice: ").strip()
            if choice == "0":
                return
            if choice == "4":
                print("Files:", ", ".join(sorted(files)) or "(empty)")
                continue
            if choice not in {"1", "2", "3"}:
                print("Invalid choice.")
                continue
            name = input("Enter file name: ").strip()
        except (EOFError, KeyboardInterrupt):
            print()
            return

        if not name:
            print("Enter a nonempty file name.")
            continue

        if choice == "1":
            if name in files:
                print("File already exists.")
            else:
                files.add(name)
                print("File created.")
        elif name not in files:
            print("File not found.")
        elif choice == "2":
            print("File found.")
        else:
            files.remove(name)
            print("File deleted.")


if __name__ == "__main__":
    main()
