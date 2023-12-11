import time
import socket
import logging
from sphero_sdk import SpheroRvrObserver, SerialObserverDal, RawMotorModesEnum, Colors, BatteryVoltageStatesEnum as VoltageStates
from threading import Thread, Event
import threading

# Initialize logging
logging.basicConfig(level=logging.INFO)

# Initialize the Sphero Rvr Observer
rvr = SpheroRvrObserver()

UDP_PORT = 1234
speed = 50

shutdown_event = Event()

# Global mode variable
current_mode = 'manual'  # Default

# Global variable to store battery percentage
battery_percentage = 0


def setup_rvr():
    try:
        rvr.wake()
        time.sleep(2)
    except Exception as e:
        logging.error(f"Error setting up RVR: {e}")

def drive_with_heading(speed, heading):
    try:
        rvr.drive_with_heading(speed, heading, 0)
    except Exception as e:
        logging.error(f"Error driving RVR: {e}")

def drive_forward():
    drive_with_heading(speed, 0)

def drive_backward():
    drive_with_heading(speed, 180)

def turn_left():
    drive_with_heading(speed, 270)

def turn_right():
    drive_with_heading(speed, 90)

def stop():
    drive_with_heading(0, 0, 0)

def autonomous_drive():
    global current_mode

    if "autonomously-control":
        current_mode = "autonomous"

    try:
        # Autonomous driving code here
        pass
    except Exception as e:
        logging.error(f"Error in autonomous driving: {e}")


def battery_percentage_handler(response):
    global battery_percentage  # Declare as global if you want to access this variable outside
    battery_percentage = response['percentage']


def get_battery_percentage():
    global battery_percentage
    # Trigger the battery percentage update (this could be an asynchronous call)
    rvr.get_battery_percentage(handler=battery_percentage_handler)

    # Wait for the response to be handled
    time.sleep(1)  # Adjust the sleep time as needed

    return battery_percentage

def send_rvr_state():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        server_address = ('192.168.63.66', 1235)  # Replace with your computer's IP address
        while not shutdown_event.is_set():
            # Use the global battery_percentage variable updated by the observer
            global battery_percentage
            direction = "Direction"  # Replace with actual direction logic
            state_message = f"{battery_percentage}%,{direction}"
            sock.sendto(state_message.encode(), server_address)
            time.sleep(1)

def udp_server():

    global shutdown_event

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        server_address = ('', UDP_PORT)  # Listen on all available interfaces
        sock.bind(server_address)

        logging.info("UDP Server listening on port 1234 for WASD commands.")
        print("UDP Server listening on port 1234 for WASD commands!")

        command_actions = {
            'start_forward': lambda: drive_forward(),
            'stop_forward': lambda: stop(),
            'start_backward': lambda: drive_backward(),
            'stop_backward': lambda: stop(),
            'start_left': lambda: turn_left(),
            'stop_left': lambda: stop(),
            'start_right': lambda: turn_right(),
            'stop_right': lambda: stop(),
            'autonomously-control': lambda: autonomous_drive(),
            'manual-control': lambda: manual_control(),
            'quit': lambda: exit()
        }

        while not shutdown_event.is_set():  # Check if the shutdown event is triggered
            try:
                data, address = sock.recvfrom(65536)
                command = data.decode().lower()

                action = command_actions.get(command)
                if action:
                    action()
                else:
                    print(f"Unknown command: {command}")

                print(f"Received command: {command} from {address}")

            except socket.timeout:
                continue

def main():
    global shutdown_event

    setup_rvr()

    udp_server_thread = Thread(target=udp_server)
    udp_server_thread.start()

    send_rvr_state_thread = Thread(target=send_rvr_state)
    send_rvr_state_thread.start()

    udp_server_thread.join()
    send_rvr_state_thread.join()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        logging.info('\nProgram terminated with keyboard interrupt.')
        shutdown_event.set()
    finally:
        rvr.close()