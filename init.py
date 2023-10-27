from safety import C
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--safety-c", action="store_true", help="initialize safety C bugs"
    )

    args = parser.parse_args()
    if args.safety_C:
        C.initialize()
