# Breeze: IoT Weather Station

## Project Overview
Breeze is an advanced IoT weather station designed to measure environmental variables including temperature, humidity, wind speed, wind direction, and atmospheric pressure. The collected data is transmitted to an IoT platform for real-time visualization, allowing users to monitor weather conditions in specific areas. This information is particularly valuable for making informed decisions about outdoor activities such as windsurfing, kitesurfing, and other wind-dependent sports.

## Introduction

### General Objective
The primary goal of this project is to develop an integrated IoT device/system comprising multiple electronic components connected to each other and to the cloud. This system is specifically designed to capture relevant meteorological data, process it in the cloud, and execute actions based on this data. The information is presented in a clear and accessible manner to end users through a web platform.

For our project, we have chosen to design and develop an advanced weather station. This station is equipped to measure key variables such as wind speed and direction, as well as ambient temperature. The collected data is transmitted to an IoT platform where the information is processed and analyzed.

The IoT platform enables real-time visualization of the collected data, as well as the generation of personalized alerts and recommendations for the end user. This provides useful and timely information that allows users to make informed decisions about wind sports activities.

The weather station is especially valuable in environments where precise measurement of these variables is crucial, such as areas dedicated to wind sports like kitesurfing, windsurfing, and paragliding. By offering accurate real-time data, the station helps athletes optimize their activities, improve their safety, and enjoy their practices more effectively.

## Problem Statement
In environments where these sports are practiced, weather conditions can change abruptly and unexpectedly. This variability makes it crucial for athletes to have real-time information about wind conditions and other environmental factors to assess whether it is convenient or safe to practice their sport at a given time.

## Project Objective
The objective of our project is to provide athletes with complete and up-to-date information necessary for decision-making. The IoT weather station we will develop will have the following key functionalities:

- **Real-Time Monitoring**: Capturing relevant meteorological data, such as wind speed and direction, and temperature, with high frequency to reflect changes in real time.

- **Data Processing**: Sending the collected data to an IoT platform in the cloud, where it will be processed and analyzed to offer an accurate assessment of environmental conditions.

- **Visualization and Alerts**: Development of a web interface that will present the information in a clear and accessible way for the end user. The platform will also generate automatic alerts and personalized recommendations based on the collected data, allowing users to act quickly in response to any significant change in conditions.

## Learning Aspects
From an educational perspective, this project will allow us to apply and consolidate the knowledge acquired in electronics and electrical engineering. We will focus on:

- **Electronic Circuit Design**: Integration of sensors for precise measurement of meteorological variables.

- **Programming and IoT Connectivity**: Development of the necessary software for data transmission to the cloud and creation of the web platform for visualization.

- **Optimization and Efficiency**: Implementation of techniques to ensure that the device is functional, efficient, and reliable under different environmental conditions.

- **Mechanical Systems**: Design and development of a physical device capable of containing all project components and providing protection against inclement weather.

From a learning perspective, we seek to apply knowledge of both electronics and software development to create a device that meets the project objectives.

## Work Plan
To build the system, we began by integrating the key components: the anemometer, wind vane, and central board. Initially, we focused on interconnecting these elements with the board and 3D printing the first parts, which would serve as prototypes for subsequent adjustments and optimizations.

As a first step, we analyzed the physical and electrical behavior of each device, studying how to connect each one to the central board and to the other components to ensure they worked together. Once the basic connections were established, we conducted measurement tests on the central board. We started with a basic functional design, to which we added additional functionalities as we progressed in the development of each peripheral.

In parallel, we began developing the system logic on the ThingsBoard platform, using rule chains to structure the data flow. This logic allows us to receive in the cloud the data captured by the device peripherals, process them, and, together with external data and conditions configured on the platform, send control signals to the device to activate different functionalities. This process has been detailed and progressive, as we tested the communication between the board and the cloud, adding and improving functionalities incrementally until achieving a complete and refined system.

Initially, we worked with the central board and a power supply similar to those available in the laboratory. It was here that we integrated the temperature and humidity sensors, evaluating their performance and connecting them to the central board to conduct tests that guaranteed their correct operation. Once we obtained satisfactory results, we proceeded with the integration of additional peripherals, ensuring that all components operate in a coordinated manner and that the data are properly received and processed in the cloud.

At this stage, we began designing the dashboard to visualize both the data received from our device and those obtained through the API. The objective of this comparison is to verify the accuracy and reliability of our device's readings. We have already identified the first contrasts between both data sources, and the results have been satisfactory: the values are practically identical, although we can still make additional adjustments to improve sensing accuracy.

