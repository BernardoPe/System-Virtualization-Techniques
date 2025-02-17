## 1.3

The most relevant entry in table 8-1 while processing the Present bit is the #PF (Page fault) exception field. 

This exception occurs when the present bit is set to 0 in the page translation table, meaning the page or table has not been loaded in physical memory yet. 

Whenever this exception happens, the table/page has to be loaded in memory, and then the Present bit set to 1.