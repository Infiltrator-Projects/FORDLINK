# Generic diagnostic baseline

FORDLINK is intentionally not a polished Ford-specific product yet, but it is
not an empty shell.

The product pins LINK and exposes the same reusable diagnostic foundations used
by MBLINK and JAGLINK:

- adapter capability modelling and transport ABI;
- ELM327 protocol/session/CAN/probe engines;
- SAE OBD-II/J1979 requests, decoding, generic DTC knowledge and OBDonUDS;
- ISO-TP over Classical CAN and CAN FD;
- the full 27-service UDS catalogue and shared UDS client machinery;
- transport-neutral diagnostic request selection;
- portable DoIP framing;
- deny-by-default read-only discovery safety and evidence primitives;
- standard parameter definitions, storage and scheduling;
- telemetry/history/export primitives;
- generic ECU-probe and diagnostic-flow machinery;
- shared KWP2000 support where applicable.

What is intentionally missing is Ford-specific knowledge: Ford module maps,
proprietary PIDs/DIDs, manufacturer DTC meanings, security algorithms and
brand-specific workflows. Those belong in FORDLINK only when evidence supports
them.

The `fordlink-generic-diagnostics` test protects this baseline independently
of the separate standard OBD-II regression.
