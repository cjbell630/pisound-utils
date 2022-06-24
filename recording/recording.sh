#!/bin/bash

. /usr/local/pisound/scripts/common/common.sh

FNAME="recording_$(date +%F_%H-%M-%S)" # naming convention for recording files

# EDIT THE FOLLOWING BEFORE USING THIS SCRIPT!!!
BT_MAC=00:00:00:00:00:00 # MAC address of the bluetooth device to send the files to
BT_CH=12 # obex channel of the bluetooth device (12 for Samsung)


if screen -list | grep -q "recording"; then # if recording already started
        screen -XS "recording" quit # stop current recording
        periodic_led_blink 3 0.5 # start flashing the LEDs quicker

        mv /home/patch/current_recording.wav "/home/patch/$FNAME.wav" # rename the file just recorded

        ffmpeg -i "/home/patch/$FNAME.wav" -vn -ar 44100 -ac 2 -b:a 192k "/home/patch/$FNAME.mp3" # convert to mp3

        # transfer mp3 to bluetooth device, continue if it fails (denied by device or failed to connect)
        obexftp --nopath --noconn --uuid none --bluetooth $BT_MAC --channel $BT_CH -v --put "/home/patch/$FNAME.mp3" || echo "failed to send"

        rm -rf "/home/patch/$FNAME.mp3" # delete the mp3 file

        # NOTE: uncomment and modify the following line if you want to transfer the file to an external drive
        # mv "/home/patch/$FNAME.wav" "/flash_drive/$FNAME.wav" # move the wav file to the flash drive

        periodic_led_blink 0 0 # stop flashing the LEDs
else # if no recording in progress
        periodic_led_blink 3 1 # start flashing the LEDs slowly to indicate recording
        screen -dm -S recording jack_capture /home/patch/current_recording.wav # start recording
fi