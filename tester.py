import sys

def main():
    args = sys.argv

    if len(args) < 3:
        print("Usage: tester.py [INPUT FILE] [OUTPUT FILE]")
        return

    with open(args[1], encoding="utf-8") as I:
        input_content = I.read()

    with open(args[2], encoding="utf-8") as O:
        output_content = O.read()

    if input_content == output_content:
        print("The two files are identical.")
    else:
        print("The two files differ.")

if __name__ == "__main__":
    main()