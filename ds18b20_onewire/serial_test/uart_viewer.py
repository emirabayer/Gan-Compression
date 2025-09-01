import serial
import numpy as np
import cv2

# --- Configuration ---
SERIAL_PORT = 'COM12'
BAUD_RATE = 115200
IMG_WIDTH = 160
IMG_HEIGHT = 120
IMG_SIZE = IMG_WIDTH * IMG_HEIGHT

# --- Main Program ---
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
print(f"Listening on {SERIAL_PORT}...")

while True:
    # Wait for the start marker
    start_marker = ser.read_until(b'START')
    if b'START' in start_marker:
        # Read exactly the number of bytes for the image
        image_data = ser.read(IMG_SIZE)
        
        # Optional: wait for end marker to ensure full frame
        # ser.read_until(b'END')

        if len(image_data) == IMG_SIZE:
            # Convert the 1D byte array to a 2D numpy array (the image)
            img = np.frombuffer(image_data, dtype=np.uint8)
            img = img.reshape((IMG_HEIGHT, IMG_WIDTH))

            # Display the image in a window
            cv2.imshow('STM32 Camera Feed', img)

    # Press 'q' to quit the viewer
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

ser.close()
cv2.destroyAllWindows()
print("Viewer closed.")