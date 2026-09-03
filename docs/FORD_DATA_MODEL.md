# Ford diagnostic data model

FORDLINK models live data as module-scoped signal definitions that can feed
Table, Dashboard and Graph views. `FordlinkSignalDefinition` stores the source
(UDS DID or broadcast CAN frame), identifier, byte and bit offsets, endianness,
signedness, scale factor, additive offset, unit, display range, precision and
presentation flags. `fordlink_signal_decode()` is bounded and range checked.

This is the landing point for independently verified Ford offsets/formulas. No
FORScan proprietary PID rows are shipped.

The manufacturer layer also contains a service/test capability taxonomy for
module self-tests, component tests, ABS bleed/calibration, BMS reset, DPF
regeneration/reset, EGR and EPB maintenance, KAM reset, injector/pilot/fuel-pump
learning, turbo-vane reset, TPMS learning, TCM adaptation, module
reset/configuration, camera/headlamp/occupant/ride-height calibration and
misfire-profile learning.

These entries are capability metadata, not executable commands. Every
state-changing family requires a verified vehicle/module profile before it can
be enabled.
