# PublicTransferNetwork
simulate public transport network with diffrent type off transit and station
using smartPointers and move semantics
class transit (abstract class)

transit type(inheritor):

    - Bus
    - Tram
    - Sprinter
    - Rail
class Stations (abstract class):
Stations type(inheritor):

    - Intercity
    - Central
    - Stad

the program will receive a config file with the "stoppingTime" of a given transit in any type of station
and "changingTransitTime" of any given station , the diff 
