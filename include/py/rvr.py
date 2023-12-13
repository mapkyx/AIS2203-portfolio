import time
import socket
import logging
from sphero_sdk import SpheroRvrObserver, Colors
from threading import Thread, Event
import random
import threading

# Initialize logging
logging.basicConfig(level=logging.INFO)

# Initialize the Sphero Rvr Observer
rvr = SpheroRvrObserver()

UDP_PORT = 1234
speed = 50

shutdown_event = Event()

# Global mode variable
current_mode = ''  # Default

# Global variable to store the last command received
last_command_received = "None"

# Global variable to store battery percentage
battery_percentage = 0

# Global variables for IMU data
imu_pitch = 0.0
imu_roll = 0.0
imu_yaw = 0.0


class Colours:
    red = (255, 0, 0)
    green = (0, 255, 0)
    blue = (0, 0, 255)
    yellow = (255, 255, 0)
    white = (255, 255, 255)
    black = (0, 0, 0)
    purple = (128, 0, 128)
    orange = (255, 165, 0)
    pink = (255, 192, 203)


def setup_rvr():
    try:
        rvr.wake()
        time.sleep(1)
        rvr.led_control.set_all_leds_rgb(*Colours.yellow)
        time.sleep(1)
        rvr.led_control.set_all_leds_rgb(*Colours.green)
    except Exception as e:
        logging.error(f"Error setting up RVR: {e}")


# IMU data handler
def imu_data_handler(response):
    global imu_pitch, imu_roll, imu_yaw
    imu_pitch = response['IMU']['pitch']
    imu_roll = response['IMU']['roll']
    imu_yaw = response['IMU']['yaw']


# Enable IMU streaming
def enable_imu_streaming():
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.imu,
        handler=imu_data_handler
    )
    rvr.sensor_control.start(interval=250)  # Adjust the interval as needed


def drive_with_heading(speed, heading):
    try:
        rvr.drive_with_heading(speed, heading, 0)
    except Exception as e:
        logging.error(f"Error driving RVR: {e}")


def drive_forward():
    global current_mode
    drive_with_heading(speed, 0)
    rvr.led_control.set_all_leds_rgb(*Colours.blue)
    if current_mode == "manual control":
        current_mode = "manual"


def drive_backward():
    global current_mode
    drive_with_heading(speed, 180)
    rvr.led_control.set_all_leds_rgb(*Colours.yellow)
    if current_mode == "manual control":
        current_mode = "manual"


def turn_left():
    global current_mode
    drive_with_heading(speed, 270)
    rvr.led_control.set_all_leds_rgb(*Colours.white)
    if current_mode == "manual control":
        current_mode = "manual"


def turn_right():
    global current_mode
    drive_with_heading(speed, 90)
    rvr.led_control.set_all_leds_rgb(*Colours.white)
    if current_mode == "manual control":
        current_mode = "manual"


def stop():
    drive_with_heading(0, 0)
    rvr.led_control.set_all_leds_rgb(*Colours.red)


def manual_control():
    global current_mode
    current_mode = "manual"
    # Additional logic to switch to manual control, if necessary


def color_detected_handler(response):
    global current_mode
    detected_color = response.get('color', None)

    if detected_color == 'red':  # Assuming 'red' is a predefined color
        print("Red color detected. Stopping RVR.")
        stop()
        current_mode = "manual"  # Switch to manual mode or handle as needed


def autonomous_drive():
    global current_mode, imu_pitch, imu_roll

    current_mode = "autonomous"
    print("Starting autonomous drive")

    # Obstacle detection thresholds
    pitch_threshold = -1  # Adjust as needed
    roll_threshold = -1  # Adjust as needed

    try:

        buffer_time = 0.5  # Time in seconds to wait before rechecking IMU data
        last_check_time = time.time()

        while not shutdown_event.is_set() and current_mode == "autonomous":
            current_time = time.time()
            if current_time - last_check_time > buffer_time:
                last_check_time = current_time
            # Perform random movement
            random_movement()

            # Check IMU data for potential collisions
            if abs(imu_pitch) > pitch_threshold or abs(imu_roll) > roll_threshold:
                print("Potential obstacle detected! Changing direction.")
                change_direction()

            time.sleep(0.1)  # Short delay for loop

    except Exception as e:
        logging.error(f"Error in autonomous driving: {e}")


def random_movement():
    """ Randomly choose to drive forward, backward, left, or right. """
    choice = random.choice(['forward', 'backward', 'left', 'right'])
    if choice == 'forward':
        drive_forward()
    elif choice == 'backward':
        drive_backward()
    elif choice == 'left':
        turn_left()
    elif choice == 'right':
        turn_right()
    time.sleep(3)  # Drive for a random duration


def change_direction():
    """ Rotate the RVR or reverse its direction when an obstacle is detected. """
    choice = random.choice(['turn', 'reverse'])
    if choice == 'turn':
        # Rotate by a random angle
        rotate(random.randint(0, 360))
    elif choice == 'reverse':
        # Reverse the direction
        if last_command_received == 'forward':
            drive_backward()
        else:
            drive_forward()
    time.sleep(1)
    stop()


def rotate(angle):
    global imu_yaw
    # Calculate the new heading by adding the rotation angle to the current yaw
    new_heading = imu_yaw + angle

    # Normalize the new heading to be within 0-360 degrees
    new_heading %= 360

    # Set the RVR to drive with the new heading
    try:
        rvr.drive_with_heading(speed, new_heading, 0)
        time.sleep(1)  # Adjust the time for rotation as needed
    except Exception as e:
        logging.error(f"Error in rotating RVR: {e}")


def battery_percentage_handler(response):
    global battery_percentage
    battery_percentage = response['percentage']


def get_battery_percentage():
    global battery_percentage
    # Trigger the battery percentage update
    rvr.get_battery_percentage(handler=battery_percentage_handler)

    # Wait for the response to be handled
    time.sleep(1)

    return battery_percentage


def send_rvr_state():
    global last_command_received

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        server_address = ('192.168.63.66', 1235)  # Replace with your computer's IP address
        while not shutdown_event.is_set():
            get_battery_percentage()  # Update battery percentage
            state_message = f"{battery_percentage}%,{last_command_received},{current_mode}"
            sock.sendto(state_message.encode(), server_address)
            time.sleep(1)


def udp_server():

    global shutdown_event, last_command_received

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
            'autonomously-control': lambda: Thread(target=autonomous_drive).start(),
            "manual-control": lambda: manual_control(),
            'quit': lambda: exit()
        }

        while not shutdown_event.is_set():
            try:
                data, address = sock.recvfrom(65536)
                command = data.decode().lower()

                # Update the last command received
                last_command_received = command

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
