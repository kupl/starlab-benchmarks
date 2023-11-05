from safety import C
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--safety-c", action='store_true')
    args = parser.parse_args()
    if args.safety_c:
        C.initialize()
