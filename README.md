# Time Manipulation Tool

This tool bypasses cheat engine DLL speed hack detection methods by intercepting and manipulating the system clock. It allows for configurable time acceleration while ensuring synchronization upon termination.

## Example Detection Code

Below is an example of a detection mechanism for speed hacks, example of what this bypasses. it is often implemented in games or applications to identify tampering with time.

```gdscript
# Time interval to check for speed hack (in seconds)
@export var check_interval: float = 5.0

# Maximum allowed speed discrepancy (e.g., 1.2 means 20% faster than real time is allowed)
@export var max_speed_discrepancy: float = 1.2
@export var min_speed_discrepancy: float = 0.8

# Check for speed hacks and slowdowns
func check_speed_hack() -> void:
    var os_time_now: int = Time.get_unix_time_from_system()
    var game_time_now: int = Time.get_ticks_msec()

    var os_time_elapsed: float = os_time_now - os_time_start
    var game_time_elapsed: float = (game_time_now - game_time_start) / 1000.0

    # Check if the interval has passed
    if game_time_elapsed >= check_interval:
        # Calculate the ratio of in-game time to OS time
        var time_ratio: float = game_time_elapsed / os_time_elapsed
        if time_ratio > max_speed_discrepancy or time_ratio < min_speed_discrepancy:
            entity.network.on_reconnect_timeout()

        # Reset the start times for the next interval
        os_time_start = os_time_now
        game_time_start = game_time_now
```

## Features

- **Time Acceleration**: Customize the speed multiplier to adjust how fast time progresses.
- **Graceful Termination**: Restores system time to the correct value upon exit.
- **Real-Time Synchronization**: Ensures system time remains accurate after manipulation.
- **Configurable Update Intervals**: Control the frequency of time updates for performance optimization.

## How It Works

1. **System Time Manipulation**: The tool modifies the system clock by calculating a fake time based on the elapsed real time multiplied by a configurable speed multiplier.
2. **Real-Time Updates**: The system time is periodically updated to simulate accelerated or slowed time.
3. **Synchronization on Exit**: On termination, the tool synchronizes the system clock with real-world time to avoid desynchronization issues.

## Configuration

You can configure the tool by modifying parameters in `TimeController.h`:

- `SPEED_MULTIPLIER`: Multiplier for time acceleration (default: `15.0`).
- `TIME_UPDATE_INTERVAL`: Interval in milliseconds for updating the system clock (default: `100ms`).
