import cv2
import numpy as np
import onnxruntime as ort
import time


# --- 1. CONFIGURATION ---
# Replace this with the actual filename of your compressed ONNX model
ONNX_MODEL_PATH = 'vangogh_mild.onnx'
# Paste the RTSP URL from your app here
RTSP_URL = 'rtsp://192.168.1.123:554/stream'  # <--- IMPORTANT: CHANGE THIS


# --- 2. PREPROCESSING & POSTPROCESSING ---


def preprocess(image_bgr):
    """
    Prepares an image frame for the ONNX model.
    - Resizes to 256x256
    - Converts from BGR (OpenCV) to RGB
    - Normalizes pixel values to the [-1, 1] range
    - Transposes dimensions to (C, H, W) for the model
    - Adds a batch dimension
    """
    resized_img = cv2.resize(image_bgr, (256, 256), interpolation=cv2.INTER_AREA)
    rgb_img = cv2.cvtColor(resized_img, cv2.COLOR_BGR2RGB)
    normalized_img = (rgb_img.astype(np.float32) / 127.5) - 1.0
    transposed_img = np.transpose(normalized_img, (2, 0, 1))
    return np.expand_dims(transposed_img, axis=0)


def postprocess(model_output):
    """
    Converts the model's raw output tensor back into a displayable image.
    - Removes the batch dimension
    - Transposes dimensions back to (H, W, C)
    - De-normalizes from [-1, 1] to [0, 255]
    - Converts from RGB back to BGR for OpenCV display
    """
    squeezed_tensor = np.squeeze(model_output)
    transposed_img = np.transpose(squeezed_tensor, (1, 2, 0))
    denormalized_img = (transposed_img + 1.0) * 127.5
    clipped_img = np.clip(denormalized_img, 0, 255).astype(np.uint8)
    return cv2.cvtColor(clipped_img, cv2.COLOR_RGB2BGR)


# --- 3. MAIN APPLICATION LOOP ---


def main():
    print("--- Live Style Transfer Demo ---")
   
    # Load the ONNX model and create an inference session
    print(f"Loading ONNX model from: {ONNX_MODEL_PATH}")
    try:
        ort_session = ort.InferenceSession(ONNX_MODEL_PATH)
        input_name = ort_session.get_inputs()[0].name
        print("Model loaded successfully.")
    except Exception as e:
        print(f"Error loading ONNX model: {e}")
        return


    # Connect to the iPhone's RTSP video stream
    print(f"Connecting to iPhone stream at: {RTSP_URL}")
    cap = cv2.VideoCapture(RTSP_URL)
    if not cap.isOpened():
        print("Error: Cannot open video stream. Check the RTSP URL and your network connection.")
        return


    print("Connection successful. Starting live demo... Press 'q' to quit.")
   
    # Create a window for the display
    cv2.namedWindow("Live GAN Demo", cv2.WINDOW_NORMAL)


    while True:
        start_time = time.time()
       
        # Read a frame from the network stream
        ret, frame = cap.read()
        if not ret:
            print("Stream ended or connection lost. Exiting.")
            break


        # Preprocess the frame for the model
        input_tensor = preprocess(frame)


        # Run inference with the ONNX model
        ort_inputs = {input_name: input_tensor}
        ort_outs = ort_session.run(None, ort_inputs)
        output_tensor = ort_outs[0]


        # Postprocess the model's output to get the final image
        gan_output = postprocess(output_tensor)
       
        # Resize the GAN output to match the original frame's height for a clean side-by-side view
        gan_output_resized = cv2.resize(gan_output, (frame.shape[1], frame.shape[0]))
       
        # Combine the original frame and the GAN output into a single window
        combined_view = np.hstack((frame, gan_output_resized))


        # Calculate and display the Frames Per Second (FPS)
        fps = 1.0 / (time.time() - start_time)
        cv2.putText(combined_view, f'FPS: {fps:.2f}', (15, 40), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 255, 0), 3)


        # Display the final result
        cv2.imshow("Live GAN Demo", combined_view)


        # Check for 'q' key press to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


    # Clean up resources
    cap.release()
    cv2.destroyAllWindows()
    print("Demo finished.")


if __name__ == "__main__":
    main()
