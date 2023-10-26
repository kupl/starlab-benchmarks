import os
import signal
from time import monotonic as timer
from subprocess import Popen, PIPE, TimeoutExpired


class Ret:
    def __init__(self, stdout, stderr, return_code, time):
        self.stdout = stdout.decode()
        self.stderr = stderr.decode()
        self.return_code = return_code
        self.time = time


def execute(cmd, dir=None, env=None, timeout=1200, verbosity=0):
    if verbosity >= 1:
        print(f"EXECUTE {cmd} AT {os.path.basename(dir)}")

    start = timer()
    try:
        process = Popen(
            cmd,
            shell=True,
            stdout=PIPE,
            stderr=PIPE,
            cwd=dir,
            env=env,
            preexec_fn=os.setsid,
        )
        stdout, stderr = process.communicate(timeout=timeout)
        returncode = process.returncode
    except TimeoutExpired:
        # send signal to the process group
        os.killpg(process.pid, signal.SIGINT)
        print(f"TIMEOUT occurs during executing {cmd[:20]} at {dir}")
        stdout, stderr = b"", b""
        returncode = 124
    except OSError:
        print(f"ERROR: failed to execute {cmd} at {dir} (maybe it is too long...)")
        stdout, stderr = b"", b""
        returncode = -1

    ret = Ret(stdout, stderr, returncode, timer() - start)

    err_msg = (
        "=== Execute %s ===\n  * return_code : %d\n  * stdout : %s\n  * stderr : %s\n  * dir : %s\n"
        % (cmd, ret.return_code, ret.stdout, ret.stderr, dir)
    )
    if ret.return_code != 0:
        if verbosity >= 1:
            print(f"ERROR - FAILED TO EXECUTE {cmd} AT {os.path.basename(dir)}")
    return ret
