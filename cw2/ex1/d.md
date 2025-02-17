# 1.4 
The code snippet in lines 378 - 397 appears to set up the Extended Feature Enable Register to determine which features can be enabled, allowing specific bits to be set on the Page-Translation-Table entry fields. 

If the EFER register has NX set to 1 (No Execute supported) then the Page-Translation-Table can have the NX field set to either 0 or 1. If the NX bit is set to 1 without the EFER having it set to 1. A #PF exception occurs.

The code from lines 387-390 checks if the No Execute feature is supported, setting the NX bit in the EFER register if it is supported. This allows the NX bit to be set in the Page-Translation-Table entry fields.
