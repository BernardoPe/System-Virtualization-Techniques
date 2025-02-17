## 1.2

Currently supported virtual address space 

\[ 2^{57} \text{ bytes} = 144115188075855870 \text{ bytes} = 128 \text{ PB} \]

Needed bits for 512 PB 

\[ 2^{57} \times \frac{512}{128} = 2^{57} \times 4 = 2^{59} \]

Since level 5 is limited to 512 entries, we would need to create a Page Map Level 6 Table 4 entry table with a 2 bit offset, that maps to the level 5 Table.

After these changes the level 6 table would have 2^2 = 4 entries that map to a level 5 table, meaning a single process could have 4 level 5 tables.

A canonical address is an address where all the unused bits of the virtual 
address space are the same value, so either 1 or 0. In this case, since
we have a 59 bit virtual address space, the canonical address ranges are the 
ranges where the first 5 bits are either set to 0 or 1.

Canonical address ranges: 
\[ 0000\_0000\_0000\_0000 - 07FF\_FFFF\_FFFF\_FFFF \]
\[ F800\_0000\_0000\_0000 - FFFF\_FFFF\_FFFF\_FFFF \]