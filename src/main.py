import cv2
import numpy as np
import requests
from io import BytesIO

# Load YOLO model
net = cv2.dnn.readNet("yolov3.weights", "yolov3.cfg")  # Replace with your YOLO weights and configuration files

# Load class names
with open("coco.names", "r") as f:
    classes = f.read().strip().split('\n')

# Set confidence threshold and non-maximum suppression threshold
conf_threshold = 0.5
nms_threshold = 0.4

# Create color for each class
colors = np.random.randint(0, 255, size=(len(classes), 3), dtype="uint8")

while True:
    # Fetch the live image from the ESP32-CAM feed URL
    url = "http://192.168.0.126/"
    response = requests.get(url)
    if response.status_code == 200:
        image_bytes = BytesIO(response.content)
        image = cv2.imdecode(np.frombuffer(image_bytes.read(), np.uint8), -1)

        blob = cv2.dnn.blobFromImage(image, 1/255.0, (416, 416), swapRB=True, crop=False)
        net.setInput(blob)
        layer_names = net.getUnconnectedOutLayersNames()
        outs = net.forward(layer_names)

        boxes = []
        confidences = []
        class_ids = []

        for out in outs:
            for detection in out:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]

                if confidence > conf_threshold:
                    center_x = int(detection[0] * image.shape[1])
                    center_y = int(detection[1] * image.shape[0])
                    w = int(detection[2] * image.shape[1])
                    h = int(detection[3] * image.shape[0])

                    x = int(center_x - w / 2)
                    y = int(center_y - h / 2)

                    boxes.append([x, y, w, h])
                    confidences.append(float(confidence))
                    class_ids.append(class_id)

        indexes = cv2.dnn.NMSBoxes(boxes, confidences, conf_threshold, nms_threshold)

        if len(indexes) > 0:
            for i in indexes.flatten():
                x, y, w, h = boxes[i]
                label = str(classes[class_ids[i]])
                confidence = confidences[i]
                color = tuple(map(int, colors[class_ids[i]]))  # Ensure color is a tuple

                cv2.rectangle(image, (x, y), (x + w, y + h), color, 2)
                text = f"{label} ({confidence:.2f})"
                cv2.putText(image, text, (x, y - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

        cv2.imshow("Object Detection", image)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
