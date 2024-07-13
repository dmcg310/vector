import os
import subprocess
import sys
import platform
import shutil
from colorama import init, Fore, Style

# Initialize colorama
init(autoreset=True)


def run_command(command):
    try:
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(Fore.RED + f"Error: {e}")
        sys.exit(1)


def install_vcpkg(vcpkg_path):
    vcpkg_dir = os.path.join(vcpkg_path, "vcpkg")
    if os.path.exists(vcpkg_dir):
        print(Fore.YELLOW + "vcpkg already installed.")
        return

    print(Fore.GREEN + "Cloning vcpkg...")
    run_command(
        f"git clone https://github.com/microsoft/vcpkg.git {vcpkg_dir}")

    print(Fore.GREEN + "Bootstrapping vcpkg...")
    if platform.system() == "Windows":
        run_command(os.path.join(vcpkg_dir, "bootstrap-vcpkg.bat"))
    else:
        run_command(os.path.join(vcpkg_dir, "bootstrap-vcpkg.sh"))


def install_dependencies(vcpkg_path):
    vcpkg_executable = os.path.join(vcpkg_path, "vcpkg", "vcpkg")
    if platform.system() == "Windows":
        vcpkg_executable += ".exe"

    print(Fore.GREEN + "Installing dependencies...")
    run_command(
        f"{vcpkg_executable} install glfw3 glm glad imgui[opengl3-binding] imgui[docking-experimental] imgui[glfw-binding]"
    )


def build_project(vcpkg_path, build_type):
    os.makedirs("build", exist_ok=True)
    os.chdir("build")

    cmake_toolchain_file = os.path.join(
        vcpkg_path, "vcpkg", "scripts", "buildsystems", "vcpkg.cmake")

    cmake_prefix_path = os.path.join(
        vcpkg_path, "installed", platform.system().lower() + "-x64", "lib", "cmake")

    print(Fore.GREEN +
          f"Configuring project with CMake for {build_type} build...")
    run_command(
        f"cmake -DCMAKE_BUILD_TYPE={build_type} -DCMAKE_TOOLCHAIN_FILE={cmake_toolchain_file} -DCMAKE_PREFIX_PATH={cmake_prefix_path} .."
    )

    print(Fore.GREEN + "Building project...")
    if platform.system() == "Windows":
        run_command("cmake --build .")
    else:
        run_command("make")


def check_dependencies():
    required_commands = ["cmake", "git"]
    if platform.system() != "Windows":
        required_commands.append("make")

    missing_commands = [
        cmd for cmd in required_commands if not shutil.which(cmd)]

    if missing_commands:
        print(
            Fore.RED + f"Missing dependencies: {', '.join(missing_commands)}")
        install = input(
            Fore.YELLOW + "Do you want to install missing dependencies? (y/n): ")
        if install.lower() == 'y':
            if platform.system() == "Linux":
                run_command("sudo apt-get install -y " +
                            " ".join(missing_commands))
            elif platform.system() == "Darwin":
                run_command("brew install " + " ".join(missing_commands))
            else:
                print(Fore.RED + "Please install the missing dependencies manually.")
                sys.exit(1)


def main():
    print(Fore.CYAN + "Welcome to the Vector setup script!")

    vcpkg_installed = input(
        Fore.YELLOW + "Is vcpkg already installed? (y/n): ").strip().lower()
    if vcpkg_installed == 'n':
        vcpkg_path = input(
            Fore.YELLOW + "Enter the path where you want to install vcpkg: ").strip().rstrip("\\/")
        install_vcpkg(vcpkg_path)
    else:
        vcpkg_path = input(
            Fore.YELLOW + "Enter the path where vcpkg is installed: ").strip().rstrip("\\/")

    check_dependencies()

    install_dependencies(vcpkg_path)

    build_type = input(
        Fore.YELLOW + "Enter the build type (Debug/Release): ").strip().capitalize()
    build_project(vcpkg_path, build_type)

    print(Fore.CYAN + "Build complete. To run the project, navigate to vector/build and run the appropriate executable for your platform.")
    if platform.system() == "Windows":
        print(Fore.CYAN + "Debug: .\\application\\Debug\\application.exe")
        print(Fore.CYAN + "Release: .\\application\\Release\\application.exe")
    else:
        print(Fore.CYAN + "./application/application")


if __name__ == "__main__":
    main()
