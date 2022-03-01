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
* lzw435.cpp:
```g++ -o lzw435 lzw435.cpp``` to build
  * Compression: <pre><code>./lzw435 c [filename]</pre></code> - this will create a compressed .lzw version of the file.
  * Decompression: <pre><code>./lzw435 e [filename.lzw]</pre></code> - this will decompress the .lzw version of the file as a second version without any added file extension (i.e. as testCase2, not testCase.txt).
* lzw435M.cpp:
```g++ -o lzw435M lzw435M.cpp``` to build
  * Compression: <pre><code>./lzw435M c [filename]</pre></code> - this will create a compressed .lzw2 version of the file with variable bit lengths depending on the bit length specified.
  * Decompression: <pre><code>./lzw435M e [filename.lzw2]</pre></code> - this will decompress the .lzw2 version as a second version of the file without any added file extension (i.e. as testCase2M, not testCase.txt).
