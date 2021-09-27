# Routine file syntax

Routines are pre-programmed experiments that can be loaded and executed at run-time. They are defined in text files, with each line being one instruction to execute.

For example, a file may look like this:

    # Example routine.
    valve 1 open
    valve 2 close
    pressure 1 3.5
    wait 1 minute
    valve 1 close


There are three main commands that can be used: toggling valves, setting pressure regulator setpoints, and waiting/pausing. In addition, comments are supported: anything following a hash (`#`) is ignored.

## Toggle valves

To toggle a valve, the format of the command is:
    
    valve <number> <open/close>

For example,  `valve 10 open` or `valve 3 close`. You may also use "all" to toggle all valves at once: `valve all open` or `valve all close`.

## Set pressure

To give a pressure setpoint, use:

    pressure <number> <value in PSI>

For example,  `pressure 2 1.5` sets the setpoint of regulator #2 to 1.5 PSI.

## Wait / pause

To pause execution, use:

    wait <time> <unit>

For example, `wait 1 minute` or `wait 30 seconds`. Multiple formats are supported for units, and the default is seconds (including if no unit is specified). The complete list of supported formats for units is:

- Milliseconds: `ms / milliseconds / millisecond / msec`
- Minutes: `minutes / minute / min / mins`
- Hours: `hours / hour / hrs / hr / h`
- Seconds: anything else. e.g. `wait 5` is interpreted as "wait for 5 seconds".

## Multiplexer

This is specific to the multiplexer designed into our co-culture chips (v4 and v5). These multiplexers uses 6 valves to direct flow to 8 different channels, or 10 valves to 32 channels.
To set the multiplexer in a routine, simply use:

    multiplexer <command>

Where `command` corresponds to the text on any of the buttons displayed in the multiplexer selection screen. For example on our v5 graphical control screen, it could be a number 1-32, or  `1-8`,`all`,`none`, `odd`, `even`, etc. 
