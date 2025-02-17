## 1.1

If the architectural limit for physical memory were to be increased to 64PB, The page tables' base address bit range would have to be extended from bits 51-12 to bits 55-12 in every figure,
and the amount of bits in the Page tables' output has to increased  from 52 to 56. 

Current architectural limit = Page size * Total pages

\[ 2^{12} \times 2^{40} \text{ bytes} = 4503599627370496 \text{ bytes} = 4 \text{ PB} \]

\[ 2^{12} \times 2^{40} \rightarrow 4 \text{ PB} \quad x \rightarrow 64 \text{ PB} \]

\[ \frac{2^{12} \times 2^{40} \times 64}{4} = 2^{52} \times 16 = 2^{52} \times 2^{4} \]

Therefore, we need to add 4 bits to the range for the limit to be increased to 64 PB.