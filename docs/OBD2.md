# Standard OBD-II baseline

FORDLINK does not need Ford-specific data to perform standards-based emissions
diagnostics. The pinned LINK dependency supplies the generic SAE OBD-II engine.

The FORDLINK facade intentionally delegates to LINK rather than copying it.
The baseline includes supported-PID discovery, standard current-data and
freeze-frame request/decoding, readiness, VIN, stored/pending/permanent DTC
reads, generic SAE DTC decoding/knowledge, and OBDonUDS PID/DID mapping.

This is **standard OBD coverage**, not a claim of Ford proprietary diagnostics.
Ford module maps, proprietary identifiers, manufacturer DTC meanings and
brand-specific diagnostic workflows belong in FORDLINK when evidence supports
them.

The `fordlink-obd2` CI test proves that the pinned LINK stack can:
- build 0100, 010C, 0902 and Mode 03 requests;
- decode a standard RPM payload;
- decode a standard P0133 DTC;
- map classic PID 0x0C to OBDonUDS DID F40C.
