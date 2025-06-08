# Smart Traffic Light Simulation

This project simulates an intelligent traffic light control system for an intersection. The simulation adapts the light cycles dynamically based on the current traffic load and ensures safe passage for vehicles without collisions.

---

## Overview

- The intersection consists of an arbitrary number of incoming roads (e.g., north, south, east, west).
- Each road has its own queue of vehicles waiting to cross the intersection.
- The system continuously updates the traffic lights based on current queue conditions.
- Only non-conflicting queues are given green lights at any time.
- Vehicles move forward only when their road has a green light.
- The simulation state is updated based on a JSON command list.

---

## Traffic Light Logic

- Each road has a light with three states: `RED`, `GREEN` or `YELLOW`.
- Road light is determined via global `lights` bitmask.
- The simulation evaluates all combinations of queues with vehicles and selects the one with:
  - The **highest total score** (queue length + wait time).
  - **No path conflicts** (i.e., vehicles do not intersect).
- Bitmasks are used to represent which roads are currently allowed to move.
- Light states are updated **immediately after any command**, including adding vehicles.
- If a road is marked as good to go, but the current command is not `step`, its light is yellow (the last bit in the lights bitmask indicates whether the command is `step`).
- Thanks to this scoring-based selection, roads that experience long waiting times are guaranteed to eventually receive green light — preventing starvation.
- Diagonal traffic movements (e.g., north → east and south → west) are not considered to be in conflict. The simulation assumes that the intersection platform is sufficiently large to accommodate all vehicles in such scenarios, however it can be changed if needed, because it is controlled by a simple logic function.
- Vehicles are allowed to turn back to the road they came from (e.g., north → north), enabling flexible routing.


---

### Bitmask Format (technical detail)

The bitmask used for determining active green/yellow roads follows this format:

- Lower bits (0–N−1): represent individual roads, where 1 means the road is allowed to move.
- Final bit (N): represents whether this is a `step` command.

Example (5-bit system for 4 roads):
  Bit 0: north
  Bit 1: east
  Bit 2: south
  Bit 3: west
  Bit 4: step (1 = GREEN, 0 = YELLOW)

  Mask = 0b10101
  → Roads 0 and 2 active, and it's a `step` command ⇒ north/south = GREEN

This representation makes yellow light handling trivial: no need for state machines or delay counters.

---

## Light State Determination

After any command:
- The system calculates which roads are eligible to proceed.
- If the command is `addVehicle`, those roads are marked as `YELLOW`.
- If the command is `step`, the same roads are upgraded to `GREEN` and one vehicle from each is dequeued.

## Input Format (JSON)

The simulation accepts a list of commands in a JSON file:

```json
{
  "commands": [
    {
      "type": "addVehicle",
      "vehicleId": "vehicle1",
      "startRoad": "south",
      "endRoad": "north"
    },
    {
      "type": "step"
    }
  ]
}
```

### Supported Commands

- `addVehicle`: Adds a vehicle to the corresponding queue.
- `step`: Advances the simulation by one unit, allowing vehicles with green lights to proceed.

---

## Output Format (JSON)

The program generates an output JSON with all vehicles that left the intersection during each simulation step:

```json
{
  "stepStatuses": [
    {
      "leftVehicles": ["vehicle1"]
    },
    {
      "leftVehicles": []
    }
  ]
}
```

Each step contains:
- `leftVehicles`: A list of vehicle IDs that passed through during that step.

---

## Building and Running

### Build the program:

```bash
make -s
```

### Run the simulation:

```bash
make -s run input.json output.json
```

Or run with printing light states for each step:

```bash
make -s debug-run input.json output.json
```

Or manually:

```bash
./bin/simulation input.json output.json
```

The `-s` flag is used to suppress makefile command output, resulting in a cleaner terminal view.

---

## Testing

The project includes unit tests for all modules. Run tests with:

```bash
make -s test
```

Each test prints whether it passed or failed, along with a summary.

---

## Project Structure

```
├── src/
│   ├── main.c
│   ├── parser.c
│   ├── step.c
│   ├── lights.c
│   └── ...
├── include/
│   ├── queue.h
│   ├── vehicle.h
│   ├── lights.h
│   └── ...
├── test/
│   ├── vehicle_test.c
│   ├── queue_test.c
│   ├── test_main.c
│   └── ...
├── bin/
├── makefile
└── README.md
```

---
## Performance

The brute-force bitmask evaluation scales with the number of active queues (O(2^k)). However, in practice:

- Most real-world intersections have ≤ 8 active directions.
- The evaluation is fast and executed only when necessary (after each command).
- The road selection algorithm could be replaced with a polynomial-time dynamic algorithm, but such implementation is complex in pure C — so it remains a possible future enhancement.

---

## Worth Noting

- The entire project is easily portable to most embedded platforms, thanks to built-in mockups of otherwise problematic standard functions.
- The program is scalable — it supports any number of roads, as long as they are listed in `common.h`.
- Thanks to its simplicity and modular structure, the application is easy to extend with additional features.

---

## Possible Extensions

- Support for pedestrian crossings.
- Priority queueing (e.g. for emergency vehicles).
- GUI visualization of intersection state.
