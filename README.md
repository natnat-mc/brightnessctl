# brightnessctl
`brightnessctl` is a tool designed to configure the brightness of the screen without `xbacklight` or `sudo`, using a simple suid binary.

## Installing
- Clone this repository
- `make`
- `sudo make install`

## Using
- `brightnessctl`: prints the current brightness
- `brightnessctl <percent>`: sets the brightness to a specific value
- `brightnessctl '+'<percent>`: adds a specific value to the brightness
- `brightnessctl '-'<percent>`: removes a specific value from the brightness

## License
This software is released under the MIT License.

