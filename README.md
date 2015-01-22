Controls RGB LED strip lights using an Arduino, and a custom I/O board.

There are lots of RGB strip lights available, and most have the same very basic, and nasty
control box, which doesn't bring out the full potential of these strips.

This project is part hardware, part software, and is so much better than the bog standard box!

Use the built in set of colour sequences, or define them yourself.
Any changes are saved to the Arduino's EEPROM, so nothing will be lost when you turn off the lights!

For each of your colour sequences, bring them to life using many different "Modes".

Modes include :

* _Jump_ (jumps from one colour to the nexts)
* _Fade_ (gently fades from one colour to the next)
* _Flash_ (turns off between each colour)
* _Fade-out_ (fades to off between each colour)
* _Double_ flash (Each colour is flashed on twice, before moving to the next colour)
* _Twinkle_ (Each colour is rapily turned on an off, like a twinkling star)
* _Alternate_ (violently flashes between pairs of colours)
* _Fade Cubic_ (like fade, but more agressive)
* _White-out_ (fades to white between each colour)

To use this code, you'll need to make a custom circuit board to accompany your Arduino.
At the time of writing, this hasn't been finalised, and I have no circuit diagram yet. Sorry.

