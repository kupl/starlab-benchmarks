import subprocess
from pathlib import Path
from .config import INFER_OUT_TAR_URL, PROJECTS_DIR


def initialize():
    tar_name = "projects.tar.gz"
    wget_command = f"wget {INFER_OUT_TAR_URL} -O {tar_name}"
    tar_command = f"tar xvzf {tar_name} -C {PROJECTS_DIR} --strip-components=1"

    subprocess.run(wget_command, shell=True).check_returncode()
    subprocess.run(tar_command, shell=True).check_returncode()
    Path(tar_name).unlink()
