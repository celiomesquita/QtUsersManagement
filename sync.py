import os
import subprocess

def run_rsync(command):
    try:
        subprocess.run(command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")

EXCLUDES = [
    "*.o",
    "*.d",
    "obj/",
    "dep/"
]

RSYNC_OPTIONS = "-h --progress --stats -rtgoPlD --update --delete"

APP_NAME = "QtUsersManagement"

def build_exclude_args(excludes):
    return ' '.join([f"--exclude={item}" for item in excludes])

# sshpass -p '@3r010g14b'
def main():
    exclude_args = build_exclude_args(EXCLUDES)
    operations = {
        1: ("Backup from AeroLogLab", f"rsync {RSYNC_OPTIONS} {exclude_args}  /home/celio/Projects/{APP_NAME}/      /media/celio/Celio31GB/{APP_NAME}/"),
        2: ("Recover to AeroLogLab",  f"rsync {RSYNC_OPTIONS} {exclude_args}  /media/celio/Celio31GB/{APP_NAME}/    /home/celio/Projects/{APP_NAME}/"),
        3: ("Backup from Notebook",   f"rsync {RSYNC_OPTIONS} {exclude_args}  /home/celio/Projects/{APP_NAME}/      /media/celio/Celio31GB/{APP_NAME}/"),
        4: ("Recover to Notebook",    f"rsync {RSYNC_OPTIONS} {exclude_args}  /media/celio/Celio31GB/{APP_NAME}/    /home/celio/Projects/{APP_NAME}/"),
        5: ("Backup from Server",     f"rsync {RSYNC_OPTIONS} {exclude_args}  /home/celio/Projects/{APP_NAME}/      /media/celio/Celio31GB/{APP_NAME}/"),
        6: ("Recover to Server",      f"rsync {RSYNC_OPTIONS} {exclude_args}  /media/celio/Celio31GB/{APP_NAME}/    /home/celio/{APP_NAME}/"),
        7: ("Backup from LETA",       f"rsync {RSYNC_OPTIONS} {exclude_args}  /home/pesquisador/Projects/{APP_NAME}/      /media/pesquisador/Celio31GB/{APP_NAME}/"),
        8: ("Recover to LETA",        f"rsync {RSYNC_OPTIONS} {exclude_args}  /media/pesquisador/Celio31GB/{APP_NAME}/    /home/pesquisador/{APP_NAME}/"),

    }

    print("Select an operation:")
    for key, (description, _) in operations.items():
        print(f"{key}: {description}")

    choice = int(input("Enter the number of the operation: "))
    
    if choice in operations:
        description, command = operations[choice]
        print(f"Executing: {description}")
        run_rsync(command)
    else:
        print("Invalid choice.")

if __name__ == "__main__":
    main()
