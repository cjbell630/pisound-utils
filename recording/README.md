# recording.sh

### Pisound button script for recording audio

---

### Description:

> This script toggles audio recording from the Pisound, and is meant to be used with the Pisound button. When a
> recording is started, the LEDs will begin flashing to indicate a recording is in progress. When it's stopped it will
> do the following:
> - Save the recording as a WAV file
> - Temporarily convert it to an MP3 file, and transfer it to the specified Bluetooth device
> - Delete the MP3 file
> - [OPTIONAL] Move the WAV file to external storage
>
> While this is happening, the LEDs will flash quicker to indicate the transfer. The file is saved with the date, so
> previous recordings do not get overwritten.

### Requirements:

* jack_capture
  ```shell
  sudo apt-get install jack_capture
  ```
* GNU screen
  ```shell
  sudo apt-get install screen
  ```

### Installation:

1. Modify `recording.sh` as specified below in [Customization](#customization).
2. For use with Pisound button:
   1. Place `recording.sh` in `/usr/local/pisound/scripts/pisound-btn/`
   2. Run `patchbox`, and select "button" > "assign"
   3. Choose the button action you want to assign recording toggle to
   4. Choose "Recording" from the list of actions
   5. "Back" > "Exit" to close the patchbox wizard

### Customization:

* REQUIRED
    * Bluetooth Device MAC Address
      * In the following line, type your device's MAC address. (If you've already used the Pisound app, you can find
        this easily by running `bluetoothctl` and typing `devices`)
        ```shell
        BT_MAC=00:00:00:00:00:00
        ```
    * OBEX Channel
      * In the following line, type your device's OBEX channel (it's probably 12, but if that doesn't work, you can use `sdptool browse [MAC ADDRESS] | grep "0x1105" -A 4 | grep "Channel"` to find it)
        ```shell
        BT_CH=12
        ``` 
* OPTIONAL
  * External Storage
    * The script can move the recording off of the micro SD and onto mounted external storage. This is particularly useful for getting these files off quickly and easily.
    * Uncomment the following line, and replace `/flash_drive/` with the mounted path to your external storage.
      ```shell
      # mv "/home/patch/$FNAME.wav" "/flash_drive/$FNAME.wav" # move the wav file to the flash drive
      ```