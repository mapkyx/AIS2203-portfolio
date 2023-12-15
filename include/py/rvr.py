import time
import socket
import logging
from sphero_sdk import SpheroRvrObserver, Colors, RvrStreamingServices, RawMotorModesEnum
from threading import Thread, Event
import random
import threading
import io
import picamera
from PIL import Image
import struct

# Initialize logging
logging.basicConfig(level=logging.INFO)

# Initialize the Sphero Rvr Observer
rvr = SpheroRvrObserver()

UDP_PORT1 = 1234 #Keyboard listener
UDP_PORT2 = 1235 #Camera capture client
UDP_PORT3 = 1236 #Send RVR state client

#Adjust speed between 0 and 255
speed = 100

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


def capture_and_send_images():
    global client_socket

    # UDP setup
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(2)
    host = '192.168.129.66'  # Replace with the IP of the receiver
    port = UDP_PORT2 # Replace with desired port number, make sure to adjust in c++ program too

    # Camera setup
    camera = picamera.PiCamera()
    camera.resolution = (720, 560)
    camera.framerate = 4
    time.sleep(1)  # Camera warm-up time

    stream = io.BytesIO()
    MAX_UDP_PACKET_SIZE = 65000  # Packet size

    try:
        for _ in camera.capture_continuous(stream, 'jpeg', use_video_port=True, quality=30):
            size = stream.tell()

            # Send frame size
            client_socket.sendto(struct.pack('<L', size), (host, port))

            # Split frame into larger chunks and send each chunk
            stream.seek(0)
            while size > 0:
                chunk = stream.read(min(size, MAX_UDP_PACKET_SIZE))
                client_socket.sendto(chunk, (host, port))
                size -= len(chunk)
                time.sleep(0.01)  # Delay between chunks

            # Reset stream for the next frame
            stream.seek(0)
            stream.truncate()

            time.sleep(0.05)  # Delay after sending each frame

    except KeyboardInterrupt:
        print("Camera capture stopped by user.")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        camera.close()
        client_socket.close()
        print("Camera and socket resources released.")


# IMU data handler
def imu_data_handler(response):
    global imu_pitch, imu_roll, imu_yaw
    imu_pitch = response['IMU']['pitch']
    imu_roll = response['IMU']['roll']
    imu_yaw = response['IMU']['yaw']
    print(f"IMU Data Updated - Pitch: {imu_pitch}, Roll: {imu_roll}, Yaw: {imu_yaw}")


# Enable IMU streaming
def enable_imu_streaming():
    rvr.sensor_control.add_sensor_data_handler(
        service=RvrStreamingServices.imu,
        handler=imu_data_handler
    )
    rvr.sensor_control.start(interval=250)


def control_raw_motors(left_speed, right_speed):
    left_mode = RawMotorModesEnum.forward.value if left_speed >= 0 else RawMotorModesEnum.reverse.value
    right_mode = RawMotorModesEnum.forward.value if right_speed >= 0 else RawMotorModesEnum.reverse.value

    rvr.raw_motors(
        left_mode=left_mode,
        left_speed=abs(left_speed),
        right_mode=right_mode,
        right_speed=abs(right_speed)
    )


def drive_forward():
    control_raw_motors(speed, speed)
    global current_mode
    rvr.led_control.set_all_leds_rgb(*Colours.blue)
    if current_mode == "manual control":
        current_mode = "manual"


def drive_backward():
    control_raw_motors(-speed, -speed)
    global current_mode
    rvr.led_control.set_all_leds_rgb(*Colours.yellow)
    if current_mode == "manual control":
        current_mode = "manual"


def turn_left():
    control_raw_motors(-speed, speed)
    global current_mode
    rvr.led_control.set_all_leds_rgb(*Colours.white)
    if current_mode == "manual control":
        current_mode = "manual"


def turn_right():
    control_raw_motors(speed, -speed)
    global current_mode
    rvr.led_control.set_all_leds_rgb(*Colours.white)
    if current_mode == "manual control":
        current_mode = "manual"


def stop():
    control_raw_motors(0, 0)
    rvr.led_control.set_all_leds_rgb(*Colours.red)


def manual_control():
    global current_mode
    current_mode = "manual"


def autonomous_drive():
    global current_mode, imu_pitch, imu_roll, imu_yaw

    current_mode = "autonomous"
    print("Starting autonomous drive")

    # Obstacle detection and navigation thresholds
    pitch_threshold = 10
    roll_threshold = 10
    yaw_change_threshold = 5

    last_yaw = imu_yaw  # Store the last yaw value for comparison

    try:
        while not shutdown_event.is_set() and current_mode == "autonomous":
            print(f"Current IMU - Pitch: {imu_pitch}, Roll: {imu_roll}, Yaw: {imu_yaw}")
            # Perform random movement
            random_movement()

            # Check IMU data for potential collisions
            if abs(imu_pitch) > pitch_threshold or abs(imu_roll) > roll_threshold:
                print("Potential obstacle detected based on pitch/roll! Changing direction.")
                change_direction()

            # Check for significant changes in yaw
            elif abs(imu_yaw - last_yaw) > yaw_change_threshold:
                print("Significant change in yaw detected! Adjusting course.")
                adjust_course_based_on_yaw(imu_yaw)
                last_yaw = imu_yaw  # Update the last yaw value

            time.sleep(0.1)  # Short delay for loop

    except Exception as e:
        logging.error(f"Error in autonomous driving: {e}")


def adjust_course_based_on_yaw(current_yaw):
    """ Adjusts the RVR's course based on changes in the yaw. """
    rotate(random.choice([-15, 15]))  # Rotate by a small angle


def random_movement():
    """ Randomly choose to drive forward, backward, left, or right with random speed and duration. """
    choice = random.choice(['forward', 'backward', 'left', 'right'])
    random_speed = random.randint(20, 100)  # Random speed
    random_duration = random.uniform(1, 3)  # Random duration

    if choice == 'forward':
        control_raw_motors(speed, speed)
    elif choice == 'backward':
        control_raw_motors(-speed, -speed)
    elif choice == 'left':
        control_raw_motors(-speed, speed)
    elif choice == 'right':
        control_raw_motors(speed, -speed)

    time.sleep(random_duration)
    stop()


def change_direction():
    """ Rotate the RVR or reverse its direction when an obstacle is detected. """
    rotate(random.randint(90, 270))  # Rotate by a random angle between 90 and 270 degrees
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
        server_address = ('192.168.129.66', UDP_PORT3)  # Replace with your computer's IP address
        while not shutdown_event.is_set():
            get_battery_percentage()  # Update battery percentage
            state_message = f"{battery_percentage}%,{last_command_received},{current_mode}"
            sock.sendto(state_message.encode(), server_address)
            time.sleep(1)


def udp_server():

    global shutdown_event, last_command_received

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        server_address = ('', UDP_PORT1)  # Listen on all available interfaces, with the defined port number
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
                data, address = sock.recvfrom(65000)
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

    enable_imu_streaming()

    capture_thread = Thread(target=capture_and_send_images)
    capture_thread.start()

    udp_server_thread.join()
    send_rvr_state_thread.join()
    capture_thread.join()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        logging.info('\nProgram terminated with keyboard interrupt.')
        shutdown_event.set()
    finally:
        rvr.close()