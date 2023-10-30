import numpy as np  # importing numpy. Please note that 4 <= 7, thus numpy is a dependency
import cv2
import serial
import time
import winsound # just so R2-D2 can talk to us :3


# Load the video feed
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

# Set the resolution to 720p
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

# loading the model
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

moving_x = False
moving_y = False
winsound.PlaySound('facial_recognition\9acknowledged.wav', winsound.SND_ASYNC)


# Main write function, uses The Force and some Dathomirian magic to move the servo
# Here be dragons
def write_center(x_face, y_face):
    # use the force
    str_max_x = 90
    str_min_x = 82
    str_max_y = 92
    str_min_y = 82

    # default force
    x_force = 85
    y_force = 85

    # the happy place
    center_vals = (640, 360)
    delta = (x_face-center_vals[0], y_face-center_vals[1])
    threshold = 80 #below this don't move
    global moving_x
    global moving_y
    

    if abs(delta[0]) > threshold:
        if delta[0] < 0:
            x_force = str_max_x
        if delta[0] > 0:
            x_force = str_min_x
        
        moving_x = True
        

    if abs(delta[1]) > threshold:
        
        if delta[1] < 0:
            y_force = str_max_y
        if delta[1] > 0:
            y_force = str_min_y
        
        moving_y = True
        
    
    data = None
    if moving_x or moving_y:
        message = bytes("A"+f"{x_force}".zfill(3)+f"{y_force}".zfill(3), 'utf-8') 
        # We settled on a protocol that uses the letter A to indicate that the message is for the servo
        
        arduino.write(message)
        data = arduino.readline()
        time.sleep(0.05)
        

    if abs(delta[0]) < threshold:
        
        x_force = 85
        moving_x = False
        

    if abs(delta[1]) < threshold:
        y_force = 85
        moving_y = False

    time.sleep(0.1)
    return data


arduino = serial.Serial(port='COM7', baudrate=38400, timeout=.1)
cntr = 0
while True:
    ret, frame = cap.read()
    grayish = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) # convert to grayscale
    faces = face_cascade.detectMultiScale(grayish, 1.3, 5) # detect faces bases on a trained model
    loc = (0, 0)  # Location of the face, (x, y), start from center
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 5) # draw a rectangle around the face
        loc = (loc[0] + (x + w/2),loc[1] + (y + h/2)) # update the location of the face


    if len(faces) > 0: # if there are faces, move the servo
        cntr = 0

        # Average the location of the faces
        loc = (loc[0]/len(faces), loc[1]/len(faces))

        # servo control
        val = write_center(loc[0], loc[1])
        #  t(val)
    cv2.imshow('frame', frame)

    if len(faces) == 0: # if there are no faces, stop the servo
        cntr += 1
        #  t(cntr)

        val = write_center(640, 320)
        #  t(val)

        # R2-D2 speaks
        if cntr == 1:
            winsound.PlaySound('facial_recognition\9.wav', winsound.SND_ASYNC)

        if cntr%15 == 0:
            winsound.PlaySound('facial_recognition\8.wav', winsound.SND_ASYNC)
        if cntr%19 == 0:
            winsound.PlaySound('facial_recognition\worried.wav', winsound.SND_ASYNC)
    
    # Quit the program
    if cv2.waitKey(1) == ord('q'):
        break
    

cap.release()
cv2.destroyAllWindows()