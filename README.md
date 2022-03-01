# lzw435
## Description
* A universal lossless data compression algorithm created by Abraham Lempel, Jacob Ziv, and Terry Welch
  * It is the basis of many PC utilities that claim to "double the capacity of your hard drive"
  * Unix file compression utility **compress**
  * GIF image,...
* A greedy algorithm; it tries to recognize increasingly longer and longer phrases that are repetitive, and encode them
* Universal coding schemes, like LZW, do not require advance knowledge and can build such knowledge on-the-fly.
* LZW compression uses a code table, with 4096 = 2<sup>12</sup> as a common choice for the **number of table entries**.
* Codes 0-255 = ASCII table
* The code table contains only the first 256 entries, with the remainder of the table being blanks.
* Compression is achieved by taking each code from the compressed file, and translating it through the code table in the same manner as encoding.
## Running
```g++ -0 lzw435 lzw435.cpp``` to build
* Compression: <pre><code>./lzw435 c [filename]</pre></code>
* Decompression: <pre><code>./lzw435 e [filename]</pre></code>
