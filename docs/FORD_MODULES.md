# Ford module and endpoint model

FORDLINK separates module identity from diagnostic endpoint candidates. Ford can
reuse a module acronym across platforms, move it between HS-CAN/MS-CAN, place
it behind a gateway, or use a different physical address on another generation.
Multiple endpoint candidates are therefore allowed for one module.

The initial catalogue includes common Ford modules such as PCM, TCM, ABS, BCM,
IPC, APIM, GWM, IPMA/IPMB, RCM, DDM/PDM, DSM, FCIM/FCDIM, ACM, DSP, PSCM, PAM,
GFM/GFM2, GPSM, HUD, HSWM, RTM, RBM and HVAC. The endpoint set is based on Ford
service-information module charts and independently corroborated open
implementations/captures. Conflicting platform families are retained as
separate candidates instead of silently choosing one.

The manufacturer module census is explicitly user-triggered after the standard
diagnostic pass; FORDLINK does not automatically probe Ford module endpoints on
connection. When requested, it probes the corroborated HS-CAN candidates. For
each endpoint it asks a bounded set of standard ISO 14229 identification DIDs
(manufacturer part/software number, serial, VIN, hardware number and system
name) plus ReadDTCInformation. Every diagnostic PDU passes through LINK's
read-only Discover policy. No session change, security access, routine, reset,
DTC clear, configuration write or programming command is permitted.

MS-CAN and additional gateway-exposed networks remain represented but are not
automatically selected until adapter-level Ford network switching is explicitly
implemented and tested.

FORScan Lite NG was inspected to understand architecture only: network-aware
discovery, module-scoped data and separate DTC/live/test/service domains.
FORDLINK does not copy FORScan native code, private database rows, PID tables,
security algorithms or procedure payloads.
