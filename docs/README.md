# telakone-firmware


## ChibiOS

Needs following repositories:
* https://github.com/kimmoli/ChibiOS in ../ChibiOS
* https://github.com/kimmoli/ChibiOS-Contrib in ../ChibiOS-Contrib

or change location in Makefile

## Shell

### status

Shows temperatures and voltages etc.

### out

Sets digital output channels; on, off or PWM dutycycle

### reboot

Reboots

## Threads
### Blinker

Blinks the green led

### Joystick

Converts joystick position to motor differential movement

<img alt="Joystick plot" src="joystick.png">

