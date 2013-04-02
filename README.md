jira-radiator
=============

Personal JIRA radiator for physical alerts to issues

- Flashing alert for new bugs
- Consistent notification colour for existing old bugs
- "All-clear" colour for bug-free project

Hardware
------------
Arduino controlling an RGB LED, such as Macetech ShiftBrite.
Connected to controlling workstation with USB.

Software
------------
Python script running on workstation parsing JIRA issues and sending serial commands to Arduino.
