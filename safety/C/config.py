import os
from pathlib import Path

BENCHMARK_ROOT = Path((os.path.abspath(__file__))).parent
INFER_OUT_TAR_URL = "https://github.com/kupl/starlab-benchmarks/releases/download/Safety-C/infer-out.tar.gz"
PROJECTS_DIR = BENCHMARK_ROOT / "projects"
