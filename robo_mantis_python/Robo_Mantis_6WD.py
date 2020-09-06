#!/usr/bin/env python
# coding: utf-8

# In[127]:


import smbus2
import time
#use this to read all the I2C 
#devices of the sensors connected

bus = smbus2.SMBus(1) # 1 indicates /dev/i2c-1
arduino_mega_address = 9

#motors 1,4,6 are on the right side
#motors 2,3,5 are on the left side
def control_motors(motor_values):
    motor_command = 1
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(0.05)
    
def set_motor(index,percent=10,duration=1):
    motor_command = 1
    motor_values= [127,127,127,127,127,127]
    motor_values[index] = motor_values[index] + percent
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
def forward_left_motors(percent=10,duration=1):
    motor_command = 1
    motor_values= [127,127+percent,127+percent,127,127+percent,127]
    
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
def reverse_left_motors(percent=10,duration=1):
    motor_command = 1
    motor_values= [127,127-percent,127-percent,127,127-percent,127]
    
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
def forward_right_motors(percent=10,duration=1):
    motor_command = 1
    motor_values= [127+percent,127,127,127+percent,127,127+percent]
    
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
def reverse_right_motors(percent=10,duration=1):
    motor_command = 1
    motor_values= [127-percent,127,127,127-percent,127,127-percent]
    
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
        

def forward(percent=10,duration=1):
    motor_command = 1
    motor_values= [127+percent,127+percent,
                   127+percent,127+percent,
                   127+percent,127+percent]
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
def reverse(percent=10,duration=1):
    motor_command = 1
    motor_values= [127-percent,127-percent,
                   127-percent,127-percent,
                   127-percent,127-percent]
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
def turn(percent=10,duration=1):
    motor_command = 1
    motor_values= [127+percent,127-percent,
                   127-percent,127+percent,
                   127-percent,127+percent]
    
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(duration)
    stop()
    
    
def stop():
    motor_command = 1
    motor_values= [127,127,127,127,127,127]
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(0.01)
    
def set_servos(servo1=100,servo2=90,servo3=50,servo4=90):
    motor_command = 2
    '''
    byte servoByte1 = 100; wrist
    byte servoByte2 = 90; claw
    byte servoByte3 = 50; lift
    byte servoByte4 = 50; main lift
    '''
    motor_values= [servo1,servo2,servo3,servo4]
    bus.write_i2c_block_data(arduino_mega_address,
                             motor_command,
                             motor_values) 
    time.sleep(0.01)
    
#forward(40,3)
#reverse(10,2)
# for i in range(6):
#     set_motor(i,40)
#forward_left_motors(50)
#forward_right_motors(50)
#reverse_right_motors(50)


# In[177]:


try:
    forward(127,5)
    #reverse(127,2)
except OSError:
    print("I/O ERROR failed")
    stop()
finally:
    stop()


# In[94]:


for i in range(50):
    set_servos(servo2=90-i,servo3=50,servo4=80)
    time.sleep(0.05)


# In[99]:


set_servos()


# In[102]:


turn(-70,0.5)


# In[113]:


forward(-100,0.5)


# In[120]:





# In[124]:


stop()


# In[104]:


try:
    forward(50)
    turn(-50,0.5)
    forward(50)
    turn(-50,0.5)
    forward(50)
    turn(-50,0.5)
    forward(50)
finally:
    stop()


# In[ ]:




