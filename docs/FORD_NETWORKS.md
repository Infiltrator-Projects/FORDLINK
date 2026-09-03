# Ford network model

FORDLINK keeps Ford vehicle-network topology separate from module identity,
diagnostic data and service procedures.

The initial topology vocabulary is:

| Network | Current meaning in FORDLINK |
| --- | --- |
| HS-CAN | High-speed Ford CAN lane used for the standard OBD path. Direct DLC access is represented with pins 6/14 and a 500 kbit/s default. |
| MS-CAN | Ford medium-speed enhanced-diagnostics lane. Direct DLC access is represented with pins 3/11 and a 125 kbit/s default. Adapter support must be explicit; a generic ELM327 name is not enough. |
| HS-CAN2..HS-CAN5 | Additional high-speed Ford lanes. They are profile-specific until vehicle evidence establishes how the diagnostic interface reaches them. |
| MS-CAN2 | Additional medium-speed Ford lane. It is profile-specific until vehicle evidence establishes access. |

The bitrate values are defaults for network modelling, not permission to force a
vehicle onto that bitrate. A vehicle profile may override them when verified
evidence exists.

## Clean-room boundary

The structure was informed by observing the behaviour and architecture of mature
Ford diagnostic software, including the separation of vehicle networks, module
inventory, DTCs, live data, tests and service procedures.

FORDLINK does **not** import or reproduce FORScan's native code, proprietary
module database, PID catalogue, service-procedure table, security algorithms or
vehicle-profile data.

Enhanced module definitions added later must carry their own provenance and be
supported by public documentation, user-captured evidence, manufacturer
information, or independently verified captures.

## Current classifier

The only automatic network classification performed today is deliberately
narrow: standard 11-bit OBD responder IDs 0x7E8 through 0x7EF are classified as
HS-CAN. Other physical responder IDs remain `Unknown` until the manufacturer
layer has evidence for them. This prevents a module address from silently
becoming a Ford-specific claim merely because it resembles a known address.

This model is the foundation for a later profile-driven scanner where each
verified module can declare its network, request/response address, protocol,
identity reads, DTC support and service capabilities independently.
