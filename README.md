# Robotic Arm
## Arduino-Based Robotic Arm

### Presentation Photo:
<div align="center">
  <img src="https://github.com/user-attachments/assets/6278c84c-629a-4eab-a5c2-bc387418545e" alt="Robotic Arm Presentation" />
</div>

This fully 3D-printed robotic arm has **3 Degrees of Freedom (DOF)** and is capable of moving through programmed paths or being remotely controlled.

---

## Hardware Components

| Component                       | Description                                     | Image                                           |
|---------------------------------|-------------------------------------------------|-------------------------------------------------|
| **Arduino Mega Board 2560**     | The main controller for the robotic arm.        | <img src="https://github.com/user-attachments/assets/e6766c17-01ea-4975-b96d-f214c8e90fee" width="189" />|
| **Servo MG996R**                | Provides precise and reliable movement for the arm. |<img src="https://github.com/user-attachments/assets/4b530668-54f5-4da7-a167-ca8fb7dc8403" width="189" /> |
| **Jumper Wires**                | Used for connections between components.        | N/A                                             |
| **NRF24L01 Radio Module**       | Enables remote control of the robotic arm.      | <img src="https://github.com/user-attachments/assets/bfef76a2-627c-448e-bd71-983fd51cfc2e" width="189" /> |
| **3D Printed Structure**        | Provides the physical frame and supports the servos. | N/A                                             |

---

## Firmware

The full code was implemented in **Arduino**, controlling the servo movement and remote control using the **NRF24L01** radio module. The communication between the controller and the robotic arm is done via pipes to ensure smooth and reliable data transfer.

---

## Software

### Inverse Kinematics (IK)
The end-point position of the robotic arm is calculated using **Inverse Kinematics (IK)**, allowing the arm to reach specified points in space. This process helps in determining the necessary angles for each joint to reach a specific target position.

### Smooth Movement with Ramp Library
The **Ramp Library** is used to ensure smooth and controlled movement of the robotic arm, minimizing jerky movements and providing more natural, precise motions during the arm's operation.

---

### Future Improvements:
- Integration of **vision systems** for object recognition and manipulation.
- Enhanced **feedback systems** using magnetic encoders and sensors to provide better control and precision.
- Upgrade of servo motors to brushless with closed loop

---
