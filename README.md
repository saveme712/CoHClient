# CoHClient
A modification that allows for various manipulations of the City of Heroes client.

## Homecoming
This client targets homecoming. The provided offsets are for homecoming and must be updated by hand. This can
easily be done going forward with bindiff.  
  
You _WILL_ get banned for using this in game, but hopefully after you cause some chaos. :)

# Usage
1. Build as x64 Release using Visual Studio
2. Inject using your favorite injector (e.g. ProcessHacker) at the login screen

It is very important to inject before logging in, because we patch out the hardware id tracking so that they
cannot keep you banned across accounts.