The next step consisted of sending signals to activate specific functions, such as the protection mechanism. In this phase, we worked directly with the cloud to properly process data from the sensors and external APIs.

Once we achieved the system's automatic protection, we consolidated a solid and complete system, with the main functionalities operating correctly and in coordination. From this point, we focused on optimizing and adding new functionalities to make the system more efficient and complete.

## Physical Design
The system will initially operate in a normal operating mode. The wind vane is located at the top, while the anemometer is located at the bottom.

The interior of the cylinder houses the mechanical and electronic system, which includes a central rod that vertically traverses the device and connects to a spiral guide mechanism driven by a servomotor. In addition, two additional guide rods have been installed that pass through the lower platform. These rods stabilize the platform's movement when rising and falling in protection mode or sleep mode, minimizing possible deviations in its trajectory and ensuring correct displacement.

The platform supporting the anemometer is located at the bottom of the cylinder and is equipped with a three-blade anemometer with hemispheres. Given its position, when protected, it is out of reach of external factors that could damage it, such as rain.

If the anemometer had been placed at the top along with the wind vane, the protection system would have required an additional cover that would be activated when the anemometer was completely inside the casing. This cover would need to include a rubber edge to ensure an effective seal inside the casing tube and thus prevent rain water from entering.

The incorporation of the solar panel led us to carefully analyze its location to optimize sunlight capture and, consequently, maximize energy generation. To achieve this, we considered the daily path of the sun, strategically orienting the panel so that it receives as much solar radiation as possible. The orientation will also allow us to calibrate the wind vane so that it can correctly report wind direction.

In case of using multiple devices, the location of each panel should be adapted both to the sun's trajectory in its specific position and to the alignment of the wind vane, which we take as a reference.

## Solution Details

### Wind Speed
#### Anemometer Development
We used an optical detection module from SunFounder. We designed and 3D printed a three-blade rotor with equally spaced hemispherical blades, aerodynamically optimized to capture wind force. The blades are mounted on a rotational system with a low-friction bearing at the base, allowing free and smooth rotation.

Each time the optical sensor detects a blade, it records the time elapsed until the next detection. This time interval allows calculating the rotation frequency, from which we estimate the angular velocity of the assembly. Finally, this angular velocity is converted into an accurate estimate of wind speed using a calibration constant specific to the anemometer design. This conversion process offers a reliable approximation of wind speed based on the system's characteristics.

### Wind Direction
To measure wind direction, we use a KY-040 rotary encoder to which we attached a 3D-manufactured wind vane. The vane applies a rotational force to the encoder's axis, causing it to rotate until it finds the position of least resistance and aligns with the wind direction.

The ESP32 board monitors the encoder's rotation in real time, recording the angular displacement of the vane. The initial position of the encoder is calibrated beforehand to align it with geographic north, so that any change in the vane's position can be interpreted in terms of cardinal directions. This system allows precise tracking of wind direction by processing the pulses generated by the encoder, establishing a direct correlation between the angular position of the axis and the direction in degrees with respect to north.

### Sleep Mode
To optimize energy consumption, we designed a routine within the program that allows the device to enter light sleep mode. This mode significantly reduces energy consumption, which translates into a notable improvement in device autonomy, prolonging the operating time before requiring a battery recharge.

The implementation of this routine is based on the configuration of wake-up pins, which are programmed to "wake up" the device upon receiving a signal. In our case, we defined the pins associated with the wind vane as wake-up pins. This is because the movements of the vane must be continuously recorded to maintain an accurate reference of wind direction. Additionally, a wakeup timer is configured that activates the device every hour. However, this interval can be adjusted so that the device wakes up less frequently, if necessary, to further optimize energy consumption.

When the device enters sleep mode, it suspends all non-essential operations and executes only the subroutines necessary to monitor the wake-up pins. If these detect a signal, the device can immediately reactivate to process the relevant information. By minimizing tasks in this state, energy consumption is drastically reduced, maximizing system efficiency and prolonging its operating time.

### Temperature, Humidity, and Pressure
To measure temperature, humidity, and atmospheric pressure, we use the BME/BMP280 environmental sensor module. This module employs the I2C communication protocol, which allows transferring data from multiple sensors through two lines: SCL (Serial Clock Line) and SDA (Serial Data Line). This configuration is especially efficient, as it allows acquiring and transmitting various environmental parameters using only two pins of the ESP32 development board, optimizing the use of the microcontroller's input and output resources.

### Protection Mode
The protection mechanism uses an MG996R continuous rotation servomotor, which drives a threaded rod to displace the platform that supports the anemometer system. When rotating in one direction, the threaded rod moves the platform towards the interior of the plastic casing, protecting all system components. This design allows automatic protection of the sensors and the anemometer, minimizing their exposure to adverse environmental conditions when not in use.

