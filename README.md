# Tec Class - Classroom Automation and Attendance Control System

## 📘 Introduction
**Tec Class** is a project focused on automating student attendance and managing classroom equipment to improve the efficiency of the learning environment. By implementing **RFID technology**, the system records student presence, eliminating the time spent on manual attendance and enhancing classroom management.

## 🎯 Objective
The primary goal of **Tec Class** is to **optimize classroom time** by automating attendance and controlling various classroom devices, such as lights, projectors, and fans, based on real-time data.

## ⚙️ Features
- **Automated Attendance**: Uses RFID tags to record student and teacher presence, updating in real-time.
- **Classroom Automation**: Controls lights, fans, and projectors based on student presence and environmental conditions (temperature).
- **Energy Efficiency**: Lights and fans are turned off when the classroom is empty, contributing to reduced energy consumption.
- **RFID Integration**: Automatically identifies students entering or leaving the classroom and updates the system's attendance records.

## 🚀 Technologies Used
- **Hardware**: 
  - Arduino Mega 2560
  - Ethernet Shield W5100
  - RFID RC522 Sensor
  - DHT11 Temperature and Humidity Sensor
  - Solid-State Relays
- **Software**: 
  - C++ (Arduino programming)
  - HTML, CSS (Web interface)
  - PHP (Back-end)
  - MySQL (Database)
- **Automation**: Integration with sensors and relays for classroom control.

## 💻 How to Run the Project
To run the project locally, follow these steps:

1. **Clone the repository**:
    ```bash
    git clone https://github.com/leostall/tec-class.git
    ```

2. **Install dependencies**:
    Navigate to the project directory and set up the environment by installing the required libraries.

3. **Configure the database**:
    Set up your MySQL database using the provided scripts.

4. **Upload code to Arduino**:
    Upload the Arduino code to the **Arduino Mega 2560** using the Arduino IDE.

5. **Run the Web Interface**:
    Use XAMPP or any local server to run the PHP.

## 📝 Future Features
- **Mobile App**: Expand functionality to mobile platforms for teachers and administrators.
- **Detailed Reporting**: Generate reports on attendance and energy savings.

## 🤝 Contributing
Contributions are welcome! Feel free to submit pull requests or open issues for any suggestions or improvements.

## 📧 Contact
For more information or questions:
- **GitHub**: [@leostall](https://github.com/leostall)
- **LinkedIn**: [@leonardostall](https://www.linkedin.com/in/leonardostall)
- **Email**: leonardostall28@gmail.com
