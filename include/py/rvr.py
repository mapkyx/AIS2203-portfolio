import time
import socket
#import struct
#import pickle
#import cv2
from sphero_sdk import SpheroRvrObserver
from infrared_control_observer import InfraredControlObserver
from sphero_sdk import SpheroRvrAsync
from sphero_sdk import RawMotorModesEnum
from threading import Thread
import asyncio

# Initialize RVR
rvr = SpheroRvrObserver()
ir_control = InfraredControlObserver(rvr)

break_loop = False
red = [0xFF, 0x0, 0x0]
blue = [0x0, 0x0, 0xFF]


async def set_all_leds(rgb_triples):
    await rvr.set_all_leds_with_32_bit_mask(
        0x3FFFFFFF,
        rgb_triples
    )
    await asyncio.sleep(.01)


def construct_blue():
    global red
    global blue
    colors = []
    for x in range(10):
        if (x % 2) == 0:
            colors.extend(red)
        else:
            colors.extend(blue)
    return colors


def construct_red():
    global red
    global blue
    colors = []
    for x in range(10):
        if (x % 2 - 1) == 0:
            colors.extend(red)
        else:
            colors.extend(blue)
    return colors


async def strobe_lights():
    lights_red = True
    while True:
        lights_red = not lights_red
        if lights_red:
            rgb_red = construct_red()
            await set_all_leds(rgb_red)
        else:
            rgb_blue = construct_blue()
            await set_all_leds(rgb_blue)
        await asyncio.sleep(0.25)


def setup_rvr():
    # Make sure RVR is awake and ready to receive commands
    rvr.wake()
    # Give RVR time to wake up
    time.sleep(2)


def drive_forward(speed=100):
    rvr.drive_with_heading(speed, 0, 0)


def drive_backward(speed=100):
    rvr.drive_with_heading(speed, 180, 0)


def turn_left(speed=100):
    rvr.drive_with_heading(speed, 270, 0)


def turn_right(speed=100):
    rvr.drive_with_heading(speed, 90, 0)


def stop():
    rvr.drive_with_heading(0, 0, 0)


#def camera_feed(server_address):
# Initialize socket
#sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Initialize camera
#camera = cv2.VideoCapture(0)  # Assuming you have the PiCamera set as the default camera

#while True:
#ret, frame = camera.read()
#if not ret:
# break

# Encode the frame in JPEG format
#_, buffer = cv2.imencode('.jpg', frame)

# You could also compress the frame to reduce bandwidth
#compressed_frame = pickle.dumps(buffer, 0)
# size = len(compressed_frame)

# Ensure that the size fits into 4 bytes
#if size > 4294967295:
#continue

# Send the size of the frame
#sock.sendto(struct.pack(">L", size), server_address)

# Send the actual frame
#sock.sendto(compressed_frame, server_address)

# Cleanup
#camera.release()
#sock.close()

async def read_ir_sensor():
    """
    Reads the IR sensor data from the RVR.
    Returns True if an obstacle is detected, False otherwise.
    """
    # Assuming the SDK provides a method to get IR sensor data
    # The function name and its usage might differ based on the SDK's documentation
    ir_sensor_data = await rvr.get_infrared_sensor_values()

    # Assuming the function returns a dictionary with sensor values
    # Here we check if any of the IR sensors detect an obstacle
    # The threshold value and sensor names depend on your RVR model and SDK
    threshold_value = 10 # Set this based on your testing
    obstacle_detected = any(value > threshold_value for value in ir_sensor_data.values())

    return obstacle_detected


def ir_message_handler(message):
    # Process the IR message to detect obstacles
    # Implement your logic here
    pass


# Autonomous control based on IR sensors (placeholder)
def setup_ir():
    # Enable IR message notification and set the handler
    ir_control.listen_for_infrared_message(ir_message_handler)


def autonomous_control():
    setup_ir()

    while True:
        # Your autonomous control logic here
        # Use ir_control methods to send or stop IR messages based on your logic
        ir_control.start_infrared_broadcasting(far_code, near_code)
        time.sleep(0.1)  # Adjust timing as needed


# Manual control - Placeholder (actual control is done in C++ program)
def manual_control():
    # This could be an empty function, as manual control is managed by the C++ program
    pass


def udp_server():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        server_address = ('', 1234)  # Listen on all available interfaces
        sock.bind(server_address)

        print("UDP Server listening on port 1234 for WASD commands.")

        command_actions = {
            'start_forward': lambda: drive_forward(),
            'stop_forward': lambda: stop(),
            'start_backward': lambda: drive_backward(),
            'stop_backward': lambda: stop(),
            'start_left': lambda: turn_left(),
            'stop_left': lambda: stop(),
            'start_right': lambda: turn_right(),
            'stop_right': lambda: stop(),
            'autonomously-control': lambda: autonomous_control(),
            'manual-control': lambda: manual_control(),
            'quit': lambda: exit()  # This will stop the server
        }

        while True:
            data, address = sock.recvfrom(4096)
            command = data.decode().lower()

            action = command_actions.get(command)
            if action:
                action()
            else:
                print(f"Unknown command: {command}")

            print(f"Received command: {command} from {address}")


def main():
    setup_rvr()

    # Start the camera feed in a separate thread
    #camera_thread = Thread(target=camera_feed, args=(('192.168.89.138', 12345),))  # Replace with your client's IP and the port
    #camera_thread.start()

    # Start the UDP server
    udp_server()

    # Wait for the camera thread to finish (if it ever does)
    #camera_thread.join()

    autonomous_control()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('\nProgram terminated with keyboard interrupt.')
    finally:
        # Clean up the RVR connection
        if rvr is not None:
            rvr.close()