This mechanism is activated when certain conditions defined in the rules configured within ThingsBoard are met. These conditions, considered together, represent a picture of adverse weather at the coordinates where the device is located.

The conditions are based on data obtained from the API, which include information on precipitation and wind speed. In addition, direct measurements of wind speed made by our device are integrated to complement the evaluation. The mechanism is activated if the precipitation exceeds a defined threshold value and if the wind speed, either estimated (from the API) or directly measured, exceeds an established limit.

Once the mechanism is activated, the servomotor begins to rotate in a specific direction according to the required action: to protect the device or to remove it from protection. When the device is protected, the servomotor continues to rotate until it reaches a stop or limit switch button, installed as a physical limit to the movement. This button automatically stops the servomotor, indicating that the protection has been completed.

### Power Supply
#### Initial Power Supply (Without Solar Panel)
In the initial stages of the project, it was decided to use rechargeable 18650-type batteries. For the preliminary design, two 18650 batteries were connected in series, each with a nominal voltage of 3.7V. This series configuration allowed obtaining a total voltage of approximately 7.4V (with variations depending on the state of charge, between 7V and 8V). Then, using a step-down module, the desired 5V for device operation was achieved.

#### Solar Panel Integration
The project faced a challenge when trying to integrate a solar panel to charge the 18650 batteries. First, charge regulators available in the market are primarily designed to handle a single 18650 battery, making it impossible to charge two batteries connected in series. To solve this problem, the system was simplified by reducing the configuration to a single 18650 battery. A charge controller compatible with this battery was used, connected to the solar panel to efficiently manage recharging. Subsequently, a step-up module (voltage booster) was incorporated to increase the battery's output voltage from 3.7V to 5V. Although reducing to a single cell decreases the system's total energy capacity, the implementation of an adequately sized solar panel provides the possibility of continuous operation without the need for manual recharging.

## IoT Platform: ThingsBoard
We used ThingsBoard in its demo version as our IoT platform. This platform offers a set of functionalities sufficient for our project. The most notable functionalities that this platform allows us to perform are:

- Management of dynamic and static variables
- Sending email alerts
- Creation of shared variables to be read by devices
- Visualization of sensed data (dashboard)
- Easy scalability in case of installation of many devices

Once we entered the platform, the first step was to create the necessary devices for the project. This initial configuration laid the foundations for implementing the specific functionalities of each device.

After configuring the devices, we began developing the rule chains, which are responsible for executing all the associated logic. These chains allow automating and managing device operations efficiently.

Among the main actions that can be performed through the rule chains are obtaining data from external APIs related to weather, reading and updating variables, executing alarms, sending email notifications, and changing device states, such as activating protection or switching to sleep mode.

### Alerts
We decided to include alert generation as a key feature, as we consider it can be very useful for users. This functionality allows them to receive important information about the device's status without the need to constantly monitor the data panel.

For alert transmission, we chose email, as it is a medium widely used by users and, furthermore, it is integrated into the version of ThingsBoard we are using. This simplifies its implementation and ensures greater accessibility for most users.

In our case, alerts will be sent each time the device enters protection mode. This will inform users when weather conditions are not suitable for sports practice. This approach significantly improves the user experience by offering proactive and relevant notifications.

### Data Presentation: Dashboards
Once all mechanisms were configured for data to periodically enter the platform and be stored in the device's telemetry, a visualization panel was created for users. This dashboard allows integrated display of data collected by the device, predictions from the API, device locations, and other key metrics.

The final dashboard is organized into two columns of widgets, each intended for different data sets.

In the first column, data obtained from an external API is shown. This data is acquired through several code blocks, which are responsible for uploading them to the corresponding entity within the platform. The inclusion of this real data is valuable, as it allows direct comparison with the data recorded by our device.

In the second column, data measured directly by the device is visualized. Widgets for historical comparison have also been added, which allow observing the variation of both data sets over time. This facilitates both real-time and historical analysis, allowing evaluation of discrepancies between API data and experimental data.

Additionally, a map was incorporated that shows the location of active Breeze devices along the country's coasts. Each device was manually configured, specifying its latitude and longitude coordinates for visualization on the map. Although we currently only have one functional device, this widget establishes a basis for future developments, allowing that, as more devices are implemented at different points in the country, their respective data can be identified and consulted.

The link to the dashboard is as follows: http://demo.thingsboard.io/dashboard/1c677ba0-9269-11ef-af67-a38a7671daf5?publicId=3d91b040-9d2a-11ef-af67-a38a7671daf5
