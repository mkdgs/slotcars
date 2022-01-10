# slotcars
slot cars experiments

==Protocole carrera==
- https://www.slotforum.com/threads/arduino-based-magracing.77829/ http://researchcommons.waikato.ac.nz/handle/10289/8661
- http://redlichelectronics.de/decoder_software.html
- http://www.wasserstoffe.de/carrera-hacks/infrarot-erkennung/index.html
- http://slotbaer.de/
- http://ssdc.jackaments.com/interface.shtml


==project==
- https://github.com/tkem/CarreraDigitalControlUnit/tree/07faf4bee84cc8cf4e29c74a9abe9e63c0172eb3
- https://github.com/thilbig/carrera-wifi/blob/f21c371769c1814df0e334b3ee0ddfcfd52c1c4b/readme.md
- https://github.com/tkem/carreralib/blob/7d7dea4991c433dfdf799ba74be1feef49879187/README.rst
- https://github.com/frechilla/qslotracing/blob/f74a8743ee748bf311a98b9765e92ad176ce4a9c/src/scxmsgfactory.h
- https://github.com/erjosito/carreradash/blob/4abf54cc99fe3df2c96cb1f7b1245b59354fd58c/README.md
- https://github.com/ctretter/CarreraAI/tree/d76c5d5b1214deebad0f8937ff75a34065f88ec7
- https://github.com/tkem/openlap
- https://github.com/buntine/laser-drift/blob/6fca9944d6feb1a7505e203ab3eee7a1fe188716/README.md
- https://github.com/azya52/carrera

Mesure de temps/impulsion
- https://www.f-legrand.fr/scidoc/docmml/sciphys/arduino/frequence/frequence.html
- https://locoduino.org/spip.php?id_article=198&page=imprime

PWM génération signal
- https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm

IR arduino

- https://github.com/NicoHood/IRLremote


Wavgat arduino card 
- https://github.com/paraplin/wavgat-board


Fisrt log fail:

Caveat: some IR receivers cannot handle a continuous signal and turn off. You need to check the data sheet for your IR receiver.
Either of these should work fine: TSSP4038 TSSP58038 from Vishay (they run at 5V or 3.3V)
from:
https://forum.arduino.cc/t/follow-an-ir-signal/184755/4

Explanation
https://electronics.stackexchange.com/questions/228690/ir-receiverrx-not-receiving-a-continuously-signal-from-ir-ledtx-rather-than


Composant:
IR sensor OSRAM SFH309FA | Vishay TEFT4300

IR emiter Vishay TSAL4400  | may be IR du type SFH487 

