import argparse
import distutils.spawn
import os
import shutil
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
DEFAULT_BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
DEPLOY_DIR = os.path.join(SCRIPT_DIR, "public")

class EmscriptenEnvironmentError(Exception):
    pass

def main(build_dir):
    emconfigure_bin = distutils.spawn.find_executable('emconfigure')
    if emconfigure_bin is None:
        print("Unable to find emconfigure binary")
        raise EmscriptenEnvironmentError

    emmake_bin = distutils.spawn.find_executable('emmake')
    if emmake_bin is None:
        print("Unable to find emmake binary")
        raise EmscriptenEnvironmentError

    try:
        os.mkdir(build_dir)
    except FileExistsError:
        pass

    os.chdir(build_dir)
    subprocess.call([emconfigure_bin, 'cmake', '..'])
    subprocess.call([emmake_bin, 'make'])

    output_dir = os.path.join(build_dir, 'web')
    built_files = [os.path.join(output_dir, f) for f in os.listdir(output_dir)]

    for file in built_files:
        shutil.copy(file, DEPLOY_DIR)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build and deply site into public directory")
    parser.add_argument('-b', '--build-dir', default=DEFAULT_BUILD_DIR)
    args = parser.parse_args()
    main(args.build_dir)