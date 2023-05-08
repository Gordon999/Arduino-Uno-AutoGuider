# Arduino-Uno-AutoGuider

AutoGuiding a telescope with an Arduino Uno and PHD etc

This was developed to AutoGuide a Meade LX10, but could be used for other scopes including those with a ST-4 interface.

It uses an Arduino Uno to accept control commands, Meade Ascii codes, from suitable guide software on a PC, or Raspberry Pi,
via USB, eg PHD, and drives 4 outputs to move the Scope.

there is also a pico version https://github.com/Gordon999/RPi-Pico-Autoguider

See [Pi-AutoGuider](https://github.com/Gordon999/Pi-AutoGuider) for the Raspberry Pi AutoGuider.

You need to provide the Relay Card. The details here show how to use a Seeed 4 Relay card but you could use any suitable relays 
or solid state relays.

Please take care when interfacing to your scope. At your own risk.
